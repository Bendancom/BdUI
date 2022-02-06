#include "pch.hpp"

namespace BdUI{
    #ifdef _WIN32
    std::pair<int,int> AttributeGetStyle(const Style& style){
        //Windows Style
        int dw_style = 0;
        if (style.Border) dw_style |= WS_BORDER;
        if (style.Caption) dw_style |= WS_CAPTION;
        if (style.Disable) dw_style |= WS_DISABLED;
        if (style.HScroll) dw_style |= WS_HSCROLL;
        if (style.VScroll) dw_style |= WS_VSCROLL;
        if (style.MaxSizeInit) dw_style |= WS_MAXIMIZE;
        if (style.MinSizeInit) dw_style |= WS_MINIMIZE;
        if (style.MaxSize) dw_style |= WS_MAXIMIZEBOX;
        if (style.MinSize) dw_style |= WS_MINIMIZEBOX;
        if (style.SysMenu) dw_style |= WS_SYSMENU;
        if (style.Tabs_Top) dw_style |= WS_TABSTOP;
        if (style.Visible) dw_style |= WS_VISIBLE;
        if (style.Size) dw_style |= WS_SIZEBOX;
        if (style.ClipChildren) dw_style |= WS_CLIPCHILDREN;

        //Windows ExStyle
        int dw_Exstyle = 0;
        if (style.AcceptFiles) dw_Exstyle |= WS_EX_ACCEPTFILES;
        if (style.ContextHelp) dw_Exstyle |= WS_EX_CONTEXTHELP;

        switch(style.Type){
            case Style::Window:{
                break;
            }
            case Style::PopUp:{
                dw_style |= WS_POPUP;
                break;
            }
            case Style::Child:{
                dw_style |= WS_CHILD;
                break;
            }
            case Style::LayerdWindow:{
                dw_Exstyle |= WS_EX_LAYERED;
                break;
            }
        }
        return std::make_pair(dw_style,dw_Exstyle);
    }
    #endif
}