#ifndef BDUI_UI
#define BDUI_UI
#include "pch.hpp"

namespace BdUI
{
    class UI{
    public:
        UI(UI* parent = nullptr) : Style(UIDefaultStyle,&AttributeGetStyle,
        [](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),Size({100,20}),Parent(parent) {
            UIEventBind();
            #ifdef _WIN32
            MouseHoverTime = HOVER_DEFAULT;
            #endif
        }
        virtual ~UI() {}
        virtual bool Create();
        bool Show();
        Attribute<std::vector<UI*>> UIList;

        #ifdef _WIN32
        void OnMouseHoverAndLeave();    //Automatic
        void ResetMouseHoverAndLeave(); //Automatic
        #endif
        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        Attribute<int> MouseHoverTime;
        #ifdef _WIN32
        Attribute<BdUI::Style,std::pair<int,int>,BdUI::Style> Style;
        Attribute<HMENU> PopMenu;
        HWND hWnd;
        #endif
        Attribute<UI*> Parent;
        
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

        void Rendering();
    private:
        #ifdef _WIN32
        Attribute<bool> IsOnMouseHoverAndLeave;
        const HINSTANCE hInstance = GetModuleHandle(NULL);
        const WNDCLASSEX UIclass = {
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
        #endif
        void UIEventBind();
    };
}
#endif