#ifndef BDUI_META
#define BDUI_META

#include <string>
#include <list>
#include <unordered_map>
#include <vector>

namespace BdUI{

	class Meta {
	private:
		static std::unordered_map<std::string, Meta*> MetaMap;
		enum {
			Plus = 0,
			Minus = 1,
			Multiply = 2,
			Division = 3,
		}Symbol = Plus;
		double coefficient = 1;
		char* variable; // '\0' is the end
		Meta* pow = nullptr;
		std::string suffix;
		std::list<Meta*> Content;
		std::string Name;
	public:
		Meta(std::string LATEX_expression);

		Meta& operator+(const Meta&);
		Meta& operator-(const Meta&);
		Meta& operator*(const Meta&);
		Meta& operator/(const Meta&);
		Meta& operator+=(const Meta&);
		Meta& operator-=(const Meta&);
		Meta& operator*=(const Meta&);
		Meta& operator/=(const Meta&);
	};
}
#endif