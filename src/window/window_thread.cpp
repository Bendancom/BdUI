#include <window.hpp>

namespace BdUI {
    void Window::WindThread() {
#ifdef _WIN32
        RGB rgb = BackgroundColor.get().GetRGB();
        WNDCLASSEX Windowclass{
            sizeof(WNDCLASSEX),
            CS_VREDRAW | CS_HREDRAW | CS_OWNDC,
            Window::__WndProc,
            0,
            0,
            GetModuleHandle(NULL),
            NULL,
            NULL,
            CreateSolidBrush(RGB(rgb.R,rgb.G,rgb.B)),
            NULL,
            TEXT("BdUI_WindowClass"),
            NULL,
        };

        if (!RegisterClassEx(&Windowclass)) { //×¢²á´°¿ÚÀà
            Creation.set_value(false);
            return;
        }

        Point location = Location.get().GetData(UnitType::Pixel);
        BdUI::Size size = Size.get().GetData(UnitType::Pixel);

        hWnd = CreateWindowEx(dwExStyle, Windowclass.lpszClassName, "Window", dwStyle, location.X, location.Y, size.Width, size.Height, NULL, NULL, Windowclass.hInstance, NULL);
        if (hWnd == NULL) {
            Creation.set_value(false);
            return;
        }

        if (!OpenGLLoader()) {
            Creation.set_value(false);
            return;
        }

        Size = Size.get();
        Location = Location.get();
        BackgroundColor = BackgroundColor.get();
        if (!VSync.exist()) VSync = true;
        else VSync = VSync.get();

        WindowList[hWnd] = this;
        Creation.set_value(true);

        Mutex.lock();
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        Mutex.unlock();
#endif
    }
#ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        Window* w = WindowList[hWnd];
        //log.write(Log::Debug, std::to_string(msg));
        switch (msg) {
        case WM_MOVE: {
            w->Location.setOnly(Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, UnitType::Pixel));
            break;
        }
        case WM_SIZE: {
            w->Size.setOnly(BdUI::Size(LOWORD(lParam), HIWORD(lParam), UnitType::Pixel));
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
            w->GraphChanged = true;
            break;
        }
        case WM_SIZING: {
            RECT* rect = reinterpret_cast<RECT*>(lParam);
            w->Size.setOnly(BdUI::Size((double)(rect->right - rect->left), (double)(rect->bottom - rect->top), UnitType::Pixel));
            w->Location.setOnly(Point(rect->left, rect->top, 0, UnitType::Pixel));
            w->GraphChanged = true;
            break;
        }
        case WM_MOVING: {
            RECT* r = reinterpret_cast<RECT*>(lParam);
            w->Location.setOnly(Point(r->left, r->top, 0, UnitType::Pixel));
            break;
        }
        case WM_DESTROY: {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(w->hRC);
            ReleaseDC(hWnd, w->hDC);
            PostQuitMessage(0);
            break;
        }
        case WM_CHAR: {
            UI* focus = w->Focus;
            BdUI::Key key;
            if (focus->Key.exist()) key = focus->Key;
            key.Code = wParam;
            key.RepeatCount = LOWORD(lParam);
            key.ScanCode = LOBYTE(HIWORD(lParam));
            key.Up_Down = !HIWORD(lParam) & KF_UP;
            while (focus->Parent.exist()) {
                focus->Key = std::move(key);
                if (focus->Key.ChangedEvent != nullptr) break;
                focus = focus->Parent;
            }
            break;
        }
        case WM_KEYDOWN: {
            UI* focus = w->Focus;
            BdUI::Key key;
            if (focus->Key.exist()) key = focus->Key;
            key.VirtualKey = BdUI::KeyType(wParam);
            key.RepeatCount = LOWORD(lParam);
            key.ScanCode = LOBYTE(HIWORD(lParam));
            key.Up_Down = 1;
            while (focus->Parent.exist()) {
                focus->Key = key;
                if (focus->Key.ChangedEvent != nullptr) break;
                focus = focus->Parent;
            }
            break;
        }
        case WM_KEYUP: {
            UI* focus = w->Focus;
            BdUI::Key key;
            if (focus->Key.exist()) key = focus->Key;
            key.VirtualKey = BdUI::KeyType(wParam);
            key.RepeatCount = LOWORD(lParam);
            key.ScanCode = LOBYTE(HIWORD(lParam));
            key.Up_Down = 0;
            while (focus->Parent.exist()) {
                focus->Key = key;
                if (focus->Key.ChangedEvent != nullptr) break;
                focus = focus->Parent;
            }
            break;
        }
        case WM_PAINT: {
            if (w->GraphChanged == true) {
                w->OpenGLMutex.lock();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glFlush();
#ifdef _WIN32
                SwapBuffers(w->hDC);
#endif
                w->OpenGLMutex.unlock();
                w->GraphChanged = false;
            }
            break;
        }
        default: {
            if (w == nullptr) return DefWindowProc(hWnd, msg, wParam, lParam);
            return MouseProc(hWnd, msg, wParam, lParam, w);
        }
        }
        return 0;
    }

    LRESULT Window::MouseProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, Window* w) {
        bool Ismouse = true;
        BdUI::Mouse mouse;
        if (w->Mouse.exist()) mouse = w->Mouse;
        UI*& Context = w->MouseContext;
        BdUI::Cursor& Cur = w->CurrentCursor;
        switch (msg)
        {
        case WM_SETCURSOR: {
            SetCursor(Cur.getIndex());
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
        }
        case WM_MOUSELEAVE: {
            if (Context != w) {
                BdUI::Mouse&& parent_mouse = w->Mouse;
                parent_mouse.Content.IsLeaved = 1;
                w->Mouse = parent_mouse;
                Context = w;
            }
            else mouse.Content.IsLeaved = 1;
            break;
        }
        case WM_MOUSEHOVER: {
            mouse.Content.Hover_Move = 0;
            break;
        }
        case WM_MOUSEMOVE: {
            if (mouse.Content.IsLeaved || mouse.Content.Hover_Move == 0) {
                TRACKMOUSEEVENT tme;
                tme.cbSize = sizeof(tme);
                tme.hwndTrack = hWnd;
                tme.dwFlags = TME_LEAVE | TME_HOVER;
                tme.dwHoverTime = 0;
                TrackMouseEvent(&tme);
            }
            mouse.Content.Hover_Move = 1;
            mouse.Location = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), 0, UnitType::Pixel);
            mouse.Content.IsLeaved = 0;
            UI* newContext = SearchUI_NearPos(mouse.Location, Context);
            if (newContext != Context) {
                BdUI::Mouse&& oldmouse = Context->Mouse;
                oldmouse.Content.IsLeaved = 1;
                Context->Mouse = oldmouse;
                Context = newContext;
            }
            Cur = Search_Area_Cursor(mouse.Location, Context);
            break;
        }
        default: {
            Ismouse = false;
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        }
        if (Ismouse) {
            UI* c = Context;
            while (c->Parent.exist()) {
                c->Mouse = mouse;
                if (c->Mouse.ChangedEvent != nullptr) break;
                c = c->Parent;
            }
        }
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