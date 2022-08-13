#ifndef BDUI_COLOR
#define BDUI_COLOR

#include <cmath>

namespace BdUI {
	struct RGB {
		unsigned char R = 0;
		unsigned char G = 0;
		unsigned char B = 0;
	};
	struct YUV {
		unsigned char Y = 0;
		unsigned char U = 0;
		unsigned char V = 0;
	};
	struct HSV {
		unsigned H : 9;
		unsigned S : 7;
		unsigned V : 7;
	};
	class Color {
	private:
		enum ColorType {
			ColorType_RGB = 0,
			ColorType_YUV = 1,
			ColorType_HSV = 2,
			Unknown = 3,
		}Type = Unknown;
		union ColorEncoding{
			RGB rgb;
			YUV yuv;
			HSV hsv;
		public:
			ColorEncoding() : rgb({0,0,0}) {}
		}color;
		unsigned char Alpha = 0;
	public:
		Color() {}
		Color(const RGB&);
		Color(const YUV&);
		Color(const HSV&);
		Color(const RGB&, const unsigned char&);
		Color(const YUV&, const unsigned char&);
		Color(const HSV&, const unsigned char&);

		unsigned char GetAlpha();
		RGB GetRGB();
		YUV GetYUV();
		HSV GetHSV();

		void SetAlpha(const unsigned char&);
		void SetColor(const RGB&);
		void SetColor(const YUV&);
		void SetColor(const HSV&);

		Color& operator=(const RGB&);
		Color& operator=(const YUV&);
		Color& operator=(const HSV&);
	};
}
#endif