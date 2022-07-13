#ifndef BDUI_SHAPE
#define BDUI_SHAPE

#include <vector>
#include "point.hpp"
#include "color.hpp"
#include "size.hpp"
#include "unit.hpp"

#include "OpenGL/glad/glad.h"

namespace BdUI {
	namespace ShapeType {
		enum ShapeType {
			Unknown = 0,
			Polygon = 1,
			Bezier = 2,
			Function = 3,
			Circle = 4,
			Rectangle = 5,
		};
	}
	class Shape : public std::vector<Point> {
	private:
		ShapeType::ShapeType Type = ShapeType::Unknown;
		unsigned int VBO;
		void Changed();
	public:
		Shape() {}
		template<ShapeType::ShapeType type> requires(type == ShapeType::Polygon || type == ShapeType::Bezier)
		Shape(const std::vector<Point>& points) : std::vector<Point>(points), Type(type) {}
		template<const ShapeType::ShapeType& type> requires(type == ShapeType::Circle)
		Shape(const Point& center,const Unit& radius) {
			this->insert(this->cend(), center);
			this->insert(this->cend(), Point(radius, 0, radius.GetType()));
			Type = type;
		}
		template<const ShapeType::ShapeType& type> requires(type == ShapeType::Rectangle)
		Shape(const Point& point,const Size& size) {
			Point&& p = point;
			if (p.GetType() != UnitType::Pixel) {
				p.ChangeUnit(UnitType::Pixel);
				this->insert(this->cend(), p);
			}
			else this->insert(this->cend(), p);
			this->insert(this->cend(), Point(size.Width, size.Height, size.GetType()));
			Type = type;
		}
		void SetShapeType(const ShapeType::ShapeType&);
		void SetCircle(const Point&, const Unit&);
		void SetQuad(const Point&,const Size&);
		ShapeType::ShapeType GetShapeType();
		void Paint(const Point&,const Size&);
	};
}
#endif