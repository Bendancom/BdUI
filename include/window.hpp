#include "pch.hpp"
#include "ui.hpp"

#ifndef BDUI_WINDOW
#define BDUI_WINDOW
namespace BdUI
{
    class Window{
    public:
        #ifdef _WIN32
        Window() : Style(&AttributeGetStyle,[](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),
        Size({CW_USEDEFAULT,CW_USEDEFAULT}),Location({CW_USEDEFAULT,CW_USEDEFAULT}),CaptionName(""),
        BackgroundColor(Wndclass.hbrBackground),Icon(Wndclass.hIcon),IconSm(Wndclass.hIconSm) {}
        #endif
        ~Window() { mutex.unlock();delete Thread; }
        bool Create();
        bool Show();
        void Block() { mutex.lock(); }

        Attribute<std::string> CaptionName;
        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        #ifdef _WIN32
        Attribute<BdUI::Style,std::pair<int,int>,BdUI::Style> Style;
        Attribute<HICON&> Icon;
        Attribute<HICON&> IconSm;
        Attribute<HMENU> PopMenu;
        Attribute<HBRUSH&> BackgroundColor;
        HWND hWnd;
        #endif
        
        void Rendering();
    private:
        #ifdef _WIN32
        const HINSTANCE hInstance = GetModuleHandle(0);
        WNDCLASSEX Wndclass = {
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW,
            WndProc,
            0,
            0,
            hInstance,
            LoadIcon(hInstance,IDI_APPLICATION),
            NULL,
            NULL,
            (STRING)"Menu",
            (STRING)ClassName.c_str(),
            LoadIcon(hInstance,IDI_APPLICATION),
        };
        std::thread *Thread;
        HGLRC OpenGL_Context = nullptr;
        Attribute<int> dwstyle;
        Attribute<int> dwExstyle;
        std::promise<HWND> _hWnd;
        std::mutex mutex;
        #endif
        void MessageLoop();
        void Initialization();
    };

    #ifdef _WIN32
    static std::map<HWND,BdUI::Window*> WindowList;
    #endif
}
#endif