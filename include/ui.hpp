#ifndef BDUI_UI
#define BDUI_UI
#include "pch.hpp"

namespace BdUI
{
    class UI{
    public:
        UI();
        virtual ~UI() {}
        std::vector<UI*> UIList;

        #ifdef _WIN32
        #endif
        Attribute<Margin> Margin;
        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        Attribute<int> MouseHoverTime;
        Attribute<bool> Visible;
        Attribute<HMENU> PopMenu;
        ReadOnly<UI*> Parent;
        
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
        Event<void(bool)> VisibleChanged;
        void Rendering();
    private:
        void UIEventBind();
    };
}
#endif