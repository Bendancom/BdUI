#ifndef BDUI_SHAPE
#define BDUI_SHAPE

#include <vector>
#include <any>
#include <graph/point.hpp>
#include <graph/color.hpp>
#include <graph/size.hpp>
#include <graph/unit.hpp>

#include <OpenGL/glad/glad.h>

#ifdef _WIN32
#include <OpenGL/glad/glad_wgl.h>
#endif

namespace BdUI {
	
	class Shape {
	public:
		enum ShapeType {
			Unknown = 0,
			Polygon = 1,
			Bezier = 2,
			Function = 3,
			Circle = 4,
			Rectangle = 5,
		};
	private:
		std::vector<Point> Points;
		ShapeType Type = ShapeType::Unknown;
		unsigned int VBO = 0;
		std::any Parameter;
	public:
		Shape() {}
		Shape(std::vector<Point>&& points, ShapeType type = Unknown);
		Shape(Point&& center, Unit&& radius);
		Shape(Point&& point, Size&& size);

		//Callback fuction,you needn't to call it
		void Paint(Point origin);

		Shape& operator=(std::pair<std::vector<Point>,ShapeType>&& points);
		Shape& operator=(std::pair<Point, Unit>&& circle);
		Shape& operator=(std::pair<Point, Size>&& rectangle);

		ShapeType GetShapeType();
	};
}
#endif