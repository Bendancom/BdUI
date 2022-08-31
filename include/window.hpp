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
        
        Attribute<Color> BackgroundColor;
        Attribute<Color> TransparentColor;
        Attribute<Icon> SmIcon;
        Attribute<Icon> Icon;
        Attribute<UI*> Focus;
        Attribute<Size> Size;
        Attribute<bool> VSync;
        Attribute<std::list<BdUI::Key>> HotKey;

        struct {
            Attribute<BdUI::Cursor> Caption;
            Attribute<BdUI::Cursor> Close;
            Attribute<BdUI::Cursor> Size;
            Attribute<BdUI::Cursor> Zoom;
            Attribute<BdUI::Cursor> Reduce;
            Attribute<BdUI::Cursor> Help;
            Attribute<BdUI::Cursor> SysMenu;
        }WindowCursor;

        EventArray<BdUI::Key>* HotKey_Tigger;
        
    private:
        std::thread *Thread;
        std::mutex Mutex;
        static bool IsLoadOpenGL;

        void WindowEventDefaultBind();
        void WindowCursorDefaultBind();

        void Paint();
        
        bool SetHotKey(std::list<BdUI::Key>, std::list<BdUI::Key>*&);
        bool SizeChange(BdUI::Size,BdUI::Size*&);
        bool LocationChange(Point, Point*&);
        bool SetBackgroundColor(Color, Color*&);
        bool SetVSync(bool,bool*&);
        bool SetClientSize(BdUI::Size, BdUI::Size*&);
        bool SetVisible(bool, bool*&);

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