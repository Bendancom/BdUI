#ifndef BDUI_SHAPE
#define BDUI_SHAPE

#include <vector>
#include <variant>
#include <any>
#include <graph/point.hpp>
#include <graph/color.hpp>
#include <graph/size.hpp>
#include <graph/unit.hpp>
#include <memory>

#include <OpenGL/glad/glad.h>

#include <math/function.hpp>

#ifdef _WIN32
#include <OpenGL/glad/glad_wgl.h>
#endif

namespace BdUI {
	class Shape {
	friend class UI;
	private:
		std::vector<std::variant<std::shared_ptr<Shape>,Function>> Content;
		void Paint(Point origin);
	public:
		Shape() {}

		// void SetVertexs(const std::vector<BdUI::Vertex>& vertexs);
		// void SetVertexs_Index(const std::vector<size_t>& verteexs_index);

		// Shape& operator+=(const Vertex&);
		// Shape& operator=(std::vector<BdUI::Vertex>);
	};
}
#endif