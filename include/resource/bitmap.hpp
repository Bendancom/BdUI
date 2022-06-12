#ifndef BdUI_BITMAP
#define BdUI_BITMAP
#include "resource.hpp"

namespace BdUI {
	class Bitmap : virtual public Resource {
	protected:
		struct BitmapFileHeader {
			std::int16_t Type = 0x4D42;
			std::int32_t Size;
			std::int16_t Reserved1 = 0;
			std::int16_t Reserved2 = 0;
			std::int32_t Offset;
		}*bitmap_fileheader = nullptr;
		struct BitmapInfoHeader {
			std::int32_t Size = 40;
			std::int32_t Width;
			std::int32_t Height;
			std::int16_t Planes;
			std::int16_t BitCount;
			std::int32_t Compression;
			std::int32_t SizeImage;//����Ϊ4�ı���
			std::int32_t XPelsPerMeter;
			std::int32_t YPelsPerMeter;
			std::int32_t ColorUsed;//0Ϊȫ����ɫ
			std::int32_t ColorImportant;//0Ϊ����Ҫ
		}*bitmap_infoheader = nullptr;
		struct RGBQuad {
			std::int8_t Bule;
			std::int8_t Green;
			std::int8_t Red;
			std::int8_t Reserved = 0;
		}*bitmap_data = nullptr;//���е�ɫ����ָ���ɫ�壬������ֱ��ָ������
#ifdef WIN32
		HBITMAP bitmap = nullptr;
#endif
	public:
		Bitmap() {}
		using Resource::operator=;
		using Resource::Resource;
		~Bitmap();
		virtual void SaveFile();
		virtual void Process();
		Bitmap& operator=(const Bitmap&);
#ifdef WIN32
		Bitmap(HBITMAP);
		operator HBITMAP();
		HBITMAP getIndex();
		void setIndex(HBITMAP);
		Bitmap& operator=(HBITMAP);
#endif
	};
}
#endif