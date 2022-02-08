#ifndef BDUI_WINDOW
#define BDUI_WINDOW
#include "pch.hpp"
#include "ui.hpp"

namespace BdUI
{
    class Window{
    public:
        #ifdef _WIN32
        Window() : Style(&AttributeGetStyle,[](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),
        Size({CW_USEDEFAULT,CW_USEDEFAULT}),Location({CW_USEDEFAULT,CW_USEDEFAULT}),CaptionName("") {}
        #endif
        ~Window();
        bool Create();
        bool Show();
        void Block() { mutex.lock(); }

        #ifdef _WIN32
        void OnMouseHoverAndLeave();    //Automatic
        void ResetMouseHoverAndLeave(); //Automatic
        #endif

        Attribute<std::string> CaptionName;
        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        Attribute<int> MouseHoverTime;
        #ifdef _WIN32
        Attribute<BdUI::Style,std::pair<int,int>,BdUI::Style> Style;
        Attribute<HICON> Icon;
        Attribute<HICON> IconSm;
        Attribute<HMENU> PopMenu;

        HWND hWnd;
        #endif
        std::vector<UI*> UIList;
        
        Event<void(Mouse)> MouseWheel;

        Event<void(Mouse)> MouseDown;
        Event<void(Mouse)> MouseUp;
        Event<void(Mouse)> MouseDoubleClick;
        Event<void(Mouse)> MouseHover;
        Event<void(Mouse)> MouseMove;
        Event<void(Mouse)> MouseLeave;

        Event<void(Mouse)> NCMouseDown;
        Event<void(Mouse)> NCMouseUp;
        Event<void(Mouse)> NCMouseDoubleClick;
        Event<void(Mouse)> NCMouseHover;
        Event<void(Mouse)> NCMouseMove;
        Event<void(Mouse)> NCMouseLeave;

        Event<void(BdUI::Size)> SizeChanged;
        Event<void(Point)> LocationChanged;
        Event<void(BdUI::Cursor)> CursorChanged;
        Event<void(std::string)> CaptionNameChanged;

        void Rendering();
    private:
        std::promise<bool> IsCreate;
        #ifdef _WIN32
        const HINSTANCE hInstance = GetModuleHandle(0);
        WNDCLASSEX Wndclass = {
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,
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
        Attribute<bool> IsOnMouseHoverAndLeave;
        std::mutex mutex;
        #endif

        void Initialization();
        void EventBind();
    };
    #ifdef _WIN32
    static std::map<HWND,BdUI::Window*> WindowList;
    #endif
}
#endif