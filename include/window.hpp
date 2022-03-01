#ifndef BDUI_WINDOW
#define BDUI_WINDOW
#include "pch.hpp"
#include "ui.hpp"

namespace BdUI{
    class Window : public UI{
    public:
        Window();
        ~Window();
        bool Create();
        void Block();
        

    private:
        std::thread *Thread;
        std::mutex Mutex;
        std::promise<bool>  IsCreation;
        void VisibleEvent(bool);
        void WindowDefaultEventBind();
        
        #ifdef _WIN32
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        void WindThread();
        #endif
    };
    
}
#endif