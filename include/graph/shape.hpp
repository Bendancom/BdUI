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
	class Shape : public std::vector<Bezier> {
	friend class UI;
	private:
		static const char* HGL;
		void Paint(Point origin);
	public:
		using std::vector<Bezier>::vector;
		using std::vector<Bezier>::operator=;

		// void SetVertexs(const std::vector<BdUI::Vertex>& vertexs);
		// void SetVertexs_Index(const std::vector<size_t>& verteexs_index);

		// Shape& operator+=(const Vertex&);
		// Shape& operator=(std::vector<BdUI::Vertex>);
	};
}
#endif