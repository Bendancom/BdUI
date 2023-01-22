#include "resource/image/cursor.hpp"

namespace BdUI {
	Cursor::Cursor(const std::string& str) {
		OpenFile(str);
	}

	void Cursor::OpenFile(const std::string& str) {
		Resource::OpenFile(str);
		if (File_Ext == "cur") Image_Type = CUR;
		if (File_Ext == "ani") Image_Type = ANI;
	}

	void Cursor::LoadFromFile() {
		delete[] mask;
		Image::LoadFromFile();
		if (Data == nullptr) {
			std::ifstream file(FilePath, std::ios::binary);
			if (Image_Type == CUR) {
				CursorFileHeader fileheader;
				CursorInfoHeader infoheader;
				file.read((char*)&fileheader, sizeof(CursorFileHeader));
				if (fileheader.type != 2) throw error::File::Type_Invalid("It isn't cursor");

				file.read((char*)&infoheader, sizeof(CursorInfoHeader));
				_Width = infoheader.Width;
				_Height = infoheader.Height;

				_X = infoheader.X_hotspot;
				_Y = infoheader.Y_hotspot;

				file.seekg(infoheader.Offset);

				unsigned long long bitmap_filesize = infoheader.ByteSize + sizeof(Bitmap_FileHeader) - Width * Height / 8;
				char* bitmap = (char*)malloc(bitmap_filesize);
				Bitmap_FileHeader* bitmap_fileheader = reinterpret_cast<Bitmap_FileHeader*>(bitmap);
				bitmap_fileheader->bfReserved1 = 0;
				bitmap_fileheader->bfReserved2 = 0;
				bitmap_fileheader->bfType = 0x4d42;
				bitmap_fileheader->bfSize = bitmap_filesize;
				bitmap_fileheader->bfOffBits = sizeof(Bitmap_FileHeader) + sizeof(Bitmap_InfoHeader) + infoheader.ColorCount * sizeof(RGBQuad);

				Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>(bitmap + sizeof(Bitmap_FileHeader));
				file.read((char*)bitmap_infoheader, sizeof(Bitmap_InfoHeader));

				bitmap_infoheader->biHeight /= 2;
				bitmap_infoheader->biSizeImage -= Width * Height / 8;

				_BitCount = bitmap_infoheader->biBitCount;

				RGBQuad* bitmap_quad = reinterpret_cast<RGBQuad*>((char*)bitmap_infoheader + sizeof(Bitmap_InfoHeader));
				file.read((char*)bitmap_quad, bitmap_infoheader->biClrUsed * sizeof(RGBQuad));
				file.read((char*)bitmap + bitmap_fileheader->bfOffBits, bitmap_infoheader->biSizeImage);

				Image::LoadFromMemory(bitmap, bitmap_filesize);

				unsigned int mask_size = Width * Height / 8;
				mask = new unsigned char[mask_size];
				file.read((char*)mask, mask_size);

				free(bitmap);
			}
			else if (Image_Type == ANI) {

			}
		}
		else {
			_X = 0;
			_Y = 0;
			GetMask();
		}
	}

