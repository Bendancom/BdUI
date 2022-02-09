#include "window.hpp"

namespace BdUI{
    Window::Window(){
        this->Style = WindowDefaultStyle;
        #ifdef _WIN32
        Size = {CW_USEDEFAULT,CW_USEDEFAULT};
        Location = {CW_USEDEFAULT,CW_USEDEFAULT};
        CaptionName = "";
        #endif
        WindowEventBind();
    }
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
        RegisterClassEx(&Windowclass);
        std::pair<int,int> style = Style;
        Point loca = Location;
        BdUI::Size size = Size;
        hWnd = CreateWindowEx(style.second,(STRING)ClassName.c_str(),(STRING)CaptionName.Get().c_str(),style.first,loca.X,loca.Y,size.Width,size.Height,NULL,NULL,hInstance,NULL);
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

    void Window::WindowEventBind(){
        CaptionName.EventList += CaptionNameChanged;
    }

    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        /*
        std::vector<HWND> hWndlist;
        HWND _hWnd = hWnd;
        while(_hWnd != NULL){
            hWndlist.push_back(_hWnd);
            _hWnd = GetParent(_hWnd);
        }
        UI *ui = WindowList[_hWnd];
        std::vector<UI*> uilist = ui->UIList.Get();
        for(int i = hWndlist.size() - 1; i >= 0;i--){
            for(std::vector<UI*>::iterator iter = uilist.begin();iter != uilist.end(); iter++){
                if((*iter)->hWnd == hWndlist[i]){
                    ui = *iter;
                    break;
                }
            }
        }*/
        UI* ui = WindowList[hWnd];
        switch(msg){
            #pragma region MouseMessage
            case WM_MOUSEMOVE:{
                ui->OnMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                ui->MouseMove(mouse);
                break;
            }
            case WM_MOUSEHOVER:{
                ui->ResetMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                ui->MouseHover(mouse);
                break;
            }
            case WM_MOUSELEAVE:{
                ui->ResetMouseHoverAndLeave();
                Mouse &&mouse{{},Mouse::None,0};
                ui->MouseLeave(mouse);
                break;
            }
            case WM_MOUSEWHEEL:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,GET_WHEEL_DELTA_WPARAM(wParam)};
                ui->MouseWheel(mouse);
                break;
            }
            case WM_RBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->MouseDoubleClick(mouse);
                break;
            }
            case WM_LBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->MouseDoubleClick(mouse);
                break;
            }
            case WM_MBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->MouseDoubleClick(mouse);
                break;
            }
            case WM_XBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->MouseDoubleClick(mouse);
                break;
            }
            case WM_RBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->MouseDown(mouse);
                break;
            }
            case WM_LBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                ui->MouseDown(mouse);
                break;
            }
            case WM_MBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                ui->MouseDown(mouse);
                break;
            }
            case WM_XBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->MouseDown(mouse);
                break;
            }
            case WM_RBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->MouseUp(mouse);
                break;
            }
            case WM_LBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                ui->MouseUp(mouse);
                break;
            }
            case WM_MBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                ui->MouseUp(mouse);
                break;
            }
            case WM_XBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->MouseUp(mouse);
                break;
            }
            case WM_NCRBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCLBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                ui->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCMBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                ui->NCMouseDoubleClick(mouse);
                break;
            }
            case WM_NCXBUTTONDBLCLK:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->NCMouseDoubleClick(mouse);
                break;
            }
            /*
            case WM_NCRBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->NCMouseDown(mouse);
                break;
            }
            case WM_NCLBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                ui->NCMouseDown(mouse);
                break;
            }
            case WM_NCMBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                ui->NCMouseDown(mouse);
                break;
            }
            case WM_NCXBUTTONDOWN:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->NCMouseDown(mouse);
                break;
            }
            case WM_NCRBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Right,0};
                ui->NCMouseUp(mouse);
                break;
            }
            case WM_NCLBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Left,0};
                ui->NCMouseUp(mouse);
                break;
            }
            case WM_NCMBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::Middle,0};
                ui->NCMouseUp(mouse);
                break;
            }
            case WM_NCXBUTTONUP:{
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},(wParam & XBUTTON1) ? Mouse::X1 : Mouse::X2,0};
                ui->NCMouseUp(mouse);
                break;
            }
            case WM_NCMOUSEMOVE:{
                ui->OnMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                ui->NCMouseMove(mouse);
                break;
            }
            case WM_NCMOUSEHOVER:{
                ui->ResetMouseHoverAndLeave();
                Mouse &&mouse{{MAKEPOINTS(lParam).x,MAKEPOINTS(lParam).y},Mouse::None,0};
                ui->NCMouseHover(mouse);
                break;
            }
            case WM_NCMOUSELEAVE:{
                ui->ResetMouseHoverAndLeave();
                Mouse &&mouse{{},Mouse::None,0};
                ui->NCMouseLeave(mouse);
                break;
            }
            #pragma endregion
            */
            case WM_SETCURSOR:{
                Cursor cursor = ui->Cursor.Get();
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
                delete ui;
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