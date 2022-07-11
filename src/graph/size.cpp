#include "graph/size.hpp"
#include <initializer_list>

namespace BdUI {
	Size::Size(const std::initializer_list<double>& list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* p = list.begin();
		Width = *p;
		Height = *(p + 1);
	}
	void Size::ChangeUnit(const UnitType::UnitType& type) {
		if (type == UnitType::PixelHorizon || type == UnitType::PixelVertical) throw error::Function::ParamError();
		if (type == Type) return;
		if (type == UnitType::Pixel) {
			Unit width(Width, Type);
			width.ChangeUnit(UnitType::PixelHorizon);
			Width = width;
			Unit height(Height, Type);
			height.ChangeUnit(UnitType::PixelVertical);
			Height = height;
			Type = type;
		}
		else {
			Unit width(Width, Type);
			width.ChangeUnit(type);
			Width = width;
			Unit height(Height, Type);
			height.ChangeUnit(type);
			Height = height;
			Type = type;
		}
	}
	const UnitType::UnitType& Size::GetType() {
		return Type;
	}

	Size& Size::operator=(const std::initializer_list<double>& list) {
		if (list.size() != 2) throw error::Function::ParamError();
		const double* a = list.begin();
		Width = *a;
		Height = *(a + 1);
		return *this;
	}
	Size& Size::operator=(const Size& size) {
		Width = size.Width;
		Height = size.Height;
		Type = size.Type;
		return *this;
	}

	Size& Size::operator+(Size& p) {
		p.ChangeUnit(Type);
		Width += p.Width;
		Height += p.Height;
		return *this;
	}
	Size& Size::operator+=(Size& p) {
		p.ChangeUnit(Type);
		Width += p.Width;
		Height += p.Height;
		return *this;
	}
	Size& Size::operator-(Size& p) {
		p.ChangeUnit(Type);
		Width -= p.Width;
		Height -= p.Height;
		return *this;
	}
	Size& Size::operator-=(Size& p) {
		p.ChangeUnit(Type);
		Width -= p.Width;
		Height -= p.Height;
		return *this;
	}
}