	void Cursor::LoadFromMemory(void* pos,unsigned long long size) {
		delete[] mask;
		Image::LoadFromMemory(pos, size);
		if (Data == nullptr) {
			CursorFileHeader* fileheader = (CursorFileHeader*)malloc(size);
			memcpy(fileheader, pos, sizeof(CursorFileHeader) + sizeof(CursorInfoHeader));

			CursorInfoHeader* infoheader = reinterpret_cast<CursorInfoHeader*>((char*)fileheader + sizeof(CursorFileHeader));
			_Width = infoheader->Width;
			_Height = infoheader->Height;

			_X = infoheader->X_hotspot;
			_Y = infoheader->Y_hotspot;

			unsigned long bitmap_file_size = sizeof(Bitmap_FileHeader) + infoheader->ByteSize - Width * Height / 8;
			char* bitmap = (char*)calloc(bitmap_file_size, 1);
			Bitmap_FileHeader* bitmap_fileheader = (Bitmap_FileHeader*)bitmap;
			bitmap_fileheader->bfType = 0x4d42;
			bitmap_fileheader->bfSize = bitmap_file_size;
			bitmap_fileheader->bfReserved1 = 0;
			bitmap_fileheader->bfReserved2 = 0;

			memcpy(bitmap + sizeof(Bitmap_FileHeader),
				(char*)pos + sizeof(CursorFileHeader) + sizeof(CursorInfoHeader),
				size - sizeof(CursorFileHeader) + sizeof(CursorInfoHeader) - Width * Height / 8);

			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>(bitmap + sizeof(Bitmap_FileHeader));

			bitmap_fileheader->bfOffBits = sizeof(Bitmap_FileHeader) + sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biClrUsed * sizeof(RGBQuad);
			bitmap_infoheader->biHeight /= 2;
			bitmap_infoheader->biSizeImage -= Width * Height / 8;

			_BitCount = bitmap_infoheader->biBitCount;

			Image::LoadFromMemory(bitmap, bitmap_file_size);

			mask = new unsigned char[Width * Height / 8];
			memcpy(mask, (char*)pos + size - Width * Height / 8, Width * Height / 8);

			free(fileheader);
			free(bitmap);
		}
		else {
			_X = 0;
			_Y = 0;
			GetMask();
		}
	}

	void Cursor::SaveToFile() {
		if (Data == nullptr) throw error::File::Data_Error("Have no Data");
		if (Image_Type == CUR) {
			std::ofstream file(FilePath, std::ios::binary);

			CursorFileHeader fileheader = { 0,2,1 };
			file.write((char*)&fileheader, sizeof(CursorFileHeader));

			CursorInfoHeader infoheader = {
				Width,
				Height,
				0,
				0,
				X,
				Y,
				0,
				sizeof(CursorFileHeader) + sizeof(CursorInfoHeader)
			};

			std::pair<void*, unsigned long long> bitmap = Image::SaveToMemory();
			Bitmap_FileHeader* bitmap_fileheader = reinterpret_cast<Bitmap_FileHeader*>(bitmap.first);
			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>((char*)bitmap_fileheader + sizeof(Bitmap_FileHeader));
			bitmap_infoheader->biSize = sizeof(Bitmap_InfoHeader);
			bitmap_infoheader->biCompression = 0;
			bitmap_infoheader->biHeight *= 2;
			bitmap_infoheader->biSizeImage = Width * Height / 8 + bitmap_infoheader->biBitCount * Width * Height / 8;

			infoheader.ByteSize = sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biSizeImage;
			file.write((char*)&infoheader, sizeof(CursorInfoHeader));
			file.write((char*)bitmap_infoheader, sizeof(Bitmap_InfoHeader));
			file.write((char*)bitmap_fileheader + bitmap_fileheader->bfOffBits, bitmap.second - bitmap_fileheader->bfOffBits);
			file.write((char*)mask, Width * Height / 8);
		}
		else if (Image_Type == ANI) {

		}
		else Image::SaveToFile();
	}

