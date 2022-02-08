#ifndef BDUI_UI
#define BDUI_UI
#include "pch.hpp"
#include "window.hpp"

namespace BdUI
{
    class UI : public Window{
    public:
        UI(Window* parent) : Style(UIStyle,&AttributeGetStyle,[](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),
        ParentWindow(parent){
            parent->UIList.push_back(this);
        }
        UI(UI* parent) : Style(UIStyle,&AttributeGetStyle,[](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),
        ParentWindow(parent) {
            parent->UIList.push_back(this);
        }
        ~UI();
        bool Create();
        bool Show();
        Window* ParentWindow = nullptr;
        UI* ParentUI = nullptr;

        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        Attribute<int> MouseHoverTime;
        #ifdef _WIN32
        Attribute<BdUI::Style,std::pair<int,int>,BdUI::Style> Style;
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

    private:
        #ifdef _WIN32
        const HINSTANCE hInstance = GetModuleHandle(0);
        WNDCLASSEX UIclass = {
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW|CS_DBLCLKS,
            WndProc,
            0,
            0,
            hInstance,
            NULL,
            NULL,
            NULL,
            (STRING)"Menu",
            (STRING)ClassName.c_str(),
            NULL,
        };
        BdUI::Style UIStyle = {
            Style::Child,
            true,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            true,
            false,
            true,
            false,
            false,
        };
        #endif
    };
}
#endif