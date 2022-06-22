#include "resource/cursor.hpp"

namespace BdUI {
	void Cursor::SaveFile() {
		if (Source == Where::Memory){
			cursor_fileheader = new CursorFileHeader;
			FileStream.write(reinterpret_cast<char*>(cursor_fileheader), sizeof(CursorFileHeader));
		}
		Resource::SaveFile();
	}

	void Cursor::Process() {
		IsProcess = true;
		if (Data == nullptr) throw error::Class::Uninitialize();
		if (Source == Where::File) { 
			cursor_fileheader = reinterpret_cast<CursorFileHeader*>(Data);
			if (cursor_fileheader->Type != 2) throw error::File::Type_Invalid();
			cursor_infoheader = reinterpret_cast<CursorInfoHeader*>(Data + sizeof(CursorFileHeader));
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(Data + sizeof(CursorFileHeader) + sizeof(CursorInfoHeader));
			bitmap_data = reinterpret_cast<RGBQuad*>(Data + sizeof(CursorFileHeader) + sizeof(CursorInfoHeader) + sizeof(BitmapInfoHeader));
			cursor_And = bitmap_data + bitmap_infoheader->SizeImage;
		}
		else {
			cursor_infoheader = reinterpret_cast<CursorInfoHeader*>(Data);
			bitmap_infoheader = reinterpret_cast<BitmapInfoHeader*>(Data + sizeof(CursorInfoHeader));
			bitmap_data = reinterpret_cast<RGBQuad*>(Data + sizeof(CursorInfoHeader) + sizeof(BitmapInfoHeader));
		}
#ifdef WIN32
		cursor = CreateCursor(GetModuleHandle(NULL), cursor_infoheader->X_hotspot, cursor_infoheader->Y_hotspot,
				cursor_infoheader->Width, cursor_infoheader->Height, cursor_And  , bitmap_data);
		if (cursor == nullptr) throw error::Class::Initialize_Failed();
#endif
	}
#ifdef WIN32
	Cursor::Cursor(HCURSOR c) {
		cursor = c;
	}
	HCURSOR Cursor::getIndex() {
		if (cursor == nullptr) Process();
		return cursor;
	}
	Cursor::operator HCURSOR() {
		if (cursor == nullptr) Process();
		return cursor;
	}
	void Cursor::setIndex(HCURSOR c) {
		cursor = c;
	}
	Cursor& Cursor::operator=(HCURSOR c) {
		cursor = c;
		return *this;
	}
#endif
	Cursor& Cursor::operator=(const Cursor& c) {
		cursor = c.cursor;
		Resource::operator=(c);
		return *this;
	}
	Cursor::Cursor(const Cursor& c) {
		cursor = c.cursor;
		Resource::operator=(c);
	}
	Cursor::~Cursor() {
		if (Source == Where::Memory) delete cursor_fileheader;
	}
}