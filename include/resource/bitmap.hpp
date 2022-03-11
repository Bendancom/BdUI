#ifndef BDUI_BITMAP
#define BDUI_BITMAP
#include "resource.hpp"

namespace BdUI{
    class BitMap : public IResource{
    protected:
        struct BitmapInfo{
            unsigned long biSize = 40;
            long biWidth;
            long biHeight; 
            unsigned short biPlanes;
            unsigned short biBitCount;
            unsigned long biCompression;
            unsigned long biSizeImage; 
            long biXPelsPerMeter; 
            long biYPelsPerMeter; 
            unsigned long biClrUsed = 0; 
            unsigned long biClrImportant = 0;
        }bitmapinfo;
        struct RGBQuad {
            unsigned char Blue;
            unsigned char Green;
            unsigned char Red;
            unsigned char Reserved;
        }*rgbquad = nullptr;
        unsigned char* data;
        struct BitMapFileHeader{
	        unsigned short bfType = 0x4d42; //固定为0x4d42;
            unsigned long bfSize; //文件大小
            unsigned short bfReserved1 = 0; //保留
            unsigned short bfReserved2 = 0; //保留
            unsigned long bfOffBits; //相对于文件起始的偏移量
        }bitmapfileheader;
    public:
        BitMap() {}
        BitMap(const std::string&);
        BitMap(const BitMap&);
        ~BitMap();
        virtual void LoadFromFile(const std::string&);
        virtual void LoadFromExecution(int);
        virtual void SaveInFile(const std::string&);
        virtual void SaveInExecution(int);
        const long& Width = bitmapinfo.biWidth;
        const long& Height = bitmapinfo.biHeight;
        const unsigned short& Planes = bitmapinfo.biPlanes;
        const unsigned short& PixelDepth = bitmapinfo.biBitCount;
    };
}
#endif