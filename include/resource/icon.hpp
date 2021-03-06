#ifndef BDUI_ICON
#define BDUI_ICON
#include "bitmap.hpp"

namespace BdUI {
	class Icon : virtual public Resource,protected Bitmap {
	private:
#ifdef WIN32
		HICON icon = nullptr;
#endif
	protected:
		struct IconFileHeader {
			std::int16_t Reserved = 0;
			std::int16_t Type = 1;
			std::int16_t Count = 1;
		}*icon_fileheader = nullptr;
		struct IconInfoHeader {
			std::int8_t Width;
			std::int8_t Height;
			std::int8_t ColorCount;
			std::int8_t Reserved = 0;
			std::int16_t Planes;
			std::int16_t BitCount;
			std::int32_t ByteInRes;
			std::int32_t Offset;
		}*icon_infoheader = nullptr;
		void *icon_And = nullptr;
	public:
		using Resource::Resource;
		Icon(){}
		~Icon() {
			if (Source == Where::Memory) delete icon_fileheader;
		}
		virtual void Process();
		virtual void SaveFile();
		Icon(const Icon&);
		Icon& operator=(const Icon&);
#ifdef WIN32
		Icon(HICON);
		Icon& operator=(HICON);
		operator HICON();
		HICON getIndex();
		void setIndex(HICON);
#endif
	};
}
#endif