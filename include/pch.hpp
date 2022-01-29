#include <string>
#include <typeinfo>
//#include <glad/glad.h>
#ifdef _WIN32
#include <windows.h>
//#include <glad/glad_wgl.h>
#endif

#include "attribute.hpp"
#include "delegate.hpp"
#include "event.hpp"

#ifdef UNICODE
#define STRING LPCWSTR
#else
#define STRING LPCSTR
#endif

#ifndef BDUI_PCH
#define BDUI_PCH
namespace BdUI
{
    struct Point{
        Point(long x = 0,long y = 0) : X(x),Y(y) {}
        long X;
        long Y;
    };
    struct Size{
        Size(long w = 0,long h = 0) : Width(w),Heigth(h) {}
        long Width;
        long Heigth;
    };
}
#endif