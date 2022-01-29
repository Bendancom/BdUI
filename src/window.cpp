#include "window.hpp"
namespace BdUI
{
    Window::Window(std::string name,Point location,BdUI::Size size){
        name = name;
        Location = location;
        Size = size;
        Thread = new std::thread(&Window::Initializatoin,this);
        Thread->detach();
    }

    void Window::Rendering(){
        int dwstyle = 0;
        int dwExstyle = WS_EX_LAYERED;

        if (Border) dwstyle = dwstyle|WS_BORDER;
        if (Caption) dwstyle = dwstyle|WS_CAPTION;
        if (ContextHelp) dwExstyle = dwExstyle|WS_EX_CONTEXTHELP;
        if (Visible) dwstyle = dwstyle|WS_VISIBLE;
        if (DragFile) dwExstyle =dwExstyle|WS_EX_ACCEPTFILES;

        if (dwstyle != this->dwstyle){
            this->dwstyle = dwstyle;
            SetWindowLong(hWnd,GWL_STYLE,dwstyle);
        }
        if(dwExstyle != this->dwExstyle){
            this->dwExstyle = dwExstyle;
            SetWindowLong(hWnd,GWL_EXSTYLE,dwExstyle);
        }
        //glViewport(0,0,Size->Width,Size->Heigth);
    }

    void Window::Initializatoin(){
        #ifdef _WIN32
        Wndclass.cbSize = sizeof(WNDCLASSEX);
        Wndclass.cbClsExtra = 0;
        Wndclass.cbWndExtra = 0;
        Wndclass.hCursor = LoadCursor(hInstance,IDC_ARROW);
        Wndclass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
        Wndclass.hIconSm = LoadIcon(hInstance,IDI_APPLICATION);
        Wndclass.hIcon = LoadIcon(hInstance,IDI_APPLICATION);
        Wndclass.lpszMenuName = (STRING)"Menu";
        Wndclass.lpszClassName = (STRING)ClassName.c_str();
        Wndclass.lpfnWndProc = WndProc;
        Wndclass.style = CS_VREDRAW|CS_HREDRAW;
        Wndclass.hInstance = hInstance;
        RegisterClassEx(&Wndclass);
        //OpenGL_Context = wglCreateContext(wglGetCurrentDC());
        hWnd = CreateWindowEx (dwExstyle,(STRING)ClassName.c_str(),(STRING)Name->c_str(),dwstyle,Location->X,Location->Y,Size->Width,Size->Heigth,NULL,NULL,hInstance,NULL);
        ShowWindow(hWnd,SW_SHOW);
        UpdateWindow(hWnd);
        WindowList.insert(std::make_pair(hWnd,this));
        MessageLoop();
        //Rendering();
        #endif
    }

    void Window::MessageLoop(){
        #ifdef _WIN32
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
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
            case WM_DESTROY:{
                DestroyWindow(hWnd);
                window->~Window();
                break;
            }
            default:{
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
    }
    #endif
}