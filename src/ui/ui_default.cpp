#include <ui.hpp>

namespace BdUI {
    void UI::UICursorDefaultBind() {
#ifdef WIN32
        Cursor.Client = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Menu = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.Left = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.Right = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.Top = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.Bottom = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.BottomLeft = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.BottomRight = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.TopLeft = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
        Cursor.Border.TopRight = BdUI::Cursor(LoadCursor(NULL, IDC_ARROW));
#endif
    }
    void UI::UIEventDefaultBind() {
        Mouse.set_func = new Delegate<bool(BdUI::Mouse, BdUI::Mouse*&)>(&UI::MouseRelativePos, this);
        Parent.set_func = new Delegate<bool(UI*, UI*&)>(&UI::ParentSet, this);
    }
}