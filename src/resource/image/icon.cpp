#include "resource/image/icon.hpp"

#define ROTATE_LEFT(x,n) ((x) << (n)) | ((x) >> ((sizeof(x)) - (n))) 
#define ROTATE_RIGHT(x,n) ((x) >> (n)) | ((x) << ((sizeof(x)) - (n)))

namespace BdUI {
	Icon::Icon(const std::filesystem::path& str) {
		OpenFile(str);
	}

	Icon::~Icon() {
		delete[] mask;
#ifdef _WIN32
		DestroyIcon(icon);
#endif
	}

	void Icon::OpenFile(const std::filesystem::path& str) {
		Image::OpenFile(str);
		if (str.extension() == "ico") Image_Type = ICO;
	}

	void Icon::LoadFromFile() {
		delete[] mask;
		Image::LoadFromFile();
		if (Data == nullptr) {
			std::ifstream f(FilePath, std::ios::binary);
			IconDir* icon_dir = reinterpret_cast<IconDir*>(malloc(sizeof(IconDir)));
			f.read((char*)icon_dir, sizeof(IconDir));
			if (icon_dir->reserved != 0 || icon_dir->type != 1) throw error::File::Data_Error("Data isn't the icon format");
			free(icon_dir);
			IconDirentry *icon_direntry = new IconDirentry;
			f.read((char*)icon_direntry, sizeof(IconDirentry));
			int a = sizeof(IconDirentry);
			_Width = icon_direntry->bwidth;
			_Height = icon_direntry->bheight;

			unsigned long bitmap_file_size = sizeof(Bitmap_FileHeader) + icon_direntry->dwbytesinres - icon_direntry->bwidth * icon_direntry->bheight / 8;
			char* bitmap = (char*)calloc(bitmap_file_size,1);
			Bitmap_FileHeader* bitmap_fileheader = (Bitmap_FileHeader*)bitmap;
			bitmap_fileheader->bfType = 0x4d42;
			bitmap_fileheader->bfSize = bitmap_file_size;
			bitmap_fileheader->bfReserved1 = 0;
			bitmap_fileheader->bfReserved2 = 0;
			bitmap_fileheader->bfOffBits = sizeof(Bitmap_FileHeader) + sizeof(Bitmap_InfoHeader) + icon_direntry->bcolorcount * sizeof(RGBQuad);
			
			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>(bitmap + sizeof(Bitmap_FileHeader));
			f.seekg(icon_direntry->dwimageoffset);
			f.read((char*)bitmap_infoheader, sizeof(Bitmap_InfoHeader));
			
			bitmap_infoheader->biHeight /= 2;
			bitmap_infoheader->biSizeImage -= Width * Height / 8;

			RGBQuad* bitmap_quad = reinterpret_cast<RGBQuad*>((char*)bitmap_infoheader + sizeof(Bitmap_InfoHeader));
			f.read((char*)bitmap_quad, bitmap_infoheader->biClrUsed * sizeof(RGBQuad));
			f.read((char*)bitmap + bitmap_fileheader->bfOffBits, bitmap_infoheader->biSizeImage);

			Image::LoadFromMemory(bitmap, bitmap_file_size);

			unsigned int icon_mask_size = icon_direntry->bheight * icon_direntry->bwidth / 8;
			mask = new unsigned char[icon_mask_size];
			f.read((char*)mask, icon_mask_size);

			free(bitmap);
		}
		else GetMask();
#ifdef _WIN32
		CreateHIcon();
#endif

	}
	void Icon::LoadFromMemory(void* pos, unsigned long long size) {
		delete[] mask;
		Image::LoadFromMemory(pos, size);
		if (Data == nullptr) {
			IconDir* idir = (IconDir*)malloc(size);
			memcpy(idir, pos, sizeof(IconDir) + sizeof(IconDirentry));

			IconDirentry* icon_direntry = reinterpret_cast<IconDirentry*>((char*)idir + sizeof(IconDir));
			_Width = icon_direntry->bwidth;
			_Height = icon_direntry->bheight;

			unsigned long bitmap_file_size = sizeof(Bitmap_FileHeader) + icon_direntry->dwbytesinres - icon_direntry->bwidth * icon_direntry->bheight / 8;
			char* bitmap = (char*)calloc(bitmap_file_size, 1);
			Bitmap_FileHeader* bitmap_fileheader = (Bitmap_FileHeader*)bitmap;
			bitmap_fileheader->bfType = 0x4d42;
			bitmap_fileheader->bfSize = bitmap_file_size;
			bitmap_fileheader->bfReserved1 = 0;
			bitmap_fileheader->bfReserved2 = 0;

			memcpy(bitmap + sizeof(Bitmap_FileHeader), (char*)pos + sizeof(IconDir) + sizeof(IconDirentry), size - sizeof(IconDir) + sizeof(IconDirentry) - Width * Height / 8);

			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>(bitmap + sizeof(Bitmap_FileHeader));

			bitmap_fileheader->bfOffBits = sizeof(Bitmap_FileHeader) + sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biClrUsed * sizeof(RGBQuad);
			bitmap_infoheader->biHeight /= 2;
			bitmap_infoheader->biSizeImage -= Width * Height / 8;

			Image::LoadFromMemory(bitmap, bitmap_file_size);

			mask = new unsigned char[Width * Height / 8];
			memcpy(mask, (char*)pos + size - Width * Height / 8, Width * Height / 8);

			free(idir);
			free(bitmap);
		}
		else GetMask();
#ifdef _WIN32
		CreateHIcon();
#endif
	}

