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
        Attribute<Point> Location;
        Attribute<bool> Visible;
        Attribute<Mouse> Mouse;
        Attribute<Key> Key;

        Attribute<HMENU> PopMenu;
        Attribute<unsigned int> MouseHoverTime;

        struct {
            Attribute<Cursor> Client;
            Attribute<Cursor> Menu;
            struct {
                Attribute<Cursor> Left;
                Attribute<Cursor> Right;
                Attribute<Cursor> Top;
                Attribute<Cursor> Bottom;
                Attribute<Cursor> BottomLeft;
                Attribute<Cursor> BottomRight;
                Attribute<Cursor> TopLeft;
                Attribute<Cursor> TopRight;
            }Border;
        }Cursor;
        
        ReadOnly<UI*> Parent;
        static UI* SearchUI_NearPos(const Point&,UI*);
        static const BdUI::Cursor* Search_Area_Cursor(const Point&,UI*);
    private:
        void UICursorDefaultBind();
        void UIEventDefaultBind();
        bool MouseRelativePos(const Point*,BdUI::Mouse,BdUI::Mouse&);
    };
}
#endif