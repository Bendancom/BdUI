#include "resource/bitmap.hpp"

namespace BdUI {
	void Bitmap::SaveFile() {
		if (!IsProcess) Process();
		bitmap_fileheader = new BitmapFileHeader;
		bitmap_fileheader->Size = sizeof(BitmapFileHeader) + bitmap_infoheader->Size + bitmap_infoheader->SizeImage;
		bitmap_fileheader->Offset = reinterpret_cast<unsigned char*>(bitmap_data) - Data;
	}

	void Bitmap::Process() {
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
		CreateBitmap(bitmap_infoheader->Width, bitmap_infoheader->Height,
			bitmap_infoheader->Planes, bitmap_infoheader->BitCount, bitmap_data);
#endif
		IsProcess = true;
	}
#ifdef WIN32
	Bitmap::Bitmap(HBITMAP b) {
		bitmap = b;
	}
	HBITMAP Bitmap::getIndex() {
		if (bitmap != nullptr) return bitmap;
		else throw error::Class::Uninitialize();
	}
	Bitmap::operator HBITMAP() {
		if (bitmap != nullptr) return bitmap;
		else throw error::Class::Uninitialize();
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
		Resource::operator=(b);
		return *this;
	}
	Bitmap::~Bitmap() {
		if (Source == Where::Memory) delete bitmap_fileheader;
	}
}