#include "window.hpp"
namespace BdUI
{
    Window::Window(std::string name,Point location,BdUI::Size size){
        Name = name;
        Location = location;
        Size = size;
        Thread = new std::thread(&Window::Initializatoin,this);
        Thread->detach();
    }

    void Window::Initializatoin(){
        #ifdef _WIN32
        Wndclass.cbSize = sizeof(WNDCLASSEX);
        Wndclass.cbClsExtra = 0;
        Wndclass.cbWndExtra = 0;
        Wndclass.hCursor = NULL;
        Wndclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
        Wndclass.hIconSm = LoadIcon(hInstance,IDI_APPLICATION);
        Wndclass.hIcon = LoadIcon(hInstance,IDI_APPLICATION);
        Wndclass.lpszMenuName = (STRING)"Menu";
        Wndclass.lpszClassName = (STRING)ClassName.c_str();
        Wndclass.lpfnWndProc = WndProc;
        Wndclass.style = CS_VREDRAW|CS_HREDRAW;
        Wndclass.hInstance = hInstance;
        RegisterClassEx(&Wndclass);
        hWnd = CreateWindowEx (dwExstyle,(STRING)ClassName.c_str(),(STRING)Name->c_str(),dwstyle,Location->X,Location->Y,Size->Width,Size->Heigth,NULL,NULL,hInstance,NULL);
        ShowWindow(hWnd,SW_SHOW);
        UpdateWindow(hWnd);
        WindowList[hWnd] = this;
        #endif

        #ifdef _WIN32
        Cursor = {LoadCursor(NULL,IDC_ARROW),
                LoadCursor(NULL,IDC_ARROW),
                LoadCursor(NULL,IDC_SIZEWE),
                LoadCursor(NULL,IDC_SIZEWE),
                LoadCursor(NULL,IDC_SIZENS),
                LoadCursor(NULL,IDC_SIZENS)};

        #endif
        MessageLoop();
    }

    void Window::MessageLoop(){
        #ifdef _WIN32
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        this->~Window();
        #endif
    }

    Window::~Window(){
        delete Thread;
    }

    #ifdef _WIN32
    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        Window *window = WindowList[hWnd];
        switch(msg){
            /*
            case WM_SIZE:{
                break;
            }
            case WM_MOVE:{
                break;
            }
            case WM_PAINT:{
                break;
            }
            */
            case WM_SETCURSOR:{
                switch(LOWORD(lParam)){
                    case HTCLIENT:{
                        SetCursor(window->Cursor->ClientCursor);
                        break;
                    }
                    case HTCAPTION:{
                        SetCursor(window->Cursor->CaptionCursor);
                        break;
                    }
                    case HTLEFT:{
                        SetCursor(window->Cursor->LeftBorderCursor);
                        break;
                    }
                    case HTRIGHT:{
                        SetCursor(window->Cursor->RightBorderCursor);
                        break;
                    }
                    case HTTOP:{
                        SetCursor(window->Cursor->TopBorderCursor);
                        break;
                    }
                    case HTBOTTOM:{
                        SetCursor(window->Cursor->BottomBorderCursor);
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