#include "window.hpp"

namespace BdUI{
    Window::Window(){
        Size = BdUI::Size{ 1000, 800 };
        Location = BdUI::Point{200,100};
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

    void Window::WindowCursorDefaultBind() {
#ifdef WIN32
        CaptionCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        CloseCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        SizeCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        ZoomCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        ReduceCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        HelpCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
        SysMenuCursor = Cursor(LoadCursor(NULL, IDC_ARROW));
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
        Cursor& Cur = w->CurrentCursor;
        switch (msg)
        {
            case WM_SETCURSOR: {
                SetCursor(Cur);
                break;
            }
            case WM_RBUTTONDOWN: {
                mouse.Content.Button = 1;
                mouse.Content.Up_Down = 1;
                break;
            }
            case WM_LBUTTONDOWN: {
                mouse.Content.Button = 2;
                mouse.Content.Up_Down = 1;
                break;
            }
            case WM_MBUTTONDOWN: {
                mouse.Content.Button = 3;
                mouse.Content.Up_Down = 1;
                break;
            }
            case WM_XBUTTONDOWN: {
                mouse.Content.Button = HIWORD(wParam) == 1 ? 4 : 5 ;
                mouse.Content.Up_Down = 1;
                break;
            }
            case WM_RBUTTONUP: {
                mouse.Content.Button = 1;
                mouse.Content.Up_Down = 0;
                break;
            }
            case WM_LBUTTONUP: {
                mouse.Content.Button = 2;
                mouse.Content.Up_Down = 0;
                break;
            }
            case WM_MBUTTONUP: {
                mouse.Content.Button = 3;
                mouse.Content.Up_Down = 0;
                break;
            }
            case WM_XBUTTONUP: {
                mouse.Content.Button = HIWORD(wParam) == 1 ? 4 : 5;
                mouse.Content.Up_Down = 0;
                break;
            }
            case WM_MOUSEWHEEL: {
                mouse.WheelDelta = HIWORD(wParam);
            }
            case WM_MOUSELEAVE: {
                BdUI::Mouse&& parent_mouse = w->Mouse;
                parent_mouse.Content.IsLeaved = 1;
                w->Mouse = parent_mouse;
                Context = w;
                break;
            }
            case WM_MOUSEHOVER: {
                mouse.Content.Hover_Move = 0;
                mouse.Location = { LOWORD(lParam),HIWORD(lParam) };
                mouse.Content.IsLeaved = 1;
                break;
            }
            case WM_MOUSEMOVE: {
                mouse.Content.Hover_Move = 1;
                mouse.Location = { LOWORD(lParam),HIWORD(lParam) };
                if (w->Mouse.get().Content.IsLeaved == 1 || mouse.Content.Hover_Move == 0) {
                    TRACKMOUSEEVENT tme;
                    tme.cbSize = sizeof(tme);
                    tme.hwndTrack = hWnd;
                    tme.dwFlags = TME_HOVER | TME_LEAVE;
                    tme.dwHoverTime = Context->MouseHoverTime;
                    TrackMouseEvent(&tme);
                }
                UI* newContext = SearchUI_NearPos(mouse.Location, Context);
                if (newContext != Context) {
                    BdUI::Mouse&& oldmouse = Context->Mouse;
                    oldmouse.Content.IsLeaved = 1;
                    Context->Mouse = oldmouse;
                    Context = newContext;
                }
                Cur = Search_Area_Cursor(mouse.Location, newContext);
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
    #endif
    #pragma endregion
}