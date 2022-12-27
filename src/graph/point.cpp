#include "graph/point.hpp"

namespace BdUI {
	Point::Point(const std::array<double,3>& list) {
		const double* p = list.data();
		X = *p;
		Y = *(p + 1);
		Z = *(p + 2);
	}

	Point& Point::ChangeUnit(const UnitType::UnitType& type) { // TODO: 实现Z的转换
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		else if (type == Type) return *this;
		else if (type == UnitType::Pixel) {
			X = Unit(X, Type).GetData(UnitType::PixelHorizon);
			Y = Unit(Y, Type).GetData(UnitType::PixelVertical);
			Z = Z;
		}
		else {
			X = Unit(X, Type).GetData(type);
			Y = Unit(Y, Type).GetData(type);
			Z = Z;
		}
		return *this;
	}
	UnitType::UnitType Point::GetType() const {
		return Type;
	}
	std::array<double, 3> Point::GetData(const UnitType::UnitType& type) const {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		else if (type == Type) return std::array<double, 3>{X, Y, Z};
		else if (type == UnitType::Pixel) {
			double x = Unit(X, Type).GetData(UnitType::PixelHorizon);
			double y = Unit(Y, Type).GetData(UnitType::PixelVertical);
			double z = Z;
			return std::array<double, 3>{x, y, z};
		}
		else {
			double x = Unit(X,Type).GetData(type);
			double y = Unit(Y, Type).GetData(type);
			double z = Z;
			return std::array<double, 3>{x, y, z};
		}
	}

	Point& Point::operator=(const std::array<double,3>& list) {
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