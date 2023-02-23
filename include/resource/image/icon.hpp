#ifndef BDUI_ICON
#define BDUI_ICON

#include <resource/image/image.hpp>
#include <memory>

namespace BdUI {
	class Icon : virtual public Resource,protected Image{
	private:
#ifdef WIN32
		HICON icon = nullptr;
		void CreateHIcon();
#endif
	protected:

		void GetMask();
#pragma pack(1)
		struct IconDir{
			std::uint16_t reserved;//must be 0
			std::uint16_t type; //icon = 1
			std::uint16_t nfiles;
		};
		struct IconDirentry {
			unsigned char bwidth; // width, in pixels, of the image
			unsigned char bheight; // height, in pixels, of the image
			unsigned char bcolorcount; // number of colors in image (0 if >=8bpp)
			unsigned char breserved; // reserved ( must be 0)
			std::uint16_t wplanes; // color planes
			std::uint16_t wbitcount;
			std::uint32_t dwbytesinres; // how many bytes in this resource?
			std::uint32_t dwimageoffset; // where in the file is this image?
		};
		struct Bitmap_FileHeader {
			std::uint16_t bfType;
			std::uint32_t bfSize;
			std::uint16_t bfReserved1;
			std::uint16_t bfReserved2;
			std::uint32_t bfOffBits;
		};
		struct Bitmap_InfoHeader {
			std::uint32_t biSize;
			std::int32_t biWidth;
			std::int32_t biHeight;
			std::uint16_t biPlanes;
			std::uint16_t biBitCount;
			std::uint32_t biCompression;
			std::uint32_t biSizeImage;
			std::int32_t biXPelsPerMeter;
			std::int32_t biYPelsPerMeter;
			std::uint32_t biClrUsed;
			std::uint32_t biClrImportant;
		};
#pragma pack()
		unsigned char *mask = nullptr;
	public:
		const int& Width = _Width;
		const int& Height = _Height;
		const int& BitCount = _BitCount;

		using Image::ImageType;
		using Image::SetImageType;

		Icon(){}
		Icon(const std::filesystem::path&);
		Icon(const Icon&);
		~Icon();

		Icon& Resize(BdUI::Size);

		virtual void OpenFile(const std::filesystem::path&);
		virtual void SaveToFile();
		virtual void LoadFromFile();
		virtual void LoadFromMemory(void* pos,unsigned long long size);
		virtual std::pair<void*, unsigned long long> SaveToMemory();
		
		Icon& operator=(const Icon&);
#ifdef WIN32
		Icon(HICON);
		Icon& operator=(HICON);
		operator HICON();
		HICON getIndex();
		void setIndex(HICON);
		unsigned char* getMask();
#endif
	};
}
#endif