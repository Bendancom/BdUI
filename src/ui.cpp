#include "ui.hpp"
namespace BdUI
{
    bool UI::Create(){
        #ifdef _WIN32
        RegisterClassEx(&UIclass);
        std::pair<int,int> style = Style;
        Point loca = Location.Get();
        BdUI::Size size = Size.Get(); 
        hWnd = CreateWindowEx (style.second,(STRING)ClassName.c_str(),NULL,style.first,loca.X,loca.Y,size.Width,size.Height,Parent.Get()->hWnd,NULL,hInstance,NULL);
        if (hWnd == NULL){
            return false;
        }
        else return true;
        #endif
    }

    bool UI::Show(){
        #ifdef _WIN32
        if (ShowWindow(hWnd,SW_SHOW)) return false;
        if (UpdateWindow(hWnd)) return false;
        #endif
        return true;
    }

    void UI::UIEventBind(){
        Size.EventList += SizeChanged;
        Location.EventList += LocationChanged;
        Cursor.EventList += CursorChanged;
    }

    #ifdef _WIN32
    void UI::OnMouseHoverAndLeave(){
        if(!IsOnMouseHoverAndLeave){
            TRACKMOUSEEVENT tme{sizeof(TRACKMOUSEEVENT),TME_HOVER|TME_LEAVE,hWnd,static_cast<DWORD>(MouseHoverTime)};
            TrackMouseEvent(&tme);
            IsOnMouseHoverAndLeave = true;
        }
    }
    void UI::ResetMouseHoverAndLeave(){
        if(IsOnMouseHoverAndLeave){
            TRACKMOUSEEVENT tme{sizeof(TRACKMOUSEEVENT),TME_HOVER|TME_LEAVE|TME_CANCEL,hWnd,static_cast<DWORD>(MouseHoverTime)};
            TrackMouseEvent(&tme);
            IsOnMouseHoverAndLeave = false;
        }
    }
    #endif
}