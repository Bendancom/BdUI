#include <character.hpp>

namespace BdUI {
	Character::Character(wchar_t c) {
		unicode = WCharToUnicode(c);
	}

	Character::Character(char c) {
		unicode.Plane = 0;
		unicode.Block_ID = 0;
		unicode.Block = c;
	}
	Character::Character(char8_t c) {
		unicode.Plane = 0;
		unicode.Block_ID = 0;
		unicode.Block = c;
	}
	Character::Character(char16_t c) {
		unicode.Plane = 0;
		unicode.Block = c & 0xFF;
		unicode.Block_ID = c >> 8;
	}
	Character::Character(char32_t c) {
		unicode.Block = c & 0xFF;
		unicode.Block_ID = (c >> 8) & 0xFF;
		unicode.Plane = (c >> 16) & 0xFF;
	}

	Character::Character(std::array<char,2> a) {
		wchar_t* w = new wchar_t;
		if ((unsigned char)a[0] >= 0x80) {
			std::mbstate_t mbstate{};
			const char data_from[] = { a[0], a[1] };
			const char* data_from_end = data_from + 2;
			const char* data_from_next = 0;
			wchar_t* data_to_end = w + 1;
			wchar_t* data_to_next = 0;
			auto result = std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale("")).in(
				mbstate, data_from, data_from_end, data_from_next, w, data_to_end, data_to_next);
		}
		else *w = a[0];
		unicode = WCharToUnicode(*w);
		delete w;
	}
	Character::Character(std::array<char8_t, 4> a) {
		if ((a[0] >> 7) == 0) {
			unicode.Block = a[0];
			unicode.Block_ID = 0;
			unicode.Plane = 0;
		}
		else{
			int j = 3;
			std::array<char8_t,4> c = a;
			for (; j >= 0; j--)
				if (c[j] > 0) break;
			c[j] <<= j;
			c[j] >>= j;
			for (int i = j - 1; i >= 0; i--)
			{
				c[i] <<= 1;
				c[i] >>= 1;
			}
			std::uint32_t u = 0;
			for (int i = 0; i <= j; i++) {
				u |= (std::uint32_t)c[i] >> 2*i;
			}
			unicode.Block = u & 0xFF;
			unicode.Block_ID = (u >> 8) & 0xFF;
			unicode.Plane = u >> 16;
		}
	}
	Character::Character(std::array<char16_t, 2> a) {
		if (a[1] == 0) {
			unicode.Plane = 0;
			unicode.Block = a[0] & 0xFF;
			unicode.Block_ID = a[0] >> 8;
		}
		else {
			std::uint16_t u[3];
			u[0] = (a[0] << 6) >> 6;
			u[1] = (a[1] << 6) >> 6;
			unicode.Block = u[0] & 0xFF;
			unicode.Block_ID = (u[0] >> 8) | ((u[1] & 0b00111111) << 2);
			unicode.Plane = (u[1] >> 6) + 1;
		}
	}

	std::array<char, 2> Character::GetChar() const{
		if (sizeof(wchar_t) == 2) {
			auto utf16 = GetUTF_16();
			if (utf16[1] == 0)
			{
				if (utf16[0] < 0x80) {
					return { (char)utf16[0] };
				}
				else {
					std::mbstate_t mbstate{};
					wchar_t w = utf16[0];
					const wchar_t* data_from = &w;
					const wchar_t* data_from_end = &w + 1;
					const wchar_t* data_from_next = 0;
					char* data_to = new char[2];
					char* data_to_end = data_to + 2;
					char* data_to_next = 0;
					std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale("")).out(
						mbstate, data_from, data_from_end, data_from_next, data_to, data_to_end, data_to_next);
					std::array<char, 2>&& array = { data_to[0],data_to[1]};
					delete[] data_to;
					return array;
				}
			}
			else throw error::Function::CarryOut_Faild("Can't Transform the Unicode to ANSI");
		}
		else {
			char32_t utf32 = GetUTF_32();
			if (utf32 < 0x80) {
				return { (char)utf32 };
			}
			else {
				if (utf32 <= 0xFFFF) {
					std::mbstate_t mbstate{};
					wchar_t w = utf32;
					const wchar_t* data_from = &w;
					const wchar_t* data_from_end = &w + 1;
					const wchar_t* data_from_next = 0;
					char* data_to = new char[2];
					char* data_to_end = data_to + 2;
					char* data_to_next = 0;
					std::use_facet<std::codecvt<wchar_t, char, std::mbstate_t>>(std::locale("")).out(
						mbstate, data_from, data_from_end, data_from_next, data_to, data_to_end, data_to_next);
					std::array<char, 2>&& array = { data_to[0],data_to[1] };
					delete[] data_to;
				}
				else throw error::Function::CarryOut_Faild("Can't Transform the Unicode to ANSI");
			}
		}
		
	}

	std::array<char8_t, 4> Character::GetUTF_8() const{
		if (unicode.Block_ID > 0 || unicode.Plane > 0 || unicode.Block >= 128) {
			char8_t utf8[4] = {0};
			unsigned char bit = 0;
			for (; bit < 8; bit++)
				if (unicode.Block_ID >> bit == 0) break;
			unsigned char utf8_long;
			if (bit + 8 <= 11) utf8_long = 2;
			else if (unicode.Plane == 0) utf8_long = 3;
			else utf8_long = 4;
			switch (utf8_long)
			{
			case 2: {
				utf8[0] = unicode.Block & 0b00111111 | 0b10000000;
				utf8[1] = (unicode.Block >> 6) | ((unicode.Block_ID & 0b111) << 2) | 0b11000000;
				break;
			}
			case 3: {
				utf8[0] = unicode.Block & 0b00111111 | 0b10000000;
				utf8[1] = (unicode.Block >> 6) | ((unicode.Block_ID & 0b00001111) << 2) | 0b10000000;
				utf8[2] = (unicode.Block_ID >> 4) | 0b11100000;
				break;
			}
			case 4: {
				utf8[0] = unicode.Block & 0b00111111 | 0b10000000;
				utf8[1] = (unicode.Block >> 6) | ((unicode.Block_ID & 0b00001111) << 2) | 0b10000000;
				utf8[2] = (unicode.Block_ID >> 4) | ((unicode.Plane & 0b11) << 4) | 0b10000000;
				utf8[3] = ((unicode.Plane >> 2) & 0b111) | 0b11110000;
				break;
			}
			}
			return { utf8[0],utf8[1],utf8[2],utf8[3] };
		}
		else return { unicode.Block,0,0,0 };
	}

	std::array<char16_t, 2> Character::GetUTF_16() const{
		if (unicode.Plane > 0) {
			char16_t utf16[2] = { 0 };
			utf16[0] = unicode.Block | (((char16_t)unicode.Block_ID) & 0b11) | 0b1101110000000000;
			utf16[1] = (unicode.Block_ID >> 2) | ((unicode.Plane - 1) << 2) | 0b1101100000000000;
			return { utf16[0],utf16[1] };
		}
		else return { char16_t((char16_t)unicode.Block | (char16_t)unicode.Block_ID << 8),0 };
	}

	char32_t Character::GetUTF_32() const{
		return ((char32_t)unicode.Plane << 16) | ((char32_t)unicode.Block_ID << 8) | unicode.Block;
	}

	Unicode Character::GetUnicode() const{
		return unicode;
	}
	wchar_t Character::GetWchar() const{
		if (sizeof(wchar_t) == 2) {
			return (wchar_t)unicode.Block_ID << 8 | unicode.Block;
		}
		else {
			return (wchar_t)unicode.Plane << 16 | (wchar_t)unicode.Block_ID << 8 | unicode.Block;
		}
	}

	bool Character::operator==(const Character& c) const {
		if (unicode.Block != c.unicode.Block) return false;
		if (unicode.Block_ID != c.unicode.Block_ID) return false;
		if (unicode.Plane != c.unicode.Plane) return false;
		return true;
	}
	bool Character::operator!=(const Character& c) const {
		if (unicode.Block != c.unicode.Block) return true;
		if (unicode.Block_ID != c.unicode.Block_ID) return true;
		if (unicode.Plane != c.unicode.Plane) return true;
		return false;
	}

	Unicode Character::WCharToUnicode(const wchar_t& w) {
		Unicode u;
		if (sizeof(wchar_t) == 4) {
			u.Block = w & 0xFF;
			u.Block_ID = (w & 0xFF00) >> 8;
			u.Plane = (w & 0xFF0000) >> 16;
		}
		else {
			u.Plane = 0;
			u.Block = w & 0xFF;
			u.Block_ID = (w & 0xFF00) >> 8;
		}
		return u;
	}

}