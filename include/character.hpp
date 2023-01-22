#ifndef BDUI_CHARACTER
#define BDUI_CHARACTER

#include <array>
#include <locale>
#include <error.hpp>

namespace BdUI {
	struct Unicode {
		std::uint8_t Plane;// ·¶Î§Îª[0,16]
		std::uint8_t Block_ID;
		std::uint8_t Block;
	};
	class Character {
	private:
		Unicode unicode;
		Unicode WCharToUnicode(const wchar_t&);
	public:
		explicit Character(wchar_t);
		explicit Character(char);
		explicit Character(char8_t);
		explicit Character(char16_t);
		explicit Character(char32_t);
		Character(std::array<char,2>);
		Character(std::array<char8_t,4>);
		Character(std::array<char16_t,2>);
		

		std::array<char8_t, 4> GetUTF_8();
		std::array<char16_t, 2> GetUTF_16();
		char32_t GetUTF_32();
		wchar_t GetWchar();
		Unicode GetUnicode();
		std::array<char, 2> GetChar();
	};
}

#endif