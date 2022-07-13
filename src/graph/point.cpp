#include "graph/point.hpp"

namespace BdUI {
	Point::Point(const std::array<double,3>& list) {
		const double* p = list.data();
		X = *p;
		Y = *(p + 1);
		Z = *(p + 2);
	}

	void Point::ChangeUnit(const UnitType::UnitType& type) { // TODO: 实现Z的转换
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
	UnitType::UnitType Point::GetType() {
		return Type;
	}
	std::array<double, 3> Point::GetPoint(const UnitType::UnitType& type) {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		if (type == Type) return std::array<double, 3>{X, Y, Z};
		Point p = Point(X, Y, Z, Type);
		p.ChangeUnit(type);
		return std::array<double, 3>{p.X, p.Y, p.Z};
	}

	Point& Point::operator=(const std::array<double,3>& list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* a = list.data();
		X = *a;
		Y = *(a+1);
		Z = *(a + 2);
		return *this;
	}
	Point& Point::operator=(const Point& p) {
		X = p.X;
		Y = p.Y;
		Z = p.Z;
		Type = p.Type;
		return *this;
	}

	Point& Point::operator+(Point& p) {
		p.ChangeUnit(Type);
		X += p.X;
		Y += p.Y;
		Z += p.Z;
		return *this;
	}
	Point& Point::operator+=(Point& p) {
		p.ChangeUnit(Type);
		X += p.X;
		Y += p.Y;
		Z += p.Z;
		return *this;
	}
	Point& Point::operator-(Point& p) {
		p.ChangeUnit(Type);
		X -= p.X;
		Y -= p.Y;
		Z -= p.Z;
		return *this;
	}
	Point& Point::operator-=(Point& p) {
		p.ChangeUnit(Type);
		X -= p.X;
		Y -= p.Y;
		Z -= p.Z;
		return *this;
	}
}