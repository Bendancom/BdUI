#include <window.hpp>

namespace BdUI {
    void Window::WindowEventDefaultBind() {
        TitleText.set_func = new Delegate<bool(std::string, std::string*&)>(&Window::SetTitleText, this);
        BackgroundColor.set_func = new Delegate<bool(Color, Color*&)>(&Window::SetBackgroundColor, this);
        Size.set_func = new Delegate<bool(BdUI::Size, BdUI::Size*&)>(&Window::SizeChange, this);
        Location.set_func = new Delegate<bool(Point, Point*&)>(&Window::LocationChange, this);
        VSync.set_func = new Delegate<bool(bool, bool*&)>(&Window::SetVSync, this);
        Visible.set_func = new Delegate<bool(bool, bool*&)>(&Window::SetVisible, this);
        Icon.set_func = new Delegate<bool(BdUI::Icon, BdUI::Icon*&)>(&Window::SetIcon, this);
        //ParentEvent
    }
    void Window::WindowCursorDefaultBind() {
#ifdef WIN32
        WindowCursor.Caption = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.Close = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.Size = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.Zoom = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.Reduce = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.Help = LoadCursor(NULL, IDC_ARROW);
        WindowCursor.SysMenu = LoadCursor(NULL, IDC_ARROW);
        
        Cursor.Border.Left = LoadCursor(NULL, IDC_SIZEWE);
        Cursor.Border.Right = LoadCursor(NULL, IDC_SIZEWE);
        Cursor.Border.Top = LoadCursor(NULL, IDC_SIZENS);
        Cursor.Border.Bottom = LoadCursor(NULL, IDC_SIZENS);
        Cursor.Border.BottomLeft = LoadCursor(NULL, IDC_SIZENESW);
        Cursor.Border.BottomRight = LoadCursor(NULL, IDC_SIZENWSE);
        Cursor.Border.TopLeft = LoadCursor(NULL, IDC_SIZENWSE);
        Cursor.Border.TopRight = LoadCursor(NULL, IDC_SIZENESW);
#endif
    }
}