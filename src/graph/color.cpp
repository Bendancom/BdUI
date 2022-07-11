#include "graph/color.hpp"

namespace BdUI {
	Color::Color(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType_RGB;
	}
	Color::Color(const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType_YUV;
	}
	Color::Color(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType_HSV;
	}
	Color::Color(const RGB& rgb, const unsigned char& a) {
		color.rgb = rgb;
		Alpha = a;
		Type = ColorType_RGB;
	}
	Color::Color(const YUV& yuv, const unsigned char& a) {
		color.yuv = yuv;
		Alpha = a;
		Type = ColorType_YUV;
	}
	Color::Color(const HSV& hsv, const unsigned char& a) {
		color.hsv = hsv;
		Alpha = a;
		Type = ColorType_HSV;
	}

	unsigned char Color::GetAlpha() {
		return Alpha;
	}
	RGB Color::GetRGB() {
		if (Type == ColorType_RGB) return color.rgb;
		else {
			RGB&& temp{ 0,0,0 };
			switch (Type) {
				case ColorType_YUV: {
					temp.R = color.yuv.Y + 1.403 * (color.yuv.V - 128);
					temp.G = color.yuv.Y - 0.343 * (color.yuv.U - 128) - 0.714 * (color.yuv.V - 128);
					temp.B = color.yuv.Y + 1.770 * (color.yuv.U - 128);
					break;
				}
				case ColorType_HSV: {
					float R, G, B;

					float H = float(color.hsv.H) / 60;
					float S = float(color.hsv.S) / 100;
					float V = float(color.hsv.V) / 100;
					if (color.hsv.S == 0) {
						R = G = B = V;
					}
					else {
						unsigned char i = (int)H;
						float C = H - i;

						float m = V - C;
						float X = V * (1 - S);
						float Y = V * (1 - S * C);
						float Z = V * (1 - S * (1 - C));

						switch (i) {
							case 0: R = V; G = Z; B = X; break;
							case 1: R = Y; G = V; B = X; break;
							case 2: R = X; G = V; B = Z; break;
							case 3: R = X; G = Y; B = V; break;
							case 4: R = Z; G = X; B = V; break;
							case 5: R = V; G = X; B = Y; break;
						}
					}
					temp.R = R * 255;
					temp.G = G * 255;
					temp.B = B * 255;
					break;
				}
			}
			return temp;
		}
	}
	YUV Color::GetYUV() {
		if (Type == ColorType_YUV) return color.yuv;
		else {
			YUV&& yuv{ 0,0,0 };
			switch (Type) {
				case ColorType_RGB:{
					yuv.Y = 0.299 * color.rgb.R + 0.587 * color.rgb.G + 0.114 * color.rgb.B;
					yuv.U = 0.437 * color.rgb.B - 0.147 * color.rgb.R - 0.289 * color.rgb.G;
					yuv.V = 0.615 * color.rgb.R - 0.515 * color.rgb.G + 0.100 * color.rgb.B;
					break;
				}
				case ColorType_HSV:{
					RGB temp;
					float R, G, B;

					float H = float(color.hsv.H) / 60;
					float S = float(color.hsv.S) / 100;
					float V = float(color.hsv.V) / 100;
					if (color.hsv.S == 0) {
						R = G = B = V;
					}
					else {
						unsigned char i = (int)H;
						float C = H - i;

						float m = V - C;
						float X = V * (1 - S);
						float Y = V * (1 - S * C);
						float Z = V * (1 - S * (1 - C));

						switch (i) {
						case 0: R = V; G = Z; B = X; break;
						case 1: R = Y; G = V; B = X; break;
						case 2: R = X; G = V; B = Z; break;
						case 3: R = X; G = Y; B = V; break;
						case 4: R = Z; G = X; B = V; break;
						case 5: R = V; G = X; B = Y; break;
						}
					}
					temp.R = R * 255;
					temp.G = G * 255;
					temp.B = B * 255;

					yuv.Y = 0.299 * temp.R + 0.587 * temp.G + 0.114 * temp.B;
					yuv.U = 0.437 * temp.B - 0.147 * temp.R - 0.289 * temp.G;
					yuv.V = 0.615 * temp.R - 0.515 * temp.G + 0.100 * temp.B;
					break;
				}
			}
			return yuv;
		}
	}
	HSV Color::GetHSV() {
		if (Type == ColorType_HSV) return color.hsv;
		else {
			HSV&& hsv{ 0,0,0 };
			switch (Type) {
				case ColorType_RGB:{
					float R = float(color.rgb.R) / 255;
					float G = float(color.rgb.G) / 255;
					float B = float(color.rgb.B) / 255;
					float max = (R >= G) ? (R >= B ? R : B) : (G >= B ? G : B);
					float min = (R <= G) ? (R <= B ? R : B) : (G <= B ? G : B);

					hsv.V = max * 100;

					if (max == 0) hsv.S = 0;
					else hsv.S = (1 - min / max) * 100;

					float h;
					if (max == min)
						h = 0;
					else if (max == R && G >= B)
						h = 60 * ((G - B) / (max - min));
					else if (max == R && G < B)
						h = 60 * ((G - B) / (max - min)) + 360;
					else if (max == G)
						h = 60 * ((B - R) / (max - min)) + 120;
					else if (max == B)
						h = 60 * ((R - G) / (max - min)) + 240;
					hsv.H = h;
					break;
				}
				case ColorType_YUV:{
					float R = float(int(color.yuv.Y + 1.403 * (color.yuv.V - 128))) / 255;
					float G = float(int(color.yuv.Y - 0.343 * (color.yuv.U - 128) - 0.714 * (color.yuv.V - 128))) / 255;
					float B = float(int(color.yuv.Y + 1.770 * (color.yuv.U - 128))) / 255;

					float max = (R >= G) ? (R >= B ? R : B) : (G >= B ? G : B);
					float min = (R <= G) ? (R <= B ? R : B) : (G <= B ? G : B);

					hsv.V = max * 100;

					if (max == 0) hsv.S = 0;
					else hsv.S = (1 - min / max) * 100;

					float h;
					if (max == min)
						h = 0;
					else if (max == R && G >= B)
						h = 60 * ((G - B) / (max - min));
					else if (max == R && G < B)
						h = 60 * ((G - B) / (max - min)) + 360;
					else if (max == G)
						h = 60 * ((B - R) / (max - min)) + 120;
					else if (max == B)
						h = 60 * ((R - G) / (max - min)) + 240;
					hsv.H = h;
					break;
				}
			}
			return hsv;
		}
	}

	void Color::SetAlpha(const unsigned char& a) {
		Alpha = a;
	}
	void Color::SetColor(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType_RGB;
	}
	void Color::SetColor(const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType_YUV;
	}
	void Color::SetColor(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType_HSV;
	}

	Color& Color::operator=(const RGB& rgb) {
		color.rgb = rgb;
		Type = ColorType_RGB;
		return *this;
	}
	Color& Color::operator = (const YUV& yuv) {
		color.yuv = yuv;
		Type = ColorType_YUV;
		return *this;
	}
	Color& Color::operator=(const HSV& hsv) {
		color.hsv = hsv;
		Type = ColorType_HSV;
		return *this;
	}
}