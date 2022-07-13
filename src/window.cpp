#include "window.hpp"

namespace BdUI{
    bool Window::IsLoadOpenGL = false;
    Window::Window(){
        WindowEventDefaultBind();
        WindowCursorDefaultBind();

        Size.setOnly(BdUI::Size( 1000, 800,UnitType::Pixel ));
        Location.setOnly(Point(5,5,UnitType::cm));
        Background.setOnly(RGB{ 255,255,255 });
        VSync.setOnly(true);
        Focus = this;
    }
    Window::~Window(){
        delete Thread;
    }
    bool Window::Create(){
        Thread = new std::thread(&Window::WindThread,this);
        Thread->detach();
        if(!Creation.get_future().get()){
            delete Thread;
            Thread = nullptr;
            return false;
        }
        return true;
    }
    void Window::Block(){
        Mutex.lock();
        Mutex.unlock();
    }
    void Window::WindowEventDefaultBind(){
        delete Mouse.set_func;
        Mouse.set_func = nullptr;
        Background.set_func = new Delegate<bool(Color, Color&)>(&Window::WindowSetBackground, this);
        Size.set_func = new Delegate<bool(BdUI::Size,BdUI::Size&)>(&Window::WindowSizeChange, this);
        Location.set_func = new Delegate<bool(Point,Point&)>(&Window::WindowLocationChange, this);
        VSync.set_func = new Delegate<bool(bool, bool&)>(&Window::WindowSetVSync, this);
        ClientSize.set_func = new Delegate<bool(BdUI::Size, BdUI::Size&)>(&Window::WindowSetClientSize, this);
        Visible.set_func = new Delegate<bool(bool, bool&)>(&Window::WindowSetVisible, this);
    }
    void Window::WindowCursorDefaultBind() {
#ifdef WIN32
        WindowCursor.Caption = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Close = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Size = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Zoom = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Reduce = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Help = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.SysMenu = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
#endif
    }

