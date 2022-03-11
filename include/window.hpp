#ifndef BDUI_WINDOW
#define BDUI_WINDOW
#include "pch.hpp"
#include "ui.hpp"

namespace BdUI{
    class Window : public UI{
    public:
        Window();
        ~Window();
        bool Create();
        void Block();
        
        Attribute<BdUI::Cursor> CaptionCursor;
        Attribute<BdUI::Cursor> BorderCursor_Left;
        Attribute<BdUI::Cursor> BorderCursor_Right;
        Attribute<BdUI::Cursor> BorderCursor_Top;
        Attribute<BdUI::Cursor> BorderCursor_Bottom;
        Attribute<BdUI::Cursor> BottomLeftCursor;
        Attribute<BdUI::Cursor> BottomRightCursor;
        Attribute<BdUI::Cursor> TopLeftCursor;
        Attribute<BdUI::Cursor> TopRightCursor;
        Attribute<BdUI::Cursor> CloseCursor;
        Attribute<BdUI::Cursor> SizeCursor;
        Attribute<BdUI::Cursor> ZoomCursor;
        Attribute<BdUI::Cursor> ReduceCursor;
        Attribute<BdUI::Cursor> HelpCursor;
        Attribute<BdUI::Cursor> MenuCursor;
    private:
        std::thread *Thread;
        std::mutex Mutex;
        std::promise<bool>  IsCreation;
        void VisibleEvent(bool);
        void WindowDefaultEventBind();
        void CursorDefaultSet();
        
        #ifdef _WIN32
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        void WindThread();
        #endif
    };
}
#endif