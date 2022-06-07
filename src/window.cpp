#include "window.hpp"

namespace BdUI{
    Window::Window(){
        #ifdef _WIN32
        Size = BdUI::Size{ CW_USEDEFAULT, CW_USEDEFAULT };
        Location = BdUI::Point{CW_USEDEFAULT,CW_USEDEFAULT};
        #endif
        WindowDefaultEventBind();
        CursorDefaultSet();
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
        Visible.Changed += Delegate<void(bool)>(&Window::VisibleEvent,this);
    }
    void Window::CursorDefaultSet(){
        /*
        CaptionCursor = BdUI::Cursor(IDC_ARROW);
        BorderCursor_Left = BdUI::Cursor(IDC_SIZEWE);
        BorderCursor_Right = BdUI::Cursor(IDC_SIZEWE);
        BorderCursor_Top = BdUI::Cursor(IDC_SIZENS);
        BorderCursor_Bottom = BdUI::Cursor(IDC_SIZENS);
        BottomLeftCursor = BdUI::Cursor(IDC_SIZENESW);
        BottomRightCursor = BdUI::Cursor(IDC_SIZENWSE);
        TopLeftCursor = BdUI::Cursor(IDC_SIZENWSE);
        TopRightCursor = BdUI::Cursor(IDC_SIZENESW);
        CloseCursor = BdUI::Cursor(IDC_ARROW);
        SizeCursor = BdUI::Cursor(IDC_ARROW);
        ZoomCursor = BdUI::Cursor(IDC_ARROW);
        ReduceCursor = BdUI::Cursor(IDC_ARROW);
        HelpCursor = BdUI::Cursor(IDC_ARROW);
        MenuCursor = BdUI::Cursor(IDC_ARROW);
        */
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
    void Window::VisibleEvent(bool visible){
        if (visible){
            #ifdef _WIN32
            ShowWindow(hWnd,SW_SHOW);
            UpdateWindow(hWnd);
            #endif
        }
        else{
            #ifdef _WIN32
            ShowWindow(hWnd,SW_HIDE);
            #endif
        }
    }

    #ifdef _WIN32
    LRESULT Window::__WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
        Window *w = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
        switch(msg){
            case WM_SETCURSOR:{
                switch(LOWORD(lParam)){
                    case HTCLIENT:{
                        //SetCursor(w->Cursor.Get());
                        break;
                    }
                }
                break;
            }
            case WM_DESTROY:{
                PostQuitMessage(0);
                break;
            }
            default:{
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        return 0;
    }
    #endif
    #pragma endregion
}