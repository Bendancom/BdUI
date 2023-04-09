#ifndef BdUI_IMAGE
#define BdUI_IMAGE
#include <graph/size.hpp>
#include <resource/resource.hpp>
#include <vector>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace BdUI {
	class Image : virtual public Resource {
	private:
		static void WriteToMemory(void*, void*, int);
#ifdef WIN32
		HBITMAP bitmap = nullptr;
		void CreateHBitmap();
#endif
	public:
		enum ImageType {
			BMP = 0,
			JPEG = 1,
			PNG = 2,
			HDR = 3,
			TGA = 4,
			ICO = 5,
			CUR = 6,
		};
	protected:
		ImageType Image_Type;
		int _Width = 0,_Height = 0,_BitCount = 0;
#pragma pack(1)
		struct RGBQuad {
			unsigned char rgbBlue;
			unsigned char rgbGreen;
			unsigned char rgbRed;
			unsigned char rgbReserved;
		};
#pragma pack()
	public:
		const int& Width = _Width;
		const int& Height = _Height;
		const int& BitCount = _BitCount;

		Image() {}
		Image(const std::filesystem::path&);
		Image(const Image&);
		~Image();

		Image& Resize(long long width,long long height);

		void SetImageType(ImageType);

		virtual void OpenFile(const std::filesystem::path&);
		virtual void SaveToFile();
		virtual std::pair<void*,unsigned long long> SaveToMemory();
		virtual void LoadFromFile();
		virtual void LoadFromMemory(void* pos,unsigned long long size);
		Image& operator=(const Image&);
#ifdef WIN32
		Image(HBITMAP);
		operator HBITMAP();
		HBITMAP getIndex();
		void setIndex(HBITMAP);
		Image& operator=(HBITMAP);
#endif
	};
}
#endif