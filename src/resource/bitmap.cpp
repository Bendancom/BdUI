#include "resource/bitmap.hpp"

namespace BdUI {
	void Bitmap::SaveFile() {
		if (Source == Where::Memory)
		{
			if (!IsProcess) Process();
			bitmap_fileheader = new BitmapFileHeader;
			bitmap_fileheader->Size = sizeof(BitmapFileHeader) + bitmap_infoheader->Size + bitmap_infoheader->SizeImage;
			bitmap_fileheader->Offset = reinterpret_cast<unsigned char*>(bitmap_data) - Data;
			FileStream.write(reinterpret_cast<char*>(bitmap_fileheader), sizeof(BitmapFileHeader));
		}
		Resource::SaveFile();
	}

	void Bitmap::Process() {
		IsProcess = true;
		if (Data == nullptr) throw error::Class::Uninitialize();
		if (Source == Where::File) {
			bitmap_fileheader = reinterpret_cast<BitmapFileHeader*>(Data);
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(Data + sizeof(BitmapFileHeader));
			bitmap_data = reinterpret_cast<RGBQuad*>(bitmap_fileheader->Offset + Data);
		}
		else{
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(Data);
			bitmap_data = reinterpret_cast<RGBQuad*>(Data + sizeof(bitmap_infoheader));
		}
#ifdef WIN32
		bitmap = CreateBitmap(bitmap_infoheader->Width, bitmap_infoheader->Height,
			bitmap_infoheader->Planes, bitmap_infoheader->BitCount, bitmap_data);
		if (bitmap == nullptr) throw error::Class::Initialize_Failed();
#endif
	}
#ifdef WIN32
	Bitmap::Bitmap(HBITMAP b) {
		bitmap = b;
	}
	HBITMAP Bitmap::getIndex() {
		if (bitmap == nullptr) Process();
		return bitmap;
	}
	Bitmap::operator HBITMAP() {
		if (bitmap == nullptr) Process();
		return bitmap;
	}
	Bitmap& Bitmap::operator=(HBITMAP b) {
		bitmap = b;
		return *this;
	}
	void Bitmap::setIndex(HBITMAP b) {
		bitmap = b;
	}
#endif
	Bitmap& Bitmap::operator=(const Bitmap& b) {
		bitmap = b.bitmap;
		Resource::operator=(b);
		return *this;
	}
	Bitmap::Bitmap(const Bitmap& b) {
		bitmap = b.bitmap;
		Resource::operator=(b);
	}
	Bitmap::~Bitmap() {
		if (Source == Where::Memory) delete bitmap_fileheader;
	}
}