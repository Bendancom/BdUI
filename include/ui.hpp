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
        Attribute<Cursor> Cursor;
        Attribute<Margin> Margin;
        Attribute<BdUI::Size> Size;
        Attribute<Point> Location;
        Attribute<int> MouseHoverTime;
        Attribute<bool> Visible;
        Attribute<HMENU> PopMenu;
        ReadOnly<UI*> Parent;
        
        
        EventArray<void(Mouse)> MouseWheel;

        EventArray<void(Mouse)> MouseDown;
        EventArray<void(Mouse)> MouseUp;
        EventArray<void(Mouse)> MouseDoubleClick;
        EventArray<void(Mouse)> MouseHover;
        EventArray<void(Mouse)> MouseMove;
        EventArray<void(Mouse)> MouseLeave;

        EventArray<void(Mouse)> NCMouseDown;
        EventArray<void(Mouse)> NCMouseUp;
        EventArray<void(Mouse)> NCMouseDoubleClick;
        EventArray<void(Mouse)> NCMouseHover;
        EventArray<void(Mouse)> NCMouseMove;
        EventArray<void(Mouse)> NCMouseLeave;
    private:
        void CursorDefaultSet();
    };
}
#endif