#ifndef BdUI_CURSOR
#define BdUI_CURSOR
#include <resource/image/icon.hpp>
#include <graph/point.hpp>

namespace BdUI {
	class Cursor : protected Icon,virtual public Resource {
	private:
#ifdef WIN32
		HCURSOR cursor = nullptr;
		void CreateHCursor();
#endif
	protected:
		typedef IconDir CursorFileHeader;
		struct CursorInfoHeader {
			std::int8_t Width;
			std::int8_t Height;
			std::int8_t ColorCount;
			std::int8_t Reserved;
			std::int16_t X_hotspot;
			std::int16_t Y_hotspot;
			std::int32_t ByteSize;
			std::int32_t Offset;//从文件头起算
		};
		short _X;
		short _Y;
	public:
		const short& X = _X;
		const short& Y = _Y;

		Cursor() {}
		Cursor(const std::filesystem::path&);
		Cursor(const Cursor&);
		~Cursor();

		void SetHotPoint(long long X,long long Y);

		virtual void OpenFile(const std::filesystem::path&);
		virtual void SaveToFile();
		virtual std::pair<void*,unsigned long long> SaveToMemory();
		virtual void LoadFromFile();
		virtual void LoadFromMemory(void*, unsigned long long);

		Cursor& Resize(long long Width, long long Height);
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