#include "graph/point.hpp"

namespace BdUI {
	std::array<long double, 2> Point::GetData(UnitType type) const { 
		return { Unit(X,Type).GetData(type),Unit(Y,Type).GetData(type)};
	}
	std::array<long double, 2> Point::GetPixel(const Monitor& monitor) const {
		if (Type == UnitType::px) return *this;
		else {
			std::array<long double, 2> dpi = monitor.GetDPI();
			return { Unit::ToUnit(X,Type,UnitType::inch) * dpi[0],Unit::ToUnit(Y,Type,UnitType::inch) * dpi[1] };
		}
	}
	UnitType Point::GetType() const { return Type; }

	void Point::SetData(const std::array<long double, 2>& list, UnitType u) {
		this->at(0) = list[0];
		this->at(1) = list[1];
		Type = u;
	}

	Point& Point::operator=(const Point& point) {
		X = point.X;
		Y = point.Y;
		Type = point.Type;
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