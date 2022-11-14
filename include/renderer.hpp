#ifndef BDUI_RENDER
#define BDUI_RENDER

#ifdef _WIN32
#include <Windows.h>
#include <OpenGL/glad/glad_wgl.h>
#endif
#include <ui.hpp>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <queue>
#include <functional>
#include <error.hpp>
#include <OpenGL/glad/glad.h>

namespace BdUI {
    class Window;
	class Renderer {
	public:
        Renderer(Window* parent) : Parent(parent) {}
        ~Renderer();

        template<typename gl_Func,typename... Args>
        void PushMessage(gl_Func gl_func, Args... args) {
            Mutex.lock();
            if (IsLoadOpenGL) {
                RendeMessage_Queue.push(std::bind(gl_func, args...));
                condition.notify_all();
            }
            Mutex.unlock();
        }
        void Render();

#ifdef _WIN32
        void Initialize(HWND hWnd);
#endif
	private:
        Window* Parent;
        static bool IsLoadOpenGL;
        std::promise<bool> Initialization;
        std::mutex Mutex;
        std::condition_variable condition;
        std::queue<std::function<void(void)>> RendeMessage_Queue;
        std::thread* Renderer_Thread = nullptr;

        void RenderMessageLoop();
        void _Render();


#ifdef _WIN32
        HWND hWnd;
		HDC hDC;
        HGLRC hRC;

        PIXELFORMATDESCRIPTOR pfd =
        {
            sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
            32,                        //Colordepth of the framebuffer.
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            24,                        //Number of bits for the depthbuffer
            8,                        //Number of bits for the stencilbuffer
            0,                        //Number of Aux buffers in the framebuffer.
            PFD_MAIN_PLANE,
            0,
            0, 0, 0
        };
#endif
	};
}

#endif