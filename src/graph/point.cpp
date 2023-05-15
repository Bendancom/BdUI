#include "graph/point.hpp"

namespace BdUI {
	std::array<long double, 2> Point::GetData(UnitType type) const { 
		return { Unit(X,Type).GetData(type),Unit(Y,Type).GetData(type)};
	}
	std::array<long double, 2> Point::GetPixel(const Monitor& monitor) const {
		if (Type == UnitType::px) return {X,Y};
		else {
			std::array<long double, 2> dpi = monitor.GetDPI();
			return { Unit::ToUnit(X,Type,UnitType::inch) * dpi[0],Unit::ToUnit(Y,Type,UnitType::inch) * dpi[1] };
		}
	}
	UnitType Point::GetType() const { return Type; }

	void Point::SetData(long double x,long double y, UnitType u) {
		X = x;
		Y = y;
		Type = u;
	}
	void Point::SetData(const std::array<long double, 2>& list, UnitType u) {
		X = list[0];
		Y = list[1];
		Type = u;
	}

	long double Point::operator[](unsigned int pos) {
		if (pos >= 2) return false;
		else if (pos == 1)return Y;
		else return X;
	}
	Point& Point::operator+(Point& p) {
		X += Unit::ToUnit(p.X,p.Type,Type);
		Y += Unit::ToUnit(p.Y, p.Type, Type);
		return *this;
	}
	Point& Point::operator+=(Point& p) {
		X += Unit::ToUnit(p.X, p.Type, Type);
		Y += Unit::ToUnit(p.Y, p.Type, Type);
		return *this;
	}
	Point& Point::operator-(Point& p) {
		X -= Unit::ToUnit(p.X, p.Type, Type);
		Y -= Unit::ToUnit(p.Y, p.Type, Type);
		return *this;
	}
	Point& Point::operator-=(Point& p) {
		X -= Unit::ToUnit(p.X, p.Type, Type);
		Y -= Unit::ToUnit(p.Y, p.Type, Type);
		return *this;
	}
}