#include "graph/unit.hpp"

namespace BdUI {
	const std::map<std::string, UnitType>& Unit::stringToUnitType() {
		static const std::map<std::string, UnitType> map{
			{"m",UnitType::m},
			{"dm",UnitType::dm},
			{"cm",UnitType::cm},
			{"mm",UnitType::mm},
			{"in",UnitType::inch},
			{"px",UnitType::px},
		};
		return map;
	}
	const std::regex& Unit::unit_regex(){
		static const std::regex r("([0-9\\.\\-]+)([a-z]+)");
		return r;
	}

	Unit::Unit(const std::string& str) {
		if (std::smatch match; std::regex_match(str, match, unit_regex()))
			if (auto iter = stringToUnitType().find(match[2].str()); iter != stringToUnitType().end()) {
				Number = std::stold(match[1].str());
				Type = iter->second;
			}
			else throw error::Unit::string_error();
		else throw error::Unit::string_error();
	}

	long double Unit::ToUnit(long double number, UnitType u_1, UnitType u_2) {
		long double&& pow_1 = 0;
		long double&& pow_2 = 0;
		switch (u_1)
		{
		case BdUI::UnitType::inch:
			pow_1 = 25.4;
			break;
		case BdUI::UnitType::mm:
			pow_1 = 1;
			break;
		case BdUI::UnitType::cm:
			pow_1 = 10;
			break;
		case BdUI::UnitType::dm:
			pow_1 = 100;
			break;
		case BdUI::UnitType::m:
			pow_1 = 1000;
			break;
		case BdUI::UnitType::px:
			if (u_2 == UnitType::px) return number;
			else throw error::Unit::unit_error();
			break;
		default:
			break;
		}
		switch (u_2)
		{
		case BdUI::UnitType::inch:
			pow_2 = 25.4;
			break;
		case BdUI::UnitType::mm:
			pow_2 = 1;
			break;
		case BdUI::UnitType::cm:
			pow_2 = 10;
			break;
		case BdUI::UnitType::dm:
			pow_2 = 100;
			break;
		case BdUI::UnitType::m:
			pow_2 = 1000;
			break;
		}
		return number * (pow_1 / pow_2);
	}

	Unit::Unit(const char* ch) { *this = Unit(std::string(ch)); }

	long double Unit::GetData(UnitType u) const { return ToUnit(Number, Type, u); }
	UnitType Unit::GetType() const { return Type; }

	void Unit::SetData(long double n, UnitType u) {
		Number = n;
		Type = u;
	}

	Unit::operator long double() { return Number; }

	Unit& Unit::operator+(Unit& u) {
		Number += ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator-(Unit& u) {
		Number -= ToUnit(u.Number,u.Type,Type);
		return *this;
	}
	Unit& Unit::operator*(Unit& u) {
		Number *= ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator/(Unit& u) {
		Number /= ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator+=(Unit& u) {
		Number += ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator-=(Unit& u) {
		Number -= ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator*=(Unit& u) {
		Number *= ToUnit(u.Number, u.Type, Type);
		return *this;
	}
	Unit& Unit::operator/=(Unit& u) {
		Number /= ToUnit(u.Number, u.Type, Type);
		return *this;
	}
}