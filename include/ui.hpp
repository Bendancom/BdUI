#include "pch.hpp"

#ifndef BDUI_UI
#define BDUI_UI
namespace BdUI
{
    class UI{
    public:
        #ifdef _WIN32
        UI() : Style(UIStyle,&AttributeGetStyle,[](BdUI::Style &s,const BdUI::Style &cs){ s = cs; return true; }),
        Size({100,20}),BackgroundColor(UIclass.hbrBackground) {}
        #endif
        bool Create();
        bool Show();
        ~UI();
        Attribute<Cursor> Cursor;
        Attribute<Size> Size;
        Attribute<Point> Location;
        #ifdef _WIN32
        Attribute<BdUI::Style,std::pair<int,int>,BdUI::Style> Style;
        Attribute<HMENU> PopMenu;
        Attribute<HBRUSH&> BackgroundColor;
        HWND hWnd;
        HWND ParenthWnd;
        #endif
    private:
        #ifdef _WIN32
        const HINSTANCE hInstance = GetModuleHandle(0);
        WNDCLASSEX UIclass = {
            sizeof(WNDCLASSEX),
            CS_VREDRAW|CS_HREDRAW,
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

        void Initialization();
        #endif
    };
}
#endif