#include "graph/shape.hpp"

namespace BdUI {
	void Shape::SetShapeType(const ShapeType::ShapeType& type) {
		Type = type;
	}
	ShapeType::ShapeType Shape::GetShapeType() {
		return Type;
	}
	void Shape::Paint(const Point& origin,const Size& ViewPort) const{
		Point location = origin.GetData(UnitType::Pixel);
		Size viewport = ViewPort.GetData(UnitType::Pixel);
		switch (Type) {

		}
	}
	void Shape::Changed() {
		for (const Point& i : *this) {
			
		}
	}
}