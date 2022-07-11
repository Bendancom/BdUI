#include "resource/icon.hpp"

namespace BdUI {
	void Icon::SaveFile() {
		if (Source == Where::Memory) {
			icon_fileheader = new IconFileHeader;
			if (FileStream != nullptr) FileStream->write(reinterpret_cast<char*>(icon_fileheader), sizeof(IconFileHeader));
		}
		Resource::SaveFile();
	}
	void Icon::Process() {
		IsProcess = true;
		if (Data == nullptr) throw error::Class::Uninitialize();
		if (Source == Where::File) {
			icon_fileheader = reinterpret_cast<IconFileHeader*>(Data);
			icon_infoheader = reinterpret_cast<IconInfoHeader*>(Data + sizeof(IconFileHeader));
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(icon_infoheader->Offset);
			bitmap_data = reinterpret_cast<RGBQuad*>(Data + sizeof(IconInfoHeader) + bitmap_infoheader->Size);
			icon_And = bitmap_data + bitmap_infoheader->SizeImage;
		}
		else {
			icon_infoheader = reinterpret_cast<IconInfoHeader*>(Data);
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(icon_infoheader->Offset);
			bitmap_data = reinterpret_cast<RGBQuad*>(Data + sizeof(IconInfoHeader) + bitmap_infoheader->Size);
			icon_And = bitmap_data + bitmap_infoheader->SizeImage;
		}
#ifdef WIN32
		icon = CreateIcon(GetModuleHandle(NULL), icon_infoheader->Width, icon_infoheader->Height, icon_infoheader->Planes,
			icon_infoheader->BitCount, reinterpret_cast<BYTE*>(icon_And), reinterpret_cast<BYTE*>(bitmap_data));
		if (icon == nullptr) throw error::Class::Initialize_Failed();
#endif
	}
#ifdef WIN32
	void Icon::setIndex(HICON i) {
		icon = i;
	}
	Icon::operator HICON() {
		if (icon == nullptr) Process();
		return icon;
	}
	HICON Icon::getIndex() {
		if (icon == nullptr) Process();
		return icon;
	}
	Icon& Icon::operator=(HICON i) {
		icon = i;
		return *this;
	}
	Icon::Icon(HICON i) {
		icon = i;
	}
#endif
	Icon::Icon(const Icon& i) {
		icon = i.icon;
		Resource::operator=(i);
	}
	Icon &Icon::operator=(const Icon& i) {
		icon = i.icon;
		Resource::operator=(i);
		return *this;
	}
}