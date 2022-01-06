#include "pch.h"
#ifdef _WIN32
#include <windows.h>
#endif

class Window{
public:
    Window(string,string,Point,Size);
    ~Window();

    Attribute<string> ClassName = NULL;    //窗体类名
    Attribute<string> Name = NULL;         //窗体名
    Attribute<bool> Border = true;         //窗体是否有边框
    Attribute<bool> Caption = true;        //窗体是否有标题栏
    Attribute<bool> Visible = true;        //窗体是否可见
    Attribute<bool> ContextHelp = false;   //窗体标题栏是否有 '?' 帮助
    Attribute<Size> Size;                  //窗体大小
    Attribute<Point> Location;             //窗体位置(以左上角为窗体基点)
    Attribute<bool> DragFile = false;      //窗体是否可以接受拖拽文件    
    #ifdef _WIN32
    HWND hWnd;
    #endif

    void Rendering();
private:
    #ifdef _WIN32
    HGLRC OpenGL_Context;
    int dwstyle = 0;
    int dwExstyle = WS_EX_LAYERED;
    #endif
    void MessageLoop();
    void Initializatoin();
};

#ifdef _WIN32
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
vector<Window*> WindowsList;
#endif