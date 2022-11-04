#include <window.hpp>

namespace BdUI {
	bool Window::OpenGLLoader() {
        hDC = GetDC(hWnd);

        int&& render = ChoosePixelFormat(hDC, &pfd);
        if (render == 0) {
            Creation.set_value(false);
            return false;
        }
        SetPixelFormat(hDC, render, &pfd);
        hRC = wglCreateContext(hDC);
        if (hRC == 0) {
            Creation.set_value(false);
            return false;
        }
        wglMakeCurrent(hDC, hRC);
        
        if (!IsLoadOpenGL) {
            if (!gladLoadGL())
                throw error::OpenGL::Initialization_Failed("gladLoad faild");
            if (!gladLoadWGL(hDC))
                throw error::OpenGL::Initialization_Failed("gladLoadWGL faild");
            IsLoadOpenGL = true;
        }
        return true;
	}
}