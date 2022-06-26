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
        //Attribute<Cursor> Cursor;
        Attribute<BdUI::Margin> Margin;
        Attribute<BdUI::Margin> Border;
        Attribute<BdUI::Size> Size;
        Attribute<Point> Location;
        Attribute<bool> Visible;
        Attribute<Mouse> Mouse;
        Attribute<HMENU> PopMenu;
        Attribute<unsigned int> MouseHoverTime;

        Attribute<Cursor> ClientCursor;
        Attribute<Cursor> MenuCursor;
        Attribute<Cursor> BorderCursor_Left;
        Attribute<Cursor> BorderCursor_Right;
        Attribute<Cursor> BorderCursor_Top;
        Attribute<Cursor> BorderCursor_Bottom;
        Attribute<Cursor> BottomLeftCursor;
        Attribute<Cursor> BottomRightCursor;
        Attribute<Cursor> TopLeftCursor;
        Attribute<Cursor> TopRightCursor;

        ReadOnly<UI*> Parent;
        static UI* SearchUI_NearPos(const Point&,UI*);
        static Cursor Search_Area_Cursor(const Point&,UI*);
    private:
        void UICursorDefaultBind();
    };
}
#endif