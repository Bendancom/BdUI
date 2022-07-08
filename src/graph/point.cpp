#include "graph/point.hpp"

namespace BdUI {
	Point::Point(std::initializer_list<double> list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* p = list.begin();
		X = *p;
		Y = *(p + 1);
	}

	void Point::ChangeUnit(const UnitType& type) {
		if (type == PixelHorizon || type == PixelVertical) throw error::Function::ParamError();
		if (type == Type) return;
		if (type == Pixel) {
			Unit x(X, Type);
			x.ChangeUnit(PixelHorizon);
			X = x;
			Unit y(Y, Type);
			y.ChangeUnit(PixelVertical);
			Y = y;
			_Type = type;
		}
		else {
			Unit x(X, Type);
			x.ChangeUnit(type);
			X = x;
			Unit y(Y, Type);
			y.ChangeUnit(type);
			Y = y;
			_Type = type;
		}
	}
	Point& Point::operator=(std::initializer_list<double> list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* a = list.begin();
		X = *a;
		Y = *(a+1);
		return *this;
	}
	Point& Point::operator=(const Point& p) {
		X = p.X;
		Y = p.Y;
		_Type = p.Type;
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