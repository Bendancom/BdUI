#ifndef BDUI_CURSOR
#define BDUI_CURSOR
#include "resource.hpp"
#include "bitmap.hpp"

namespace BdUI{
    class Cursor : public BitMap{
    public:
        Cursor(){}
        Cursor(const std::string&);
        Cursor(const Cursor&);
        Cursor(int);
        ~Cursor();
        virtual void LoadFromFile(const std::string&);
        virtual void LoadFromExecution(int);
        virtual void SaveInFile(const std::string&);
        virtual void SaveInExecution(int);
        Cursor& operator=(const Cursor&);

#ifdef _WIN32
        HCURSOR hCursor;
#endif
        struct CursorInfo {
            unsigned char csWidth;
            unsigned char csHeight;
            unsigned char csColorCount;
            unsigned char csReserved = 0;
            unsigned short csXHotspot;
            unsigned short csYHotspot;
            unsigned long csSize;
            unsigned long csOffset;
        }cursorinfo;
        struct CursorDir {
            unsigned short csReserved = 0;
            unsigned short csType = 2;
            unsigned short csCount;
        }cursordir;
        struct CursorData {
            BitmapInfo bitmapinfo;
            RGBQuad rgbquad;
            void* XORmask;
            void* ANDmask;
        };
    };
}
#endif