	void Icon::SaveToFile() {
		if (Image_Type == ICO) {
			if (Data == nullptr) throw error::File::Data_Error("Have no Data");

			SetImageType(BMP);

			std::ofstream file(FilePath, std::ios::binary);

			IconDir idir = { 0,1,1 };
			file.write((char*)&idir, sizeof(IconDir));

			IconDirentry idirent = { 
				Width,
				Height,
				0,
				0,
				1,
				BitCount,
				0,
				sizeof(IconDir) + sizeof(IconDirentry)
			};

			std::pair<void*,unsigned long long> bitmap = Image::SaveToMemory();
			Bitmap_FileHeader* bitmap_fileheader = reinterpret_cast<Bitmap_FileHeader*>(bitmap.first);
			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>((char*)bitmap_fileheader + sizeof(Bitmap_FileHeader));
			bitmap_infoheader->biSize = sizeof(Bitmap_InfoHeader);
			bitmap_infoheader->biCompression = 0;
			bitmap_infoheader->biHeight *= 2;
			bitmap_infoheader->biSizeImage = Width * Height / 8 + bitmap_infoheader->biBitCount * Width * Height / 8;

			idirent.dwbytesinres = sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biSizeImage;
			file.write((char*)&idirent, sizeof(IconDirentry));
			file.write((char*)bitmap_infoheader, sizeof(Bitmap_InfoHeader));
			file.write((char*)bitmap_fileheader + bitmap_fileheader->bfOffBits, bitmap.second - bitmap_fileheader->bfOffBits);
			file.write((char*)mask, Width * Height / 8);

			SetImageType(ICO);
		}
		else Image::SaveToFile();
	}

