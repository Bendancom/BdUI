#include "glad/glad.h"
#ifdef _WIN32
#include "glad/glad_wgl.h"
#endif
#ifdef LINUX
#include "glad/glad_glx.h"
#endif

struct Point{
    long x;
    long y;
};

struct Size{
    long Width;
    long Heigth;
};