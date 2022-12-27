#ifndef BDUI_UI
#define BDUI_UI
#include "pch.hpp"

namespace BdUI
{
    class UI{
    public:
        UI();
        ~UI() {}

        AttributeVector<UI*> UIList;

        void Paint(const Size&);

        Attribute<UI*> Parent;

        Attribute<BdUI::Margin> Margin;
        Attribute<Shape> Shape;
        Attribute<Point> Location;
        Attribute<bool> Visible;
        Attribute<Key> Key;
        Attribute<Mouse> Mouse;

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

        Event<void(BdUI::Mouse)>* MouseEvent = nullptr;
        
        static UI* SearchUI_NearPos(const Point&,UI*);
        static BdUI::Cursor Search_Area_Cursor(const Point&,UI*);
    private:
        void UICursorDefaultBind();
        void UIEventDefaultBind();

        bool MouseRelativePos(BdUI::Mouse,BdUI::Mouse*&);
        bool ParentSet(UI*, UI*&);
    };
}
#endif