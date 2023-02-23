#ifndef BDUI_COLOR
#define BDUI_COLOR

namespace BdUI {
	struct RGBA {
		unsigned char R = 0;
		unsigned char G = 0;
		unsigned char B = 0;
		unsigned char A = 0;
	};
	struct YUVA {
		unsigned char Y = 0;
		unsigned char U = 0;
		unsigned char V = 0;
		unsigned char A = 0;
	};
	struct HSVA {
		unsigned char H = 0;
		unsigned char S = 0;
		unsigned char V = 0;
		unsigned char A = 0;
	};
	struct GLRGBA {
		float R;
		float G;
		float B;
		float A;
	};
	class Color {
	private:
		/*
		Eigen::Matrix3f RGB2YUV{
			0.299,0.587,0.114,
			-0.147,-0.289,-0.436,
			0.615,-0.515,-0.100,
		};
		Eigen::Matrix3f YUV2RGB{
			1,1.4,0,
			1,-0.39,-0.58,
			1,2.03,0,
		};*/

		enum ColorType {
			ColorType_RGBA = 0,
			ColorType_YUVA = 1,
			ColorType_HSVA = 2,
			Unknown = 3,
		}Type = Unknown;
		union ColorEncoding{
			RGBA rgba;
			YUVA yuva;
			HSVA hsva;
		public:
			ColorEncoding() : rgba({0,0,0,0}) {}
		}color;
	public:
		Color() {}
		Color(const RGBA&);
		Color(const YUVA&);
		Color(const HSVA&);
		Color(const GLRGBA&);

		RGBA GetRGBA() const;
		YUVA GetYUVA() const;
		HSVA GetHSVA() const;
		GLRGBA GetGLRGBA() const;

		void SetColor(const RGBA&);
		void SetColor(const YUVA&);
		void SetColor(const HSVA&);
		void SetColor(const GLRGBA&);

		Color& operator=(const RGBA&);
		Color& operator=(const YUVA&);
		Color& operator=(const HSVA&);
		Color& operator=(const GLRGBA&);

		Color& operator=(const Color&);
	};
}
#endif