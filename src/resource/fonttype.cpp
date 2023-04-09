#include <resource/fonttype.hpp>

namespace BdUI {
	FontType::FontType(const FontType& font) {

	}
	FontType::FontType(const std::filesystem::path& str) {
		OpenFile(str);
	}

	void FontType::OpenFile(const std::filesystem::path& str) {
		FilePath = str;
	}
	void FontType::LoadFromFile() {
		std::ifstream file(FilePath);
		file.seekg(0, std::ios::end);
		Size = file.tellg();
		file.seekg(0, std::ios::beg);
		Data = (unsigned char*)malloc(Size);
		file.read((char*)Data, Size);
		stbtt_InitFont(&font_info, Data, 0);
		stbtt_GetFontVMetrics(&font_info, &yMax, &yMin, (int*)&lineGap);
		height = yMax - yMin;
		lineHeight = height + lineGap;
	}
	std::pair<FontSize, Shape> FontType::GetFont(const Character& character,const long long& pixel_height) {
		FontSize&& font_size{ 0 };
		char32_t code = character.GetUTF_32();
		stbtt_GetCodepointBox(&font_info, code, &font_size.xMin, &font_size.yMin, &font_size.xMax, &font_size.yMax);
		stbtt_GetCodepointHMetrics(&font_info, code, (int*)&font_size.advance, &font_size.xMin);
		float scale = this->height / pixel_height;
		font_size.scale = scale;
		font_size.lineGap = lineGap * scale;
		font_size.lineHeight = lineHeight * scale;
		font_size.height = pixel_height * scale;
		font_size.yMax *= scale;
		font_size.yMin *= scale;
		font_size.xMax *= scale;
		font_size.xMin *= scale;
		font_size.advance *= scale;
		font_size.width = font_size.xMax - font_size.xMin;

		stbtt_vertex* vertex_ptr = nullptr;
		unsigned int vercount = 0;
		vercount = stbtt_GetGlyphShape(&font_info, code, &vertex_ptr);
		std::vector<Point> vertexs;
		/*
		Vertex vertex;
		for (int i = 0; i < vercount; i++) {
			switch (vertex_ptr->type)
			{
			case STBTT_vmove:
				vertex = Vertex{ (float)vertex_ptr->x,(float)vertex_ptr->y,0 };
				break;
			case STBTT_vline:
				vertexs.push_back(vertex);
				vertex = Vertex{ (float)vertex_ptr->x,(float)vertex_ptr->y,0 };
				break;
			case STBTT_vcurve:
				vertexs.push_back(vertex);
				break;
			}
			vertex_ptr++;
		}
		Shape&& shape = Shape();
		return { font_size,shape };
		*/
	}
}