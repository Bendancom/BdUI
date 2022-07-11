#include "graph/shape.hpp"

namespace BdUI {
	void Shape::SetShapeType(const ShapeType::ShapeType& type) {
		Type = type;
	}
	ShapeType::ShapeType Shape::GetShapeType() {
		return Type;
	}
	void Shape::Paint(const Point& origin,const Size& ViewPort) {
		
	}
}