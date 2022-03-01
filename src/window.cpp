#include "window.hpp"

namespace BdUI{
    Window::Window(){
        #ifdef _WIN32
        Size = {CW_USEDEFAULT,CW_USEDEFAULT};
        Location = {CW_USEDEFAULT,CW_USEDEFAULT};
        #endif
    }
    Window::~Window(){
        Mutex.unlock();
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
    }
    void Window::WindowDefaultEventBind(){
        VisibleChanged += Delegate<void(bool)>(&Window::VisibleEvent,this);
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
        SetWindowWord(hWnd,0,reinterpret_cast<long>(this));
        WindowDefaultEventBind();
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
        Window *w = reinterpret_cast<Window*>(GetWindowWord(hWnd,0));
        printf("%p\n",w);
        printf("%d\n",msg);
        switch(msg){/*
            #pragma region Mouse
            case WM_MOUSEMOVE:{
                //OnMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                MouseMove(mouse);
                break;
            }
            case WM_MOUSEHOVER:{
                //ResetMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                MouseHover(mouse);
                break;
            }
            case WM_MOUSELEAVE:{
                //ResetMouseHoverAndLeave();
                Mouse &&mouse{{},Mouse::None,0};
                MouseLeave(mouse);
                break;
            }
            case WM_MOUSEWHEEL:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,GET_WHEEL_DELTA_WPARAM(wParam)};
                MouseWheel(mouse);
                break;
            }
            case WM_RBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                MouseDoubleClick(mouse);
                break;
            }
            case WM_LBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                MouseDoubleClick(mouse);
                break;
            }
            case WM_MBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                MouseDoubleClick(mouse);
                break;
            }
            case WM_XBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                MouseDoubleClick(mouse);
                break;
            }
            case WM_RBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                MouseDown(mouse);
                break;
            }
            case WM_LBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                MouseDown(mouse);
                break;
            }
            case WM_MBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                MouseDown(mouse);
                break;
            }
            case WM_XBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                MouseDown(mouse);
                break;
            }
            case WM_RBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                MouseUp(mouse);
                break;
            }
            case WM_LBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                MouseUp(mouse);
                break;
            }
            case WM_MBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                MouseUp(mouse);
                break;
            }
            case WM_XBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                MouseUp(mouse);
                break;
            }
            #pragma endregion
            case WM_SETCURSOR:{
                struct Cursor cursor = Cursor.Get();
                switch(LOWORD(lParam)){
                    case HTCLIENT:{
                        SetCursor(cursor.Client);
                        break;
                    }
                    case HTCAPTION:{
                        SetCursor(cursor.Caption);
                        break;
                    }
                    case HTLEFT:{
                        SetCursor(cursor.LeftBorder);
                        break;
                    }
                    case HTRIGHT:{
                        SetCursor(cursor.RightBorder);
                        break;
                    }
                    case HTTOP:{
                        SetCursor(cursor.TopBorder);
                        break;
                    }
                    case HTBOTTOM:{
                        SetCursor(cursor.BottomBorder);
                        break;
                    }
                    case HTBOTTOMLEFT:{
                        SetCursor(cursor.BottomLeft);
                        break;
                    }
                    case HTTOPLEFT:{
                        SetCursor(cursor.TopLeft);
                        break;
                    }
                    case HTTOPRIGHT:{
                        SetCursor(cursor.TopRight);
                        break;
                    }
                    case HTCLOSE:{
                        SetCursor(cursor.Close);
                        break;
                    }
                    case HTSIZE:{
                        SetCursor(cursor.Size);
                        break;
                    }
                    case HTSYSMENU:{
                        SetCursor(cursor.SystemMenu);
                        break;
                    }
                    case HTMENU:{
                        SetCursor(cursor.Menu);
                        break;
                    }
                    case HTZOOM:{
                        SetCursor(cursor.Zoom);
                        break;
                    }
                    case HTREDUCE:{
                        SetCursor(cursor.Reduce);
                        break;
                    }
                    case HTHELP:{
                        SetCursor(cursor.Help);
                        break;
                    }
                    case HTVSCROLL:{
                        SetCursor(cursor.VScroll);
                        break;
                    }
                    case HTHSCROLL:{
                        SetCursor(cursor.HScroll);
                        break;
                    }
                    case HTERROR:{
                        SetCursor(cursor.Error);
                        break;
                    }
                    case HTTRANSPARENT:{
                        SetCursor(cursor.Transparent);
                        break;
                    }
                }
                break;
            }
            */
            case WM_DESTROY:{
                w->~Window();
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