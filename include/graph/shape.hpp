#ifndef BDUI_SHAPE
#define BDUI_SHAPE

#include <list>
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
	class Shape : public std::list<Point> {
	private:
		ShapeType::ShapeType Type = ShapeType::Unknown;
		unsigned int VBO;
	public:
		Shape(){}
		template<ShapeType::ShapeType type> requires(type != ShapeType::Rectangle && type != ShapeType::Circle)
		Shape(const std::list<Point>& points) : std::list<Point>(points), Type(type) {
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
		}
		template<const ShapeType::ShapeType& type> requires(type == ShapeType::Circle)
		Shape(const Point& center,const Unit& radius) {
			this->insert(this->cend(), center);
			this->insert(this->cend(), Point(radius, 0, radius.GetType()));
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			Type = type;
		}
		template<const ShapeType::ShapeType& type> requires(type == ShapeType::Rectangle)
		Shape(const Point& point,const Size& size) {
			if (point.GetType() != UnitType::Pixel) {
				Point&& p = point;
				p.ChangeUnit(UnitType::Pixel);
				this->insert(this->cend(), p);
			}
			else this->insert(this->cend(), point);
			this->insert(this->cend(), Point(size.Width, size.Height, size.GetType()));
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
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