#ifndef BDUI_WINDOW
#define BDUI_WINDOW

#ifdef _WIN32
#define WM_OPENGLCONTEXT WM_USER+1
#endif

#include <pch.hpp>
#include <ui.hpp>

namespace BdUI{
    class Window : public UI{
    public:
        Window();
        ~Window();
        void Create();
        void Block();
        
        Attribute<std::string> TitleText;
        Attribute<Color> BackgroundColor;
        Attribute<Color> TransparentColor;//undo
        Attribute<Icon> SmIcon;//nudo
        Attribute<Icon> Icon;//undo
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

        AttributeVector<KeyList> HotKey;
        Event<void(KeyList)>* HotKey_Tigger = nullptr;
        Event<void(BdUI::Cursor)>* CurrentCursorChanged = nullptr;
        
#ifdef _WIN32
        Delegate<LRESULT(HWND, UINT, WPARAM, LPARAM, Window*)> DefWindowProcess = std::bind(DefWindowProc, std::placeholders::_1,
            std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
#endif
    private:
        friend class Monitor;
        std::pair<UI*, KeyList> keylist;
        Mouse mouse;
        std::promise<bool> Creation;
        std::thread *Thread;
        std::mutex Mutex;
        std::mutex OpenGLMutex;
        Renderer* Render;

        void WindowEventDefaultBind();
        void WindowCursorDefaultBind();
        
        bool SetTitleText(std::string,std::string*&);
        bool SetHotKey(std::vector<BdUI::Key>, std::vector<BdUI::Key>*&);
        bool SizeChange(BdUI::Size,BdUI::Size*&);
        bool LocationChange(Point, Point*&);
        bool SetBackgroundColor(Color, Color*&);
        bool SetVSync(bool,bool*&);
        bool SetClientSize(BdUI::Size, BdUI::Size*&);
        bool SetVisible(bool, bool*&);
        bool SetIcon(BdUI::Icon, BdUI::Icon*&);

        UI* CurrentMouseAtUI = dynamic_cast<UI*>(this);
        BdUI::Cursor CurrentCursor = Cursor.Client;
        
        void _Render();
        bool GraphChanged = true;
        #ifdef _WIN32
        HWND hWnd = nullptr;
        std::array<char,2> ANSI = {0,0};

        static std::map<HWND,Window*> WindowList;
        int dwExStyle = NULL;
        int dwStyle = WS_OVERLAPPEDWINDOW;

        static LRESULT CALLBACK __WndProc(HWND, UINT, WPARAM, LPARAM);
        static LRESULT MouseProc(HWND, UINT, WPARAM, LPARAM, Window*);
        static void MouseVitualKey(WPARAM,BdUI::Mouse&);
        void WindThread();
        #endif
    };
}
#endif