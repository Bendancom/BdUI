#include "pch.h"

class UI_Object{
public:
    UI_Object();
    ~UI_Object();

    struct Style{
        bool Visible;           //控件是否可见
    }Style;
    struct Attribute{
        Point Point;
        Size Size;
    }Attribute;
private:

};
