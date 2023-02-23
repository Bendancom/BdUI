#ifndef BDUI_PCH
#define BDUI_PCH

#include <string>
#include <typeinfo>
#include <future>
#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <map>

#ifdef _WIN32
#include <Windows.h>
#include <WinUser.h>
#include <WinBase.h>
#include <windowsx.h>
#include <wingdi.h>
#include <OpenGL/glad/glad_wgl.h>
#endif

#include <OpenGL/glad/glad.h>
#include <graph/unit.hpp>
#include <graph/point.hpp>
#include <graph/color.hpp>
#include <graph/shape.hpp>
#include <graph/size.hpp>

#include <attribute/attribute.hpp>
#include <attribute/attribute_vector.hpp>

#include <hardware/keyboard.hpp>
#include <hardware/mouse.hpp>

#include <delegate.hpp>
#include <event.hpp>
#include <character.hpp>
#include <renderer.hpp>
#include <log.hpp>
#include <readonly.hpp>

#include <math/meta.hpp>
#include <math/function.hpp>

#include <resource/resource.hpp>
#include <resource/popmenu.hpp>
#include <resource/image/image.hpp>
#include <resource/image/icon.hpp>
#include <resource/image/cursor.hpp>

namespace BdUI
{
    struct Margin{
        int Left = 0;
        int Right = 0;
        int Top = 0;
        int Bottom = 0;
    };
}
#endif