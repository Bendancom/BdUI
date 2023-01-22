#include <window.hpp>

namespace BdUI {
    void Window::WindThread() {
#ifdef _WIN32
        RGBA rgb = BackgroundColor.get().GetRGBA();
        BdUI::Icon icon = Icon;
        
        WNDCLASSEX Windowclass{
            sizeof(WNDCLASSEX),
            CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
            __WndProc,
            0,
            0,
            GetModuleHandle(NULL),
            icon,
            NULL,
            CreateSolidBrush(RGB(rgb.R,rgb.G,rgb.B)),
            NULL,
            TEXT("BdUI_WindowClass"),
            NULL,
        };

        if (!RegisterClassEx(&Windowclass)) throw error::Function::CarryOut_Faild("Faild to Register WindowClass");

        Point location = Location.get().ChangeUnit(UnitType::Pixel);
        BdUI::Size size = Size.get().GetData(UnitType::Pixel);

        hWnd = CreateWindowEx(dwExStyle, Windowclass.lpszClassName, "Window", dwStyle, location.X, location.Y, size.Width, size.Height, NULL, NULL, Windowclass.hInstance, NULL);
        if (hWnd == NULL) throw error::Function::CarryOut_Faild("Faild to create Window");

        Render.Initialize(hWnd);

        Size = Size.get();
        Location = Location.get();
        BackgroundColor = BackgroundColor.get();

        if (!VSync.exist()) VSync = true;
        else VSync = VSync.get();
        if (TitleText.exist()) TitleText = TitleText.get();
        else TitleText = "Window";

        WindowList[hWnd] = this;

        Mutex.lock();
        Creation.set_value(true);
        MSG msg;
        while (GetMessage(&msg, hWnd, 0, 0) > 0) {
            if (!TranslateAccelerator(hWnd, nullptr, &msg)) {
                if (msg.message == WM_KEYDOWN) {
                    if (msg.wParam == VK_PROCESSKEY)
                    {
                        msg.wParam = ImmGetVirtualKey(msg.hwnd);
                    }
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        Mutex.unlock();
#endif
    }
#ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        Window* w = WindowList[hWnd];
        if (w == nullptr) return DefWindowProc(hWnd, msg, wParam, lParam);
        switch (msg) {
        case WM_MOVE: {
            w->Location.setOnly(Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, UnitType::Pixel));
            break;
        }
        case WM_SIZE: {
            w->Size.setOnly(BdUI::Size(LOWORD(lParam), HIWORD(lParam), UnitType::Pixel));
            w->Render.PushMessage(glViewport,0, 0, LOWORD(lParam), HIWORD(lParam));
            w->GraphChanged = true;
            break;
        }
        case WM_SIZING: {
            RECT* rect = reinterpret_cast<RECT*>(lParam);
            w->Size.setOnly(BdUI::Size((double)(rect->right - rect->left), (double)(rect->bottom - rect->top), UnitType::Pixel));
            w->Location.setOnly(Point(rect->left, rect->top, 0, UnitType::Pixel));
            w->Render.PushMessage(glViewport, 0, 0, rect->right - rect->left, rect->bottom - rect->top);
            //w->Render.PushMessage();
            w->GraphChanged = true;
            break;
        }
        case WM_MOVING: {
            RECT* r = reinterpret_cast<RECT*>(lParam);
            w->Location.setOnly(Point(r->left, r->top, 0, UnitType::Pixel));
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_CHAR: {
            UI* focus = w->Focus;
            if (wParam >= 0x80 && wParam <= 0xff) {
                if (w->ANSI[0] == 0) {
                    w->ANSI[0] = wParam;
                }
                else {
                    w->ANSI[1] = wParam;
                    if (focus->KeyCharEvent != nullptr)
                        focus->KeyCharEvent->CarryOut(Character(w->ANSI));
                    w->ANSI = { 0,0 };
                }
            }
            else {
                if (w->ANSI[0] != 0) {
                    w->ANSI[1] = wParam;
                    if (focus->KeyCharEvent != nullptr)
                        focus->KeyCharEvent->CarryOut(Character(w->ANSI));
                    w->ANSI = { 0,0 };
                }
                else {
                    if (focus->KeyCharEvent != nullptr)
                        focus->KeyCharEvent->CarryOut(Character((char)wParam));
                }
            }
            break;
        }
        case WM_KEYDOWN: {
            UI* focus = w->Focus;
            BdUI::Key key;
            key.VirtualKey = BdUI::KeyType(wParam);
            key.RepeatCount = LOWORD(lParam);
            key.ScanCode = LOBYTE(HIWORD(lParam));
            key.Up_Down = 1;
            if (w->keylist.first != focus) {
                w->keylist.second.clear();
                w->keylist.first = focus;
            }
            else w->keylist.second += key.VirtualKey;
            if (focus->PopMenuKey.Isfind(w->keylist.second)) {
                focus->PopMenu.getReference()->PopUp(hWnd);
            };
            if (focus->KeyboardEvent != nullptr)
                focus->KeyboardEvent->CarryOut(key);
            break;
        }
        case WM_KEYUP: {
            UI* focus = w->Focus;
            BdUI::Key key;
            key.VirtualKey = BdUI::KeyType(wParam);
            key.RepeatCount = LOWORD(lParam);
            key.ScanCode = LOBYTE(HIWORD(lParam));
            key.Up_Down = 0;
            if (w->keylist.first != focus) w->keylist.second.clear();
            else w->keylist.second -= key.VirtualKey;
            if (focus->KeyboardEvent != nullptr)
                focus->KeyboardEvent->CarryOut(key);
            break;
        }
        case WM_PAINT: {
            if (w->GraphChanged == true) {
                w->Render.Render();
                w->GraphChanged = false;
            }
            break;
        }
        default: {
            return MouseProc(hWnd, msg, wParam, lParam, w);
        }
        }
        return 0;
    }

    LRESULT Window::MouseProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Window* w) {
        BdUI::Mouse mouse;
        UI*& MouseUI = w->CurrentMouseAtUI;
        BdUI::Cursor& Cur = w->CurrentCursor;
        switch (msg)
        {
        case WM_SETCURSOR: {
            BdUI::Cursor cur = Cur;

            if (MouseUI == w) {
                switch (LOWORD(lParam))
                {
                case HTBOTTOM: cur = w->Cursor.Border.Bottom; break;
                case HTBOTTOMLEFT: cur = w->Cursor.Border.BottomLeft; break;
                case HTBOTTOMRIGHT: cur = w->Cursor.Border.BottomRight; break;
                case HTTOP:cur = w->Cursor.Border.Top; break;
                case HTTOPLEFT: cur = w->Cursor.Border.TopLeft; break;
                case HTTOPRIGHT:cur = w->Cursor.Border.TopRight; break;
                case HTLEFT:cur = w->Cursor.Border.Left; break;
                case HTRIGHT: cur = w->Cursor.Border.Right; break;
                case HTCLIENT: cur = w->Cursor.Client; break;
                case HTCAPTION:cur = w->WindowCursor.Caption; break;
                case HTCLOSE: cur = w->WindowCursor.Close; break;
                case HTSIZE:cur = w->WindowCursor.Size; break;
                case HTSYSMENU:cur = w->WindowCursor.SysMenu; break;
                case HTREDUCE:cur = w->WindowCursor.Reduce; break;
                case HTZOOM:cur = w->WindowCursor.Zoom; break;
                case HTHELP:cur = w->WindowCursor.Help; break;
                }

                if (cur.getIndex() != Cur.getIndex()) {
                    if (w->CurrentCursorChanged != nullptr)
                        w->CurrentCursorChanged->CarryOut(cur);
                    Cur = cur;
                }
            }
            SetCursor(Cur);
            break;
        }
        case WM_RBUTTONDOWN: {
            mouse.Button.Right = 1;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_LBUTTONDOWN: {
            mouse.Button.Left = 1;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_MBUTTONDOWN: {
            mouse.Button.Middle = 1;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_XBUTTONDOWN: {
            if (GET_XBUTTON_WPARAM(wParam) & XBUTTON1) mouse.Button.X1 = 1;
            if (GET_XBUTTON_WPARAM(wParam) & XBUTTON2) mouse.Button.X2 = 1;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_RBUTTONUP: {
            mouse.Button.Right = 0;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_LBUTTONUP: {
            mouse.Button.Left = 0;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_MBUTTONUP: {
            mouse.Button.Middle = 0;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_XBUTTONUP: {
            if (HIWORD(wParam) == 1) mouse.Button.X1 = 0;
            else mouse.Button.X2 = 0;
            MouseVitualKey(wParam, mouse);
            break;
        }
        case WM_MOUSEWHEEL: {
            mouse.WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            break;
        }
        case WM_MOUSELEAVE: {
            mouse.Content = Mouse::Leave;
            break;
        }
        case WM_MOUSEHOVER: {
            mouse.Content = Mouse::Hover;
            mouse.Location = Point{ (double)GET_X_LPARAM(lParam),(double)GET_Y_LPARAM(lParam),0,UnitType::Pixel };
            break;
        }
        case WM_MOUSEMOVE: {
            if (mouse.Content == Mouse::Leave || mouse.Content == Mouse::Hover) {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = hWnd;
                tme.dwFlags = TME_LEAVE | TME_HOVER;
                tme.dwHoverTime = 1000;
                TrackMouseEvent(&tme);
            }
            mouse.Content = Mouse::Move;
            mouse.Location = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, UnitType::Pixel);
            break;
        }
        default: {
            return w->DefWindowProcess(hWnd, msg, wParam, lParam, w);
        }
        }
        UI* c = MouseUI;
        UI* focus = w->Focus;
        if (w->keylist.first != focus) {
            w->keylist.second.clear();
            w->keylist.first = focus;
        }
        if (mouse.Button.Right == 0)
            w->keylist.second -= KeyType::RButton;
        else w->keylist.second += KeyType::RButton;
        if (mouse.Button.Left == 0)
            w->keylist.second -= KeyType::LButton;
        else w->keylist.second += KeyType::LButton;
        if (mouse.Button.Middle == 0)
            w->keylist.second -= KeyType::MButton;
        else w->keylist.second += KeyType::MButton;
        if (mouse.Button.X1 == 0)
            w->keylist.second -= KeyType::XButton1;
        else w->keylist.second += KeyType::XButton1;
        if (mouse.Button.X2 == 0)
            w->keylist.second -= KeyType::XButton2;
        else w->keylist.second += KeyType::XButton2;
        if (focus->PopMenuKey.Isfind(w->keylist.second))
            focus->PopMenu.getReference()->PopUp(hWnd);
        return 0;
    }

    void Window::MouseVitualKey(WPARAM wParam, BdUI::Mouse& m) {
        if (wParam & MK_CONTROL) m.Button.Ctrl = 1;
        else m.Button.Ctrl = 0;
        if (wParam & MK_SHIFT) m.Button.Shift = 1;
        else m.Button.Shift = 0;
        if (GetKeyState(VK_MENU) < 0) m.Button.Alt = 1;
        else m.Button.Alt = 0;
    }
#endif
}