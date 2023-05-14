#ifndef BDUI_UNIT
#define BDUI_UNIT

#include <cmath>
#include <map>
#include <regex>
#include "error.hpp"

#ifdef WIN32
#include <Windows.h>
#endif

namespace BdUI {
	enum class UnitType {
		inch,
		mm, 
		cm,
		dm,
		m,
		px,
	};
	class Unit{
	private:
		long double Number;
		UnitType Type;

		friend class Point;
		friend class Size;
		static const std::map<std::string, UnitType>& stringToUnitType();
		static const std::regex& unit_regex();
		static long double ToUnit(long double, UnitType, UnitType);
	public:
		Unit(const char*);
		Unit(const std::string&);
		Unit(long double, UnitType = UnitType::mm);
		Unit(const Unit& unit) : Number(unit.Number),Type(unit.Type) {}

		UnitType GetType() const;
		long double GetData(UnitType) const;
		void SetData(long double, UnitType);

		operator long double();

		Unit& operator+(Unit&);
		Unit& operator-(Unit&);
		Unit& operator*(Unit&);
		Unit& operator/(Unit&);
		Unit& operator+=(Unit&);
		Unit& operator-=(Unit&);
		Unit& operator*=(Unit&);
		Unit& operator/=(Unit&);
	};
}
#endif