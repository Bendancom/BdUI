#include "window.hpp"
namespace BdUI
{
    Window::~Window(){
        mutex.unlock();
        delete Thread;
        WindowList.erase(hWnd); 
    }

    bool Window::Create(){
        Thread = new std::thread(&Window::Initialization,this);
        Thread->detach();
        #ifdef _WIN32
        if (IsCreate.get_future().get()) return true;
        else{
            delete Thread;
            return false;
        }
        #endif
    }

    void Window::Initialization(){
        #ifdef _WIN32
        RegisterClassEx(&Wndclass);
        std::pair<int,int> style = Style;
        Point loca = Location.Get();
        BdUI::Size size = Size.Get(); 
        hWnd = CreateWindowEx (style.second,(STRING)ClassName.c_str(),(STRING)CaptionName.Get().c_str(),style.first,loca.X,loca.Y,size.Width,size.Height,NULL,NULL,hInstance,NULL);
        if (hWnd == NULL){
            IsCreate.set_value(false);
            return;
        }
        else IsCreate.set_value(true);
        WindowList[hWnd] = this;
        MSG msg;
        mutex.lock();
        while (GetMessage(&msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        mutex.unlock();
        this->~Window();
        #endif
    }

    bool Window::Show(){
        #ifdef _WIN32
        if (ShowWindow(hWnd,SW_SHOW)) return false;
        if (UpdateWindow(hWnd)) return false;
        #endif
        return true;
    }

    void Window::EventBind(){
        Size.EventList += SizeChanged;
        Location.EventList += LocationChanged;
        Cursor.EventList += CursorChanged;
        CaptionName.EventList += CaptionNameChanged;
    }

    #ifdef _WIN32
    void Window::OnMouseHoverAndLeave(){
        if(!IsOnMouseHoverAndLeave){
            TRACKMOUSEEVENT tme{sizeof(TRACKMOUSEEVENT),TME_HOVER|TME_LEAVE,hWnd,static_cast<DWORD>(MouseHoverTime)};
            TrackMouseEvent(&tme);
            IsOnMouseHoverAndLeave = true;
        }
    }
    void Window::ResetMouseHoverAndLeave(){ IsOnMouseHoverAndLeave = false; }
    #endif

    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        std::vector<HWND> hWndlist;
        HWND _hWnd = hWnd;
        while(_hWnd != NULL){
            hWndlist.push_back(_hWnd);
            _hWnd = GetParent(_hWnd);
        }
        Window *window = WindowList[_hWnd];
        for(int i = hWndlist.size() - 1; i >= 0;i--){
            for(std::vector<UI*>::iterator iter = window->UIList.begin();iter != window->UIList.end(); iter++){
                if((*iter)->hWnd == hWndlist[i]){
                    window = *iter;
                    break;
                }
            }
        }
        switch(msg){
            #pragma region MouseMessage
            case WM_MOUSEMOVE:{
                window->OnMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                window->MouseMove(mouse);
                break;
            }
            case WM_MOUSEHOVER:{
                window->ResetMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                window->MouseHover(mouse);
                break;
            }
            case WM_MOUSELEAVE:{
                window->ResetMouseHoverAndLeave();
                Mouse &&mouse{{},Mouse::None,0};
                window->MouseLeave(mouse);
                break;
            }
            case WM_MOUSEWHEEL:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,GET_WHEEL_DELTA_WPARAM(wParam)};
                window->MouseWheel(mouse);
                break;
            }
            case WM_RBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->MouseDoubleClick(mouse);
                break;
            }
            case WM_LBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->MouseDoubleClick(mouse);
                break;
            }
            case WM_MBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->MouseDoubleClick(mouse);
                break;
            }
            case WM_XBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->MouseDoubleClick(mouse);
                break;
            }
            case WM_RBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->MouseDown(mouse);
                break;
            }
            case WM_LBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                window->MouseDown(mouse);
                break;
            }
            case WM_MBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                window->MouseDown(mouse);
                break;
            }
            case WM_XBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->MouseDown(mouse);
                break;
            }
            case WM_RBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->MouseUp(mouse);
                break;
            }
            case WM_LBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                window->MouseUp(mouse);
                break;
            }
            case WM_MBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                window->MouseUp(mouse);
                break;
            }
            case WM_XBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->MouseUp(mouse);
                break;
            }
            case WM_NCRBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCLBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                window->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCMBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                window->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCXBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCRBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->NCMouseDown(mouse);
                break;
            }
            case WM_NCLBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                window->NCMouseDown(mouse);
                break;
            }
            case WM_NCMBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                window->NCMouseDown(mouse);
                break;
            }
            case WM_NCXBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->NCMouseDown(mouse);
                break;
            }
            case WM_NCRBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                window->NCMouseUp(mouse);
                break;
            }
            case WM_NCLBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                window->NCMouseUp(mouse);
                break;
            }
            case WM_NCMBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                window->NCMouseUp(mouse);
                break;
            }
            case WM_NCXBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                window->NCMouseUp(mouse);
                break;
            }
            case WM_NCMOUSEMOVE:{
                window->OnMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                window->NCMouseMove(mouse);
                break;
            }
            case WM_NCMOUSEHOVER:{
                window->ResetMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                window->NCMouseHover(mouse);
                break;
            }
            case WM_NCMOUSELEAVE:{
                window->ResetMouseHoverAndLeave();
                Mouse &&mouse{{},Mouse::None,0};
                window->NCMouseLeave(mouse);
                break;
            }
            #pragma endregion
            case WM_SETCURSOR:{
                Cursor cursor = window->Cursor.Get();
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
            case WM_DESTROY:{
                delete window;
                break;
            }
            default:{
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        return 0;
    }
    #endif
}