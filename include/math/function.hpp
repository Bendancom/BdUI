#ifndef BDUI_FUNCTION
#define BDUI_FUNCTION

#include <math/meta.hpp>
#include <array>

namespace BdUI {

	class Function : public Meta {
	private:
		Variable dependent_variable;
	public:
		Function(const Variable& c,const Meta& m) : dependent_variable(c), Meta(m) {}
		Function(const Function&) = default;

		std::pair<Variable, double> calculate(const std::map<Variable, double>& = std::map<Variable,double>());
		std::pair<Variable, double> operator()(const std::map<Variable, double>& = std::map<Variable,double>());

		Function& operator=(const Function&) = default;
		using Meta::operator=;
	};

	template<int number>
	class Parametric_func : private std::array<Function,number>{
	public:
		using std::array<Function, number>::array;

		inline std::map<Variable, double> calculate(const std::map<Variable, double>& m = std::map<Variable, double>()) {
			std::map<Variable, double>&& temp = std::map<Variable, double>();
			for (Function i : this)
				temp.insert(i.calculate(m));
			return temp;
		}
		inline std::map<Variable, double> operator()(const std::map<Variable, double>& m = std::map<Variable, double>()) {
			std::map<Variable, double>&& temp = std::map<Variable, double>();
			for (Function i : this)
				temp.insert(i.calculate(m));
			return temp;
		}

		using std::array<Function, number>::operator=;
	};
}

#endif