#include "graph/color.hpp"

namespace BdUI {
	Color::Color(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType::RGB;
	}
	Color::Color(const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType::YUV;
	}
	Color::Color(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType::HSV;
	}
	Color::Color(const RGB& rgb, const unsigned char& a) {
		color.rgb = rgb;
		Alpha = a;
		Type = ColorType::RGB;
	}
	Color::Color(const YUV& yuv, const unsigned char& a) {
		color.yuv = yuv;
		Alpha = a;
		Type = ColorType::YUV;
	}
	Color::Color(const HSV& hsv, const unsigned char& a) {
		color.hsv = hsv;
		Alpha = a;
		Type = ColorType::HSV;
	}

	unsigned char Color::GetAlpha() {
		return Alpha;
	}
	Color::RGB Color::GetRGB() {
		if (Type == ColorType::RGB) return color.rgb;
		else {
			RGB&& temp{0,0,0};
			switch (Type) {
				case ColorType::YUV: {
					temp.R = color.yuv.Y + 1.403 * (color.yuv.V - 128);
					temp.G = color.yuv.Y - 0.343 * (color.yuv.U - 128) - 0.714 * (color.yuv.V - 128);
					temp.B = color.yuv.Y + 1.770 * (color.yuv.U - 128);
					break;
				}
				case ColorType::HSV: {
					char H = color.hsv.H / 60;
					float S = float(color.hsv.S) / 100;
					float V = float(color.hsv.V) / 100;
					if (color.hsv.S == 0) {
						return RGB{ (unsigned char)(V * 255),(unsigned char)(V * 255),(unsigned char)(V * 255) };
					}
					else {
						float C = V * S;
						float X = C * (1 - (H % 2) - 1);
						float m = V - C;
						unsigned char i = H;
						switch (i) {
							case 0: {

							}
						}
					}
					break;
				}
			}
			return temp;
		}
	}

	void Color::SetAlpha(const unsigned char& a) {
		Alpha = a;
	}
	void Color::SetColor(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType::RGB;
	}
	void Color::SetColor(const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType::YUV;
	}
	void Color::SetColor(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType::HSV;
	}

	Color& Color::operator=(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType::RGB;
	}
	Color& Color::operator = (const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType::YUV;
	}
	Color& Color::operator=(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType::HSV;
	}
}