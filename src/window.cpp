#include "window.hpp"

namespace BdUI{
    bool Window::IsLoadOpenGL = false;
    Window::Window(){
        Size.setOnly(BdUI::Size{ 1000, 800 });
        Location.setOnly(BdUI::Point{ 200,100 });
        WindowEventDefaultBind();
        WindowCursorDefaultBind();
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
        Size.set_func = new Delegate<bool(BdUI::Size,BdUI::Size&)>(&Window::WindowSizeChange, this, Location.getPointer(), std::placeholders::_1, std::placeholders::_2);
        Location.set_func = new Delegate<bool(Point,Point&)>(&Window::WindowLocationChange, this, Size.getPointer(), std::placeholders::_1, std::placeholders::_2);
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

#ifdef _WIN32
    void Window::WindThread(){
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
		    MessageBox(NULL, TEXT("Register Class Failed!"), NULL, MB_OK);
            Creation.set_value(false);
            return;
	    }
        Point location = Location;
        BdUI::Size size = Size;
        hWnd = CreateWindowEx(dwExStyle,Windowclass.lpszClassName,NULL,dwStyle,location.X,location.Y,size.Width,size.Height,NULL,NULL,Windowclass.hInstance,NULL);
        if(hWnd == NULL){
            Creation.set_value(false);
            return;
        }
        hDC = GetWindowDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd = {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW,
            24,
            0,0,0,0,0,0,
            0,
            0,
            0,
            0,0,0,0,
            32,
            0,
            0,
            PFD_MAIN_PLANE,
            0,
            0,0,0
        };
        int&& render = ChoosePixelFormat(hDC, &pfd);
        if (render == 0) {
            Creation.set_value(false);
            DestroyWindow(hWnd);
            return;
        }
        SetPixelFormat(hDC, render, &pfd);
        hRC = wglCreateContext(hDC);
        if (hRC == 0) {
            Creation.set_value(false);
            DestroyWindow(hWnd);
            return;
        }
        wglMakeCurrent(hDC, hRC);
        if (!IsLoadOpenGL) {
            if (!gladLoadGL()) {
                Creation.set_value(false);
                return;
            }
            IsLoadOpenGL = true;
        }
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(this));
        Creation.set_value(true);
        Mutex.lock();
        MSG msg;
        while(GetMessage(&msg,NULL,0,0) > 0){
            TranslateMessage(&msg);
		    DispatchMessageA(&msg);
        }
        Mutex.unlock();
    }
    #endif

    #pragma region WindowEvent
    void Window::Show() {
#ifdef _WIN32
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
#endif
        Visible = true;
    }
    void Window::Hide() {
#ifdef _WIN32
        ShowWindow(hWnd, SW_HIDE);
#endif
        Visible = false;
    }
    bool Window::WindowSetText(const std::string& s) {
#ifdef WIN32
        SetWindowText(hWnd, TEXT(s.c_str()));
#endif
        return true;
    }
    bool Window::WindowSizeChange( const Point* location,BdUI::Size size,BdUI::Size& old) {
        old = size;
#ifdef WIN32
        MoveWindow(hWnd, location->X, location->Y, size.Width, size.Height, TRUE);
        glViewport(0, 0, size.Width, size.Height);
#endif
        return true;
    }
    bool Window::WindowLocationChange(const BdUI::Size* size, Point location, Point& old) {
        old = location;
#ifdef WIN32
        MoveWindow(hWnd, location.X, location.Y, size->Width, size->Height, TRUE);
#endif
        return true;
    }
    void Window::Paint() {
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex3f(-1, -1, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 1, 0);
        glColor3f(0, 0, 1);
        glVertex3f(1, -1, 0);
        glEnd();
        glFlush();
    }

    #ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
        Window *w = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
        switch(msg){
            case WM_WINDOWPOSCHANGED: { //同时处理 WM_SIZE 与 WM_MOVE 消息
                WINDOWPOS *p = reinterpret_cast<WINDOWPOS*>(lParam);
                w->Location.setOnly(Point(p->x, p->y, Pixel));
                w->Size.setOnly(BdUI::Size{ (unsigned long)p->cx,(unsigned long)p->cy });
                glViewport(0, 0, (unsigned long)p->cx, (unsigned long)p->cy);
                break;
            }
            case WM_SIZING: {
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                w->Size.setOnly(BdUI::Size{ (unsigned long)(rect->right - rect->left),(unsigned long)(rect->bottom - rect->top) });
                w->Location.setOnly(Point(rect->left, rect->top, Pixel));
                glViewport(0, 0, (unsigned long)(rect->right - rect->left), (unsigned long)(rect->bottom - rect->top));
                break;
            }
            case WM_MOVING:{
                RECT* r = reinterpret_cast<RECT*>(lParam);
                w->Location.setOnly(Point(r->left, r->top, Pixel));
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
                mouse.Location = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), Pixel);
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