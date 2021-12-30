#include "pch.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <string>

using namespace std;

class Window{
public:
    Window(string classname,string name,int x,int y,int width,int heigth);
    ~Window();
    struct Style{
        bool Border = true;         //窗体是否有边框
        bool Caption = true;        //窗体是否有标题栏
        bool Visible = true;        //窗体是否可见
        bool ContextHelp = false;   //窗体标题栏是否有 '?' 帮助
    }Style;
    struct Attribute{
        Size Size;                  //窗体大小
        Point Point;                //窗体位置(以左上角为窗体基点)
        bool DragFile;              //窗体是否可以接受拖拽文件
    }Attribute;
private:
    #ifdef _WIN32
    HWND window;
    #endif
};