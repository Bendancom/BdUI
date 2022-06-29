#include "window.hpp"

namespace BdUI{
    Window::Window(){
        Size = BdUI::Size{ 1000, 800 };
        Location = BdUI::Point{200,100};
        Mouse.set_func = Delegate<bool(BdUI::Mouse, BdUI::Mouse&)>();
        WindowDefaultEventBind();
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
    void Window::WindowDefaultEventBind(){

    }
    /*
    template<typename E,typename... T>
    static void Window::EventDeliver<E>(UI*, T...) requires(typeid(E) == typeid(EventArray<void(T...)>)) {

    }*/

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
            MessageBox(NULL, TEXT("Creating Window Failed"), NULL, MB_OK);
            Creation.set_value(false);
            return;
        }
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
    void Window::SetText(const std::string& s) {
#ifdef WIN32
        SetWindowText(hWnd, TEXT(s.c_str()));
#endif
    }

    #ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
        Window *w = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
        switch(msg){
            case WM_WINDOWPOSCHANGED: { //同时处理 WM_SIZE 与 WM_MOVE 消息
                WINDOWPOS *p = reinterpret_cast<WINDOWPOS*>(lParam);
                w->Location = Point{ p->x ,p->y };
                w->Size = BdUI::Size{ (unsigned long)p->cx,(unsigned long)p->cy };
                break;
            }
            case WM_MOVING:{
                RECT* r = reinterpret_cast<RECT*>(lParam);
                w->Location = Point{r->left,r->top};
                break;
            }
            case WM_DESTROY:{
                PostQuitMessage(0);
                break;
            }
            case WM_CHAR: {
                UI* focus = w->Focus;
                BdUI::Key&& key = focus->Key;
                key.code = wParam;
                key.RepeatCount = LOWORD(lParam);
                key.scan_code = LOBYTE(HIWORD(lParam));
                key.Up_Down = !HIWORD(lParam) & KF_UP;
                focus->Key = key;
                std::string s = "" + (char)lParam;
                w->SetText(s);
                break;
            }
            case WM_KEYDOWN: {
                UI* focus = w->Focus;
                BdUI::Key&& key = focus->Key;
                key.VirtualKey = BdUI::Key::Type(wParam);
                key.RepeatCount = LOWORD(lParam);
                key.scan_code = LOBYTE(HIWORD(lParam));
                key.Up_Down = 1;
                focus->Key = key;
                break;
            }
            case WM_KEYUP: {
                UI* focus = w->Focus;
                BdUI::Key&& key = focus->Key;
                key.VirtualKey = BdUI::Key::Type(wParam);
                key.RepeatCount = LOWORD(lParam);
                key.scan_code = LOBYTE(HIWORD(lParam));
                key.Up_Down = 0;
                focus->Key = key;
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
                mouse.Location = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
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
        }
        if (Ismouse) {
            Context->Mouse = mouse;
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