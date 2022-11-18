#include <window.hpp>

namespace BdUI {
    void Window::WindowEventDefaultBind() {
        delete Mouse.set_func;
        Mouse.set_func = nullptr;
        TitleText.set_func = new Delegate<bool(std::string, std::string*&)>(&Window::SetTitleText, this);
        BackgroundColor.set_func = new Delegate<bool(Color, Color*&)>(&Window::SetBackgroundColor, this);
        Size.set_func = new Delegate<bool(BdUI::Size, BdUI::Size*&)>(&Window::SizeChange, this);
        Location.set_func = new Delegate<bool(Point, Point*&)>(&Window::LocationChange, this);
        VSync.set_func = new Delegate<bool(bool, bool*&)>(&Window::SetVSync, this);
        Visible.set_func = new Delegate<bool(bool, bool*&)>(&Window::SetVisible, this);
        //ParentEvent
    }
    void Window::WindowCursorDefaultBind() {
#ifdef WIN32
        WindowCursor.Caption = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Close = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Size = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Zoom = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Reduce = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.Help = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        WindowCursor.SysMenu = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        
        Cursor.Border.Left = BdUI::Cursor(LoadCursor(NULL, IDC_SIZEWE));
        Cursor.Border.Right = BdUI::Cursor(LoadCursor(NULL, IDC_SIZEWE));
        Cursor.Border.Top = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENS));
        Cursor.Border.Bottom = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENS));
        Cursor.Border.BottomLeft = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENESW));
        Cursor.Border.BottomRight = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        Cursor.Border.TopLeft = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENWSE));
        Cursor.Border.TopRight = BdUI::Cursor(LoadCursor(NULL, IDC_SIZENESW));
#endif
    }
}