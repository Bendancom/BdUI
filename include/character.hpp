#ifndef BDUI_CHARACTER
#define BDUI_CHARACTER

#include <array>
#include <locale>
#include <error.hpp>

namespace BdUI {
	struct Unicode {
	public:
		std::uint8_t Plane;// ·¶Î§Îª[0,16]
		std::uint8_t Block_ID;
		std::uint8_t Block;
		std::weak_ordering operator<=>(const Unicode&) const;
	};
	class Character {
	private:
		Unicode unicode;
		Unicode WCharToUnicode(const wchar_t&);
	public:
		Character(const char*);

		Character(wchar_t);
		Character(char);
		Character(char8_t);
		Character(char16_t);
		Character(char32_t);
		Character(const int&) = delete;
		Character(const short&) = delete;
		Character(Unicode u) : unicode(u){}
		Character(std::array<char,2>);
		Character(std::array<char8_t,4>);
		Character(std::array<char16_t,2>);
		
		bool operator==(const Character&) const;
		bool operator!=(const Character&) const;
		std::weak_ordering operator<=>(const Character&) const;

		std::array<char8_t, 4> GetUTF_8() const;
		std::array<char16_t, 2> GetUTF_16() const;
		char32_t GetUTF_32() const;
		wchar_t GetWchar() const;
		Unicode GetUnicode() const;
		std::array<char, 2> GetChar() const;
	};
}

#endif