	std::pair<void*, unsigned long long> Cursor::SaveToMemory() {
		if (Data == nullptr) throw error::File::Data_Error("Have no Data");
		if (Image_Type == CUR) {
			SetImageType(BMP);
			std::pair<void*, unsigned long long> bitmap = Image::SaveToMemory();
			SetImageType(CUR);

			Bitmap_FileHeader* bitmap_fileheader = reinterpret_cast<Bitmap_FileHeader*>(bitmap.first);
			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>((char*)bitmap_fileheader + sizeof(Bitmap_FileHeader));

			bitmap_infoheader->biSize = sizeof(Bitmap_InfoHeader);
			bitmap_infoheader->biCompression = 0;
			bitmap_infoheader->biHeight *= 2;
			unsigned long long bitmap_sizeimage = Width * Height / 8 + bitmap_infoheader->biBitCount * Width * Height / 8;
			bitmap_infoheader->biSizeImage = bitmap_sizeimage;

			char* cur = (char*)malloc(sizeof(CursorFileHeader) + sizeof(CursorInfoHeader) + sizeof(Bitmap_InfoHeader) + bitmap_sizeimage);
			CursorFileHeader* fileheader = reinterpret_cast<CursorFileHeader*>(cur);
			fileheader->nfiles = 1;
			fileheader->reserved = 0;
			fileheader->type = 1;
			CursorInfoHeader* infoheader = reinterpret_cast<CursorInfoHeader*>(cur + sizeof(CursorFileHeader));
			infoheader->Width = Width;
			infoheader->Height = Height;
			infoheader->ColorCount = bitmap_infoheader->biClrUsed;
			infoheader->Reserved = 0;
			infoheader->X_hotspot = X;
			infoheader->Y_hotspot = Y;
			infoheader->Offset = sizeof(CursorFileHeader) + sizeof(CursorInfoHeader);
			infoheader->ByteSize = sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biSizeImage;

			memcpy(cur + infoheader->Offset, bitmap_infoheader, sizeof(Bitmap_InfoHeader));
			memcpy(cur + infoheader->Offset + sizeof(Bitmap_InfoHeader), (char*)bitmap.first + bitmap_fileheader->bfOffBits, bitmap_infoheader->biBitCount * Width * Height / 8);
			memcpy(cur + infoheader->Offset + sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biBitCount * Width * Height, mask, Width * Height / 8);
			free(bitmap.first);
			return { cur,sizeof(CursorFileHeader) + sizeof(CursorInfoHeader) + sizeof(Bitmap_InfoHeader) + bitmap_sizeimage };
		}
		else if (Image_Type == ANI) {
			return { 0,0 };
		}
		else return Image::SaveToMemory();
	}

	void Cursor::SetHotPoint(Point point) {
		point.ChangeUnit(UnitType::Pixel);
		_X = point.X;
		_Y = point.Y;
#ifdef _WIN32
		CreateHCursor();
#endif
	}

	Cursor& Cursor::Resize(BdUI::Size size) {
		Image::Resize(size);
		GetMask();
#ifdef _WIN32
		CreateHCursor();
#endif
		return *this;
	}

#ifdef WIN32
	void Cursor::CreateHCursor() {
		DestroyCursor(cursor);
		HBITMAP c_mask = CreateBitmap(Width, Height, 1, 1, mask);
		ICONINFO info = {
			false,X,Y,c_mask,Image::getIndex()
		};
		cursor = CreateIconIndirect(&info);
	}
	Cursor::Cursor(HCURSOR c) {
		cursor = (HCURSOR)CopyImage(c, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	}
	HCURSOR Cursor::getIndex() {
		if (cursor == nullptr) throw error::Class::Uninitialize();
		return cursor;
	}
	Cursor::operator HCURSOR() {
		if (cursor == nullptr) throw error::Class::Uninitialize();
		return cursor;
	}
	void Cursor::setIndex(HCURSOR c) {
		cursor = (HCURSOR)CopyImage(c, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	}
	Cursor& Cursor::operator=(HCURSOR c) {
		cursor = (HCURSOR)CopyImage(c, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
		return *this;
	}
#endif

	Cursor& Cursor::operator=(const Cursor& c) {
		Image::operator=(c);
#ifdef _WIN32
		DestroyCursor(cursor);
		cursor = (HCURSOR)CopyImage(c.cursor, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
#endif
		return *this;
	}
	Cursor::Cursor(const Cursor& c) {
		Image::operator=(c);
#ifdef _WIN32
		cursor = (HCURSOR)CopyImage(c.cursor, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
#endif
	}
	Cursor::~Cursor() {
#ifdef _WIN32
		DestroyCursor(cursor);
#endif
	}
}