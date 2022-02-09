#ifndef BDUI_WINDOW
#define BDUI_WINDOW
#include "pch.hpp"
#include "ui.hpp"

namespace BdUI{
    class Window : public UI{
    public:
        Window();
        ~Window();
        virtual bool Create();
        void Block() { mutex.lock(); }

        Attribute<std::string> CaptionName;
        #ifdef _WIN32
        Attribute<HICON> Icon;
        Attribute<HICON> IconSm;
        #endif

        Event<void(std::string)> CaptionNameChanged;
    private:
        const HINSTANCE hInstance = GetModuleHandle(NULL);
        const WNDCLASSEX Windowclass{
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,
            WndProc,
            0,
            0,
            hInstance,
            NULL,
            NULL,
            NULL,
            NULL,
            (STRING)ClassName.c_str(),
            NULL,
        };
        std::promise<bool> IsCreate;
        #ifdef _WIN32
        std::thread *Thread;
        std::mutex mutex;
        #endif
        void WindowEventBind();
        void Initialization();
    };
    #ifdef _WIN32
    static std::map<HWND,BdUI::Window*> WindowList;
    #endif
}
#endif