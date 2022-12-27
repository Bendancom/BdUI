#include <resource/image/image.hpp>

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#ifdef _WIN32
#define STBI_WINDOWS_UTF8
#endif
#include <resource/stb/stb_image.h>
#include <resource/stb/stb_image_write.h>
#include <resource/stb/stb_image_resize.h>

namespace BdUI {
	void Image::WriteToMemory(void* context,void* data,int size) {
		std::vector<unsigned char>* buffer = reinterpret_cast<std::vector<unsigned char>*>(context);
		for (int i = 0; i < size; i++) buffer->push_back(*((unsigned char*)data + i));
	}

	Image::Image(const std::string& str) {
		Resource_Type = Resource::Image;
		OpenFile(str);
	}

	void Image::OpenFile(const std::string& str) {
		Resource::OpenFile(str);
		if (File_Ext == "jpeg" || File_Ext == "jpg") Image_Type = JPEG;
		else if (File_Ext == "bmp") Image_Type = BMP;
		else if (File_Ext == "png") Image_Type = PNG;
		else if (File_Ext == "hdr") Image_Type = HDR;
		else if (File_Ext == "tga") Image_Type = TGA;
	}

	Image::~Image() {
#ifdef _WIN32
		DeleteObject(bitmap);
#endif
	}

	std::pair<void*, unsigned long long> Image::SaveToMemory() {
		if (Data == nullptr) throw error::File::Data_Error("Have no Data");

		std::vector<unsigned char> buffer;
		switch (Image_Type)
		{
		case BdUI::Image::BMP:
			stbi_write_bmp_to_func(WriteToMemory, &buffer, Width, Height, BitCount / 8 , Data);
			break;
		case BdUI::Image::JPEG:
			stbi_write_jpg_to_func(WriteToMemory, &buffer, Width, Height, BitCount / 8, Data, 90);
			break;
		case BdUI::Image::PNG:
			stbi_write_png_to_func(WriteToMemory, &buffer, Width, Height, BitCount / 8, Data, Width * BitCount / 8);
			break;
		case BdUI::Image::HDR:
			stbi_write_hdr_to_func(WriteToMemory, &buffer, Width, Height, BitCount / 8, (float*)Data);
			break;
		case BdUI::Image::TGA:
			stbi_write_tga_to_func(WriteToMemory, &buffer, Width, Height, BitCount / 8, Data);
			break;
		default:
			throw error::File::Type_Invalid("Can't Output this format");
			break;
		}
		unsigned long long size = buffer.size();
		void* pos = malloc(size);
		memcpy(pos, buffer.data(), size);
		return { pos,size };
	}

	void Image::SaveToFile() {
		if (FilePath.empty() == true) throw error::File::Open_Failed("Haven't opened the file");
		if (Data == nullptr) throw error::File::Data_Error("Have no Data");

		switch (Image_Type)
		{
		case BdUI::Image::BMP:
			stbi_write_bmp(FilePath.c_str(), Width, Height, BitCount / 8, Data);
			break;
		case BdUI::Image::JPEG:
			stbi_write_jpg(FilePath.c_str(), Width, Height, BitCount / 8, Data, 90);
			break;
		case BdUI::Image::PNG:
			stbi_write_png(FilePath.c_str(), Width, Height, BitCount / 8, Data, Width * BitCount / 8);
			break;
		case BdUI::Image::HDR:
			stbi_write_hdr(FilePath.c_str(), Width, Height, BitCount / 8, (float*)Data);
			break;
		case BdUI::Image::TGA:
			stbi_write_tga(FilePath.c_str(), Width, Height, BitCount / 8, Data);
			break;
		default:
			throw error::File::Type_Invalid("Can't Output this format");
			break;
		}
	}

	void Image::LoadFromFile() {
		Clear();
		std::ifstream file(FilePath, std::ios::binary);
		if (!file) throw error::File::Open_Failed("Can't open the file");
		file.seekg(0,std::ios::end);
		unsigned long long size = file.tellg();
		file.seekg(0,std::ios::beg);
		unsigned char* pos = (unsigned char*)malloc(size);
		file.read((char*)pos, size);
		int n;
		Data = stbi_load_from_memory(pos, size, &_Width, &_Height, &n, 4);
		_BitCount = n * 8;
		Size = Width * Height * BitCount / 8;
		free(pos);
#ifdef _WIN32
		CreateHBitmap();
#endif
	}

	void Image::LoadFromMemory(void* pos,unsigned long long size) {
		Clear();
		int n;
		Data = stbi_load_from_memory((unsigned char*)pos, size, &_Width, &_Height, &n, 4);
		_BitCount = n * 8;
		Size = Width * Height * BitCount / 8;
#ifdef _WIN32
		CreateHBitmap();
#endif
	}

	Image& Image::Resize(BdUI::Size size) {
		size.ChangeUnit(UnitType::Pixel);
		Size = size.Width * size.Height * BitCount / 8;
		unsigned char* output = (unsigned char*)malloc(Size);
		stbir_resize_uint8(Data, Width, Height, 0, output, size.Width, size.Height, 0, BitCount / 8);
		delete[] Data;
		Data = output;
		_Width = size.Width;
		_Height = size.Height;
		CreateHBitmap();
		return *this;
	}

	void Image::SetImageType(ImageType type) {
		Image_Type = type;
	}

#ifdef WIN32
	void Image::CreateHBitmap() {
		DeleteObject(bitmap);
		char* data = (char*)malloc(Size);
		memcpy(data, Data, Size);
		if (BitCount == 32) {
			RGBQuad* rgb = reinterpret_cast<RGBQuad*>(data);
			for (int i = 0; i < Size * 8 / BitCount; i++) {
				unsigned char r = rgb->rgbBlue;
				rgb->rgbBlue = rgb->rgbRed;
				rgb->rgbRed = r;
				rgb++;
			}
		}
		else if (BitCount == 24){
			struct BGR{
				unsigned char B;
				unsigned char G;
				unsigned char R;
			}* bgr = reinterpret_cast<BGR*>(data);
			for (int i = 0; i < Size * 8 / BitCount; i++) {
				unsigned char r = bgr->B;
				bgr->B = bgr->R;
				bgr->R = r;
				bgr++;
			}
		}
		if (Data != nullptr) bitmap = CreateBitmap(Width, Height, 1, BitCount, data);
		free(data);
	}

	Image::Image(HBITMAP b) {
		bitmap = (HBITMAP)CopyImage(b, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	}
	HBITMAP Image::getIndex() {
		if (bitmap == nullptr) throw error::File::Data_Error("Not have the data");
		return bitmap;
	}
	Image::operator HBITMAP() {
		if (bitmap == nullptr) throw error::File::Data_Error("Not have the data");
		return bitmap;
	}
	Image& Image::operator=(HBITMAP b) {
		bitmap = (HBITMAP)CopyImage(b, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		return *this;
	}
	void Image::setIndex(HBITMAP b) {
		bitmap = (HBITMAP)CopyImage(b, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	}
#endif
	Image& Image::operator=(const Image& b) {
		Resource::operator=(b);
		_Width = b.Width;
		_Height = b.Height;
		_BitCount = b.BitCount;
#ifdef _WIN32
		bitmap = (HBITMAP)CopyImage(b.bitmap, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
#endif
		return *this;
	}
	Image::Image(const Image& b) {
		Resource::operator=(b);
		_Width = b.Width;
		_Height = b.Height;
		_BitCount = b.BitCount;
#ifdef _WIN32
		bitmap = (HBITMAP)CopyImage(b.bitmap, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
#endif
	}
}