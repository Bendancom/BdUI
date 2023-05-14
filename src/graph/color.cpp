#include "graph/color.hpp"

namespace BdUI {
	Color::Color(const RGBA& rgb) {
		color.rgba = rgb;
		Type = ColorType_RGBA;
	}
	Color::Color(const YUVA& yuv) {
		color.yuva = yuv;
		Type = ColorType_YUVA;
	}
	Color::Color(const HSVA& hsv) {
		color.hsva = hsv;
		Type = ColorType_HSVA;
	}
	Color::Color(const GLRGBA& glrgba) {
		SetColor(glrgba);
	}

	RGBA Color::GetRGBA() const {
		if (Type == ColorType_RGBA) return color.rgba;
		else {
			switch (Type) {
				case ColorType_YUVA: {
					unsigned char&& R = color.yuva.Y + 1.4 * color.yuva.U;
					unsigned char&& G = color.yuva.Y - 0.39 * color.yuva.U - 0.58 * color.yuva.V;
					unsigned char&& B = color.yuva.Y + 2.03 * color.yuva.U;
					return RGBA{ R,G,B,color.yuva.A };
				}
				case ColorType_HSVA: {
					float R, G, B;

					float H = float(color.hsva.H) / 255 * 100;
					float S = float(color.hsva.S) / 255 * 100;
					float V = float(color.hsva.V) / 255 * 100;

					if (color.hsva.S == 0) {
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
					return RGBA{ (unsigned char)(R * 255),(unsigned char)(G * 255),(unsigned char)(B * 255),color.hsva.A };
				}
			}
		}
	}
	YUVA Color::GetYUVA() const {
		if (Type == ColorType_YUVA) return color.yuva;
		else {
			switch (Type) {
				case ColorType_RGBA:{
					unsigned char&& Y = 0.299 * color.rgba.R + 0.587 * color.rgba.G + 0.114 * color.rgba.B;
					unsigned char&& U = -0.147 * color.rgba.R - 0.289 * color.rgba.G - 0.436 * color.rgba.B;
					unsigned char&& V = 0.615 * color.rgba.R - 0.515 * color.rgba.G - 0.100 * color.rgba.B;
					return YUVA{ Y,U,V,color.rgba.A };
				}
				case ColorType_HSVA:{
					float R, G, B;

					float H = float(color.hsva.H) / 256 * 100;
					float S = float(color.hsva.S) / 256 * 100;
					float V = float(color.hsva.V) / 256 * 100;
					if (color.hsva.S == 0) {
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
					unsigned char&& Y = 0.299 * int(R * 255) + 0.587 * int(G * 255) + 0.114 * int(B * 255);
					unsigned char&& U = -0.147 * int(R * 255) - 0.289 * int(G * 255) - 0.436 * int(B * 255);
					unsigned char&& _V = 0.615 * int(R * 255) - 0.515 * int(G * 255) - 0.100 * int(B * 255);
					return YUVA{ Y,U,_V,color.hsva.A };
				}
			}
		}
	}
	HSVA Color::GetHSVA() const {
		if (Type == ColorType_HSVA) return color.hsva;
		else {
			switch (Type) {
				case ColorType_RGBA:{
					HSVA hsva = {0,0,0,color.rgba.A};
					float R = float(color.rgba.R) / 255;
					float G = float(color.rgba.G) / 255;
					float B = float(color.rgba.B) / 255;
					float max = (R >= G) ? (R >= B ? R : B) : (G >= B ? G : B);
					float min = (R <= G) ? (R <= B ? R : B) : (G <= B ? G : B);

					hsva.V = max * 100;

					if (max == 0) hsva.S = 0;
					else hsva.S = (1 - min / max) * 100;

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
					hsva.H = h;
					return hsva;
				}
				case ColorType_YUVA:{
					HSVA hsva = { 0,0,0,color.yuva.A };
					unsigned char&& _R = color.yuva.Y + 1.4 * color.yuva.U;
					unsigned char&& _G = color.yuva.Y - 0.39 * color.yuva.U - 0.58 * color.yuva.V;
					unsigned char&& _B = color.yuva.Y + 2.03 * color.yuva.U;

					float R = (float)(_R) / 255;
					float G = (float)(_G) / 255;
					float B = (float)(_B) / 255;

					float max = (R >= G) ? (R >= B ? R : B) : (G >= B ? G : B);
					float min = (R <= G) ? (R <= B ? R : B) : (G <= B ? G : B);

					hsva.V = max * 100;

					if (max == 0) hsva.S = 0;
					else hsva.S = (1 - min / max) * 100;

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
					hsva.H = h;
					return hsva;
				}
			}
		}
	}
	GLRGBA Color::GetGLRGBA() const {
		RGBA rgba = GetRGBA();
		return GLRGBA{ float(rgba.R) / 255,float(rgba.G) / 255,float(rgba.B) / 255,float(rgba.A) / 255 };
	}

	void Color::SetColor(const RGBA& rgb) {
		color.rgba = rgb;
		Type = ColorType_RGBA;
	}
	void Color::SetColor(const YUVA& yuv) {
		color.yuva = yuv;
		Type = ColorType_YUVA;
	}
	void Color::SetColor(const HSVA& hsv) {
		color.hsva = hsv;
		Type = ColorType_HSVA;
	}
	void Color::SetColor(const GLRGBA& glrgba) {
		SetColor(RGBA{ (unsigned char)(glrgba.R * 255),(unsigned char)(glrgba.G * 255),(unsigned char)(glrgba.B * 255),(unsigned char)(glrgba.A * 255) });
	}

	Color& Color::operator=(const RGBA& rgb) {
		color.rgba = rgb;
		Type = ColorType_RGBA;
		return *this;
	}
	Color& Color::operator = (const YUVA& yuv) {
		color.yuva = yuv;
		Type = ColorType_YUVA;
		return *this;
	}
	Color& Color::operator=(const HSVA& hsv) {
		color.hsva = hsv;
		Type = ColorType_HSVA;
		return *this;
	}
	Color& Color::operator=(const GLRGBA& glrgba) {
		SetColor(glrgba);
		return *this;
	}
	Color& Color::operator=(const Color& c) {
		color = c.color;
		Type = c.Type;
		return *this;
	}
}