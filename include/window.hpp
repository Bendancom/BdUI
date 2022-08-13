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
        void Paint(const BdUI::Size&) = delete;
        
        Attribute<Color> Background;
        Attribute<Icon> SmIcon;
        Attribute<Icon> Icon;
        Attribute<UI*> Focus;
        Attribute<Size> Size;
        Attribute<bool> VSync;

        struct {
            Attribute<BdUI::Cursor> Caption;
            Attribute<BdUI::Cursor> Close;
            Attribute<BdUI::Cursor> Size;
            Attribute<BdUI::Cursor> Zoom;
            Attribute<BdUI::Cursor> Reduce;
            Attribute<BdUI::Cursor> Help;
            Attribute<BdUI::Cursor> SysMenu;
        }WindowCursor;

        
    private:
        std::thread *Thread;
        std::mutex Mutex;
        static bool IsLoadOpenGL;

        void WindowEventDefaultBind();
        void WindowCursorDefaultBind();

        void Paint();
        
        bool WindowSizeChange(BdUI::Size,BdUI::Size*&);
        bool WindowSetText(const std::string*&);
        bool WindowLocationChange(Point, Point*&);
        bool WindowSetBackground(Color, Color*&);
        bool WindowSetVSync(bool,bool*&);
        bool WindowSetClientSize(BdUI::Size, BdUI::Size*&);
        bool WindowSetVisible(bool, bool*&);

        UI* MouseContext = this;
        BdUI::Cursor CurrentCursor = Cursor.Client;
        

        bool GraphChanged = true;
        #ifdef _WIN32
        static std::map<HWND,Window*> WindowList;
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;
        HWND hWnd = nullptr;
        HDC hDC = nullptr;
        HGLRC hRC = nullptr;
        std::promise<bool> Creation;
        static LRESULT CALLBACK __WndProc(HWND,UINT,WPARAM,LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        static void MouseVitualKey(WPARAM,BdUI::Mouse&);
        void WindThread();
        #endif
    };
}
#endif