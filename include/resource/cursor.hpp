#ifndef BdUI_CURSOR
#define BdUI_CURSOR
#include "bitmap.hpp"

namespace BdUI {
	class Cursor : virtual protected Bitmap,virtual public Resource {
	protected:
		struct CursorFileHeader {
			std::int16_t Reserved = 0;//必须为0
			std::int16_t Type = 2;//为2 == 光标
			std::int16_t Count = 1;//光标个数
		}*cursor_fileheader = nullptr;
		struct CursorInfoHeader {
			std::int8_t Width;
			std::int8_t Height;
			std::int8_t ColorCount;
			std::int8_t Reserved;
			std::int16_t X_hotspot;
			std::int16_t Y_hotspot;
			std::int32_t ByteSize;
			std::int32_t Offset;//从文件头起算
		}*cursor_infoheader = nullptr;
#ifdef WIN32
		HCURSOR cursor = nullptr;
		void* And = nullptr;
#endif
	public:
		using Resource::operator=;
		using Resource::Resource;
		Cursor() {}
		~Cursor();
		virtual void SaveFile();
		virtual void Process();
		Cursor& operator=(const Cursor&);
#ifdef WIN32
		Cursor(HCURSOR);
		HCURSOR getIndex();
		operator HCURSOR();
		void setIndex(HCURSOR);
		Cursor& operator=(HCURSOR);
#endif 
	};
}
#endif