	std::pair<void*, unsigned long long> Icon::SaveToMemory() {
		if (Data == nullptr) throw error::File::Data_Error("Have no Data");
		if (Image_Type == ICO) {
			SetImageType(BMP);
			std::pair<void*, unsigned long long> bitmap = Image::SaveToMemory();
			SetImageType(ICO);

			Bitmap_FileHeader* bitmap_fileheader = reinterpret_cast<Bitmap_FileHeader*>(bitmap.first);
			Bitmap_InfoHeader* bitmap_infoheader = reinterpret_cast<Bitmap_InfoHeader*>((char*)bitmap_fileheader + sizeof(Bitmap_FileHeader));

			bitmap_infoheader->biSize = sizeof(Bitmap_InfoHeader);
			bitmap_infoheader->biCompression = 0;
			bitmap_infoheader->biHeight *= 2;
			unsigned long long bitmap_sizeimage = Width * Height / 8 + bitmap_infoheader->biBitCount * Width * Height / 8;
			bitmap_infoheader->biSizeImage = bitmap_sizeimage;

			char* ico = (char*)malloc(sizeof(IconDir) + sizeof(IconDirentry) + sizeof(Bitmap_InfoHeader) + bitmap_sizeimage);
			IconDir* idir = reinterpret_cast<IconDir*>(ico);
			idir->nfiles = 1;
			idir->reserved = 0;
			idir->type = 1;
			IconDirentry* idirent = reinterpret_cast<IconDirentry*>(ico + sizeof(IconDir));
			idirent->bwidth = Width;
			idirent->bheight = Height;
			idirent->bcolorcount = bitmap_infoheader->biClrUsed;
			idirent->breserved = 0;
			idirent->wbitcount = bitmap_infoheader->biBitCount;
			idirent->wplanes = 1;
			idirent->dwimageoffset = sizeof(IconDir) + sizeof(IconDirentry);
			idirent->dwbytesinres = sizeof(Bitmap_InfoHeader) + bitmap_infoheader->biSizeImage;

			memcpy(ico + idirent->dwimageoffset, bitmap_infoheader, sizeof(Bitmap_InfoHeader));
			memcpy(ico + idirent->dwimageoffset + sizeof(Bitmap_InfoHeader), (char*)bitmap.first + bitmap_fileheader->bfOffBits, bitmap_infoheader->biBitCount * Width * Height / 8);
			memcpy(ico + idirent->dwimageoffset + sizeof(Bitmap_InfoHeader) + idirent->wbitcount * Width * Height, mask, Width * Height / 8);
			free(bitmap.first);
			return { ico,sizeof(IconDir) + sizeof(IconDirentry) + sizeof(Bitmap_InfoHeader) + bitmap_sizeimage};
		}
		else return Image::SaveToMemory();
	}

	Icon& Icon::Resize(long long width,long long height) {
		Image::Resize(width,height);
		GetMask();
#ifdef _WIN32
		CreateHIcon();
#endif
		return *this;
	}

	void Icon::GetMask() {
		if (BitCount != 32 && BitCount != 16)
			mask = (unsigned char*)calloc(1, Width * Height / 8);
		else {
			mask = (unsigned char*)calloc(1, Width * Height / 8);
			for (int i = 0; i < Width; i++)
				for (int j = 0; j < Height / 8; j++)
					for (int k = 0; k < 8; k++)
						if (BitCount == 16)
							*(mask + j) |= ROTATE_RIGHT((Data[(i * Width * BitCount / 8 + (j * 8 + k) * BitCount / 8) + 1] > 0), k + 1);
						else
							*(mask + j) |= ROTATE_RIGHT((Data[(i * Width * BitCount / 8 + (j * 8 + k) * BitCount / 8) + 3] > 0), k + 1);
		}
	}

#ifdef WIN32
	void Icon::CreateHIcon() {
		DestroyIcon(icon);
		HBITMAP i_mask = CreateBitmap(Width, Height, 1, 1, mask);
		ICONINFO icon_info = { true,0,0,i_mask,Image::getIndex() };
		icon = CreateIconIndirect(&icon_info);
		DeleteObject(i_mask);
	}

	void Icon::setIndex(HICON i) {
		icon = (HICON)CopyImage(i, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	}
	Icon::operator HICON() {
		if (icon == nullptr) throw error::Class::Uninitialize();
		return icon;
	}
	HICON Icon::getIndex() {
		if (icon == nullptr) throw error::Class::Uninitialize();
		return icon;
	}
	Icon& Icon::operator=(HICON i) {
		icon = (HICON)CopyImage(i, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
		return *this;
	}
	Icon::Icon(HICON i) {
		icon = (HICON)CopyImage(i, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	}
#endif

	Icon::Icon(const Icon& i) {
		Image::operator=(i);
		if (i.mask != nullptr) {
			delete[] mask;
			mask = new unsigned char[Width * Height / 8];
			memcpy(mask, i.mask, sizeof(IconDir));
		}
#ifdef _WIN32
		icon = (HICON)CopyImage(i.icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
#endif
	}
	Icon &Icon::operator=(const Icon& i) {
		Image::operator=(i);
		if (i.mask != nullptr) {
			delete[] mask;
			mask = new unsigned char[Width * Height / 8];
			memcpy(mask, i.mask, sizeof(IconDir));
		}
#ifdef _WIN32
		icon = (HICON)CopyImage(i.icon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
#endif
		return *this;
	}
}