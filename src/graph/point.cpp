#include "graph/point.hpp"

namespace BdUI {
	Point::Point(const std::array<float,3>& list) {
		const float* p = list.data();
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
	std::array<float, 3> Point::GetData(const UnitType::UnitType& type) const {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		else if (type == Type) return std::array<float, 3>{X, Y, Z};
		else if (type == UnitType::Pixel) {
			float x = Unit(X, Type).GetData(UnitType::PixelHorizon);
			float y = Unit(Y, Type).GetData(UnitType::PixelVertical);
			float z = Z;
			return std::array<float, 3>{x, y, z};
		}
		else {
			float x = Unit(X,Type).GetData(type);
			float y = Unit(Y, Type).GetData(type);
			float z = Z;
			return std::array<float, 3>{x, y, z};
		}
	}
	float* Point::GetPointPtr() {
		return &X;
	}


	Point& Point::operator=(const std::array<float,3>& list) {
		const float* a = list.data();
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