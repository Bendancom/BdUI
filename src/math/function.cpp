#include <math/function.hpp>

namespace BdUI {
	function::function(const function& f) : Meta(f), dependent_variable(f.dependent_variable) {}
	function& function::operator=(const function& f) {
		dependent_variable = f.dependent_variable;
		Meta::operator=(f);
		return *this;
	}
	std::pair<Variable, double> function::calculate(const std::map<Variable, double>& m) const{
		return { dependent_variable,Meta::calculate(m) };
	}
	std::pair<Variable, double> function::operator()(const std::map<Variable, double>& m) const{
		return calculate(m);
	}
	std::vector<std::pair<Variable, double>> Parametricfunc::calculate(const std::map<Variable, double>& m) const {
		std::vector<std::pair<Variable, double>>&& temp{};
		for (int i = 0; i < this->size(); i++)
			temp.push_back(at(i).calculate(m));
		return temp;
	}
	std::vector<std::pair<Variable, double>> Parametricfunc::operator()(const std::map<Variable, double>& m) const {
		return calculate(m);
	}
}