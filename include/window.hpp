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
        
        Attribute<Icon> SmIcon;
        Attribute<Icon> Icon;

        Attribute<Cursor> CaptionCursor;
        Attribute<Cursor> BorderCursor_Left;
        Attribute<Cursor> BorderCursor_Right;
        Attribute<Cursor> BorderCursor_Top;
        Attribute<Cursor> BorderCursor_Bottom;
        Attribute<Cursor> BottomLeftCursor;
        Attribute<Cursor> BottomRightCursor;
        Attribute<Cursor> TopLeftCursor;
        Attribute<Cursor> TopRightCursor;
        Attribute<Cursor> CloseCursor;
        Attribute<Cursor> SizeCursor;
        Attribute<Cursor> ZoomCursor;
        Attribute<Cursor> ReduceCursor;
        Attribute<Cursor> HelpCursor;
        Attribute<Cursor> MenuCursor;
        Attribute<Cursor> ClientCursor;
    private:
        std::thread *Thread;
        std::mutex Mutex;
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