#include "graph/shape.hpp"

namespace BdUI {
	Shape::Shape(std::vector<Point>&& points, ShapeType type) {
		if (type != Bezier && type != Polygon && type != Rectangle && type != Unknown)
			throw error::Class::Initialize_Failed("Use wrong type");
		Points = points;
		Type = type;
	}
	Shape::Shape(Point&& center, Unit&& radius) : Type(Circle), Parameter(radius) {
		Points.push_back(center);
	}
	Shape::Shape(Point&& point, Size&& size) : Type(Rectangle), Parameter(size) {
		Points.push_back(point);
	}

	void Shape::Paint(Point origin) {
	}

	Shape& Shape::operator=(std::pair<std::vector<Point>, ShapeType>&& points) {
		Points = points.first;
		Type = points.second;
		return *this;
	}
	Shape& Shape::operator=(std::pair<Point, Unit>&& circle) {
		Points.clear();
		Points.push_back(circle.first);
		Parameter = circle.second;
		Type = Circle;
		return *this;
	}
	Shape& Shape::operator=(std::pair<Point, Size>&& rectangle) {
		Points.clear();
		Points.push_back(rectangle.first);
		Parameter = rectangle.second;
		Type = Rectangle;
		return *this;
	}
	Shape::ShapeType Shape::GetShapeType() {
		return Type;
	}
}