#include "window.hpp"
namespace BdUI
{
    bool Window::Create(){
        Thread = new std::thread(&Window::Initialization,this);
        Thread->detach();
        #ifdef _WIN32
        if (_hWnd.get_future().get()) return true;
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
        _hWnd.set_value(hWnd);
        if (hWnd == 0) return;
        WindowList[hWnd] = this;
        #endif
        MessageLoop();
    }

    bool Window::Show(){
        if (ShowWindow(hWnd,SW_SHOW)) return false;
        if (UpdateWindow(hWnd)) return false;
        return true;
    }

    void Window::MessageLoop(){
        #ifdef _WIN32
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

    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        Window *window = WindowList[hWnd];
        switch(msg){
            case WM_RBUTTONDBLCLK:{
                TrackPopupMenuEx(window->PopMenu,TPM_LEFTALIGN | TPM_TOPALIGN,LOWORD(lParam),HIWORD(lParam),window->hWnd,nullptr);
                break;
            }
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
                DestroyWindow(hWnd);
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