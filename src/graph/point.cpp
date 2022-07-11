#include "graph/point.hpp"

namespace BdUI {
	Point::Point(const std::initializer_list<double>& list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* p = list.begin();
		X = *p;
		Y = *(p + 1);
	}

	void Point::ChangeUnit(const UnitType::UnitType& type) {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		if (type == Type) return;
		if (type == UnitType::Pixel) {
			Unit x(X, Type);
			x.ChangeUnit(UnitType::PixelHorizon);
			X = x;
			Unit y(Y, Type);
			y.ChangeUnit(UnitType::PixelVertical);
			Y = y;
			Type = type;
		}
		else {
			Unit x(X, Type);
			x.ChangeUnit(type);
			X = x;
			Unit y(Y, Type);
			y.ChangeUnit(type);
			Y = y;
			Type = type;
		}
	}
	const UnitType::UnitType& Point::GetType() {
		return Type;
	}

	Point& Point::operator=(const std::initializer_list<double>& list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* a = list.begin();
		X = *a;
		Y = *(a+1);
		return *this;
	}
	Point& Point::operator=(const Point& p) {
		X = p.X;
		Y = p.Y;
		Type = p.Type;
		return *this;
	}

	Point& Point::operator+(Point& p) {
		p.ChangeUnit(Type);
		X += p.X;
		Y += p.Y;
		return *this;
	}
	Point& Point::operator+=(Point& p) {
		p.ChangeUnit(Type);
		X += p.X;
		Y += p.Y;
		return *this;
	}
	Point& Point::operator-(Point& p) {
		p.ChangeUnit(Type);
		X -= p.X;
		Y -= p.Y;
		return *this;
	}
	Point& Point::operator-=(Point& p) {
		p.ChangeUnit(Type);
		X -= p.X;
		Y -= p.Y;
		return *this;
	}
}