    void Window::WindThread(){
#ifdef _WIN32
        const WNDCLASSEX Windowclass{
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,
            Window::__WndProc,
            0,
            8,
            GetModuleHandle(NULL),
            NULL,
            NULL,
            NULL,
            NULL,
            TEXT("BdUI_WindowClass"),
            NULL,
        };
        if (!RegisterClassEx(&Windowclass)) { //注册窗口类
            Creation.set_value(false);
            return;
	    }
        Point location = Location;
        location.ChangeUnit(UnitType::Pixel);
        BdUI::Size size = Size;
        size.ChangeUnit(UnitType::Pixel);
        hWnd = CreateWindowEx(dwExStyle,Windowclass.lpszClassName,NULL,dwStyle,location.X,location.Y,size.Width,size.Height,NULL,NULL,Windowclass.hInstance,NULL);
        if(hWnd == NULL){
            Creation.set_value(false);
            return;
        }
        ShowWindow(hWnd, Visible ? SW_SHOW : SW_HIDE);
        hDC = GetWindowDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
        
        int&& render = ChoosePixelFormat(hDC, &pfd);
        if (render == 0) {
            Creation.set_value(false);
            DestroyWindow(hWnd);
            hWnd = nullptr;
            return;
        }
        SetPixelFormat(hDC, render, &pfd);
        hRC = wglCreateContext(hDC);
        if (hRC == 0) {
            Creation.set_value(false);
            DestroyWindow(hWnd);
            hWnd = nullptr;
            return;
        }
        wglMakeCurrent(hDC, hRC);
        if (!IsLoadOpenGL) {
            if (!gladLoadWGL(hDC)) {
                Creation.set_value(false);
                DestroyWindow(hWnd);
                hWnd = nullptr;
                return;
            }
            if (!gladLoadGL()) {
                Creation.set_value(false);
                DestroyWindow(hWnd);
                hWnd = nullptr;
                return;
            }
            IsLoadOpenGL = true;
        }
        
        wglSwapIntervalEXT(VSync);
        RGB rgb = Background.get().GetRGB();
        glClearColor(float(rgb.R) / 255, float(rgb.G) / 255, float(rgb.B) / 255, float(Background.get().GetAlpha()) / 255);
        SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(this));
        Creation.set_value(true);
        Mutex.lock();
        MSG msg;
        while(GetMessage(&msg,NULL,0,0) > 0){
            TranslateMessage(&msg);
		    DispatchMessageA(&msg);
        }
        Mutex.unlock();
#endif
    }

    #pragma region WindowEvent
    void Window::Paint() {
        if (GraphChanged == true) {
            glClear(GL_COLOR_BUFFER_BIT);
            for (auto i : UIList) {
                i->Paint(ClientSize);
            }
#ifdef _WIN32
            SwapBuffers(hDC);
#endif
            GraphChanged = false;
        }
    }

    bool Window::WindowSetText(const std::string& s) {
#ifdef WIN32
        if(hWnd != nullptr) SetWindowText(hWnd, TEXT(s.c_str()));
#endif
        return true;
    }
    bool Window::WindowSizeChange(BdUI::Size size,BdUI::Size& old) {
        old = size;
        size.ChangeUnit(UnitType::Pixel);
#ifdef WIN32
        if(hWnd != nullptr) SetWindowPos(hWnd, NULL, 0, 0, size.Width, size.Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS);
        if(IsLoadOpenGL) glViewport(0, 0, size.Width, size.Height);
#endif
        return true;
    }
    bool Window::WindowLocationChange(Point location, Point& old) {
        old = location;
        location.ChangeUnit(UnitType::Pixel);
#ifdef WIN32
        if(hWnd != nullptr) SetWindowPos(hWnd, NULL,location.X, location.Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOSENDCHANGING | SWP_ASYNCWINDOWPOS);
#endif
        return true;
    }
    bool Window::WindowSetBackground(Color n, Color& old) {
        RGB rgb = n.GetRGB();
        if(IsLoadOpenGL) glClearColor(float(rgb.R) / 255, float(rgb.G) / 255, float(rgb.B) / 255, float(n.GetAlpha()) / 255);
        old = n;
        return true;
    }
    bool Window::WindowSetVSync(bool n, bool& old) {
#ifdef _WIN32
        if (IsLoadOpenGL) {
            wglMakeCurrent(hDC, hRC);
            wglSwapIntervalEXT(n);
            wglMakeCurrent(NULL, NULL);
        }
#endif
        old = n;
        return true;
    }
    bool Window::WindowSetVisible(bool visible, bool& old) {
        old = visible;
        if (hWnd != nullptr) {
            ShowWindow(hWnd, visible ? SW_SHOW : SW_HIDE);
        }
        return true;
    }
    bool Window::WindowSetClientSize(BdUI::Size size, BdUI::Size& old) {
        old = size;
        size.ChangeUnit(UnitType::Pixel);
        if(IsLoadOpenGL) glViewport(0, 0, size.Width, size.Height);
        return true;
    }

    #ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
        Window *w = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
        switch(msg){
            case WM_MOVE: {
                w->Location.setOnly(Point(LOWORD(lParam), HIWORD(lParam), UnitType::Pixel));
                break;
            }
            case WM_SIZE: {
                w->Size.setOnly(BdUI::Size(LOWORD(lParam), HIWORD(lParam), UnitType::Pixel));
                RECT rect;
                GetClientRect(hWnd, &rect);
                w->ClientSize = BdUI::Size(rect.right - rect.left, rect.bottom - rect.top, UnitType::Pixel);
                w->GraphChanged = true;
                break;
            }
            case WM_SIZING: {
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                w->Size.setOnly(BdUI::Size((double)(rect->right - rect->left),(double)(rect->bottom - rect->top),UnitType::Pixel ));
                w->Location.setOnly(Point(rect->left, rect->top, UnitType::Pixel));
                RECT rec;
                GetClientRect(hWnd, &rec);
                w->ClientSize = BdUI::Size(rec.right - rec.left, rec.bottom - rec.top, UnitType::Pixel);
                w->GraphChanged = true;
                break;
            }
            case WM_MOVING:{
                RECT* r = reinterpret_cast<RECT*>(lParam);
                w->Location.setOnly(Point(r->left, r->top, UnitType::Pixel));
                break;
            }
            case WM_DESTROY:{
                wglMakeCurrent(NULL, NULL);
                wglDeleteContext(w->hRC);
                ReleaseDC(hWnd, w->hDC);
                PostQuitMessage(0);
                break;
            }
            case WM_CHAR: {
                UI* focus = w->Focus;
                BdUI::Key&& key = focus->Key;
                key.Code = wParam;
                key.RepeatCount = LOWORD(lParam);
                key.ScanCode = LOBYTE(HIWORD(lParam));
                key.Up_Down = !HIWORD(lParam) & KF_UP;
                while (focus->Parent.exist()) {
                    focus->Key = key;
                    if (focus->Key.ChangedEvent != nullptr) break;
                    focus = focus->Parent;
                }
                break;
            }
            case WM_KEYDOWN: {
                UI* focus = w->Focus;
                BdUI::Key key = focus->Key;
                key.VirtualKey = BdUI::Key::Type(wParam);
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
                BdUI::Key&& key = focus->Key;
                key.VirtualKey = BdUI::Key::Type(wParam);
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
                w->Paint();
                break;
            }
            default:{
                if (w == nullptr) return DefWindowProc(hWnd, msg, wParam, lParam);
                else return MouseProc(hWnd, msg, wParam, lParam, w);
            }
        }
        return 0;
    }

    LRESULT Window::MouseProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam,Window* w) {
        bool Ismouse = true;
        BdUI::Mouse mouse = w->MouseContext->Mouse;
        UI*& Context = w->MouseContext;
        const BdUI::Cursor* Cur = w->CurrentCursor;
        switch (msg)
        {
            case WM_SETCURSOR: {
                SetCursor(const_cast<BdUI::Cursor*>(Cur)->getIndex());
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
                mouse.Location = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), UnitType::Pixel);
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
            default:{
                Ismouse = false;
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
            if (Ismouse) {
                UI* c = Context;
                while (c->Parent.exist()) {
                    c->Mouse = mouse;
                    if (c->Mouse.ChangedEvent != nullptr) break;
                    c = c->Parent;
                }
            }
        }
        return 0;
    }

    void Window::MouseVitualKey(WPARAM wParam,BdUI::Mouse& m) {
        if (wParam & MK_CONTROL) m.Button.Ctrl = 1;
        else m.Button.Ctrl = 0;
        if (wParam & MK_SHIFT) m.Button.Shift = 1;
        else m.Button.Shift = 0;
        if (GetKeyState(VK_MENU) < 0) m.Button.Alt = 1;
        else m.Button.Alt = 0;
    }
    #endif
    #pragma endregion
}