#include "./resource/bitmap.hpp"

namespace BdUI {
	BitMap::BitMap(const std::string& s) {
		LoadFromFile(s);
	}
	void BitMap::LoadFromFile(const std::string& s){
		FilePath = s;
		std::fstream file(s,std::ios::in|std::ios::binary);
		file.read((char*)&bitmapfileheader,16);
		FileSize = bitmapfileheader.bfSize;
		file.read((char*)&bitmapinfo,40);
		if (bitmapfileheader.bfOffBits > 56){
			int size = bitmapfileheader.bfOffBits - 56;
			rgbquad = (RGBQuad*)calloc(1,size);
			file.read((char*)rgbquad,size);
		}
		file.close();
	}
}