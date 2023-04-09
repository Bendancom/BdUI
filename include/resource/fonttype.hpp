#ifndef BDUI_FONTTYPE
#define BDUI_FONTTYPE

#define STB_TRUETYPE_IMPLEMENTATION
#include <resource/stb/stb_truetype.h>
#include <resource/resource.hpp>
#include <graph/point.hpp>
#include <character.hpp>
#include <graph/shape.hpp>

namespace BdUI {
	struct FontSize {
		int xMax;
		int xMin;
		int yMin;
		int yMax;
		unsigned int width;
		unsigned int height;
		unsigned int advance;
		unsigned int lineGap;
		unsigned int lineHeight;
		float scale;
	};

	class FontType : public Resource {
	private:
		stbtt_fontinfo font_info;
	protected:
		int yMin;
		int yMax;
		unsigned int height;
		unsigned int lineGap;
		unsigned int lineHeight;
	public:
		FontType() {}
		FontType(const std::filesystem::path&);
		FontType(const FontType&);

		std::pair<FontSize, Shape> GetFont(const Character&,const long long& pixel_height);

		virtual void OpenFile(const std::filesystem::path& filename);
		virtual void LoadFromFile();
		virtual void LoadFromMemory(void* pos, unsigned long long size);
		virtual void SaveToFile();
		virtual std::pair<void*, unsigned long long> SaveToMemory();
	};
}

#endif