#include "window.hpp"
namespace BdUI
{
    Window::Window(std::string name,Point location,BdUI::Size size){
        name = name;
        Location = location;
        Size = size;
        WindowsList.push_back(this);
        Initializatoin();
        MessageLoop();
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
        Wndclass = WNDCLASSEX{ sizeof(WNDCLASSEX),CS_VREDRAW | CS_HREDRAW ,WndProc, 0, 0, hInstance,Icon,Cursor,BackgroundColor,MenuName->c_str(),WindowClassName.c_str(),};
        RegisterClassEx(&Wndclass);
        //OpenGL_Context = wglCreateContext(wglGetCurrentDC());
        hWnd = CreateWindowEx (dwExstyle,WindowClassName.c_str(),Name->c_str(),
                        dwstyle,Location->X,Location->Y,Size->Width,Size->Heigth,NULL,NULL,hInstance,NULL);
        Rendering();
        #endif
    }

    void Window::MessageLoop(){
        #ifdef _WIN32
        MSG msg;
        while(true){
            if(PeekMessage(&msg,hWnd,0,0,PM_REMOVE)){
                if(msg.message == WM_QUIT) break;
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else WaitMessage();
        }
        #endif
    }

    #ifdef _WIN32
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
        Window* window;
        for(Window* iter : WindowsList){
            if(iter->hWnd == hWnd){
                window = iter;
                break;
            }
        }

        switch(msg){
            case WM_SIZE:{
                window->Size = Size(LOWORD(lParam),HIWORD(lParam));
                break;
            }
            case WM_MOVE:{
                window->Location = Point(LOWORD(lParam),HIWORD(lParam));
            }
            case WM_PAINT:{
                window->Rendering();
                break;
            }
            default:{
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }
        }
        return CallNextHookEx(NULL,msg,wParam,lParam);
    }
    #endif
}