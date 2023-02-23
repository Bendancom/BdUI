#include <math/function.hpp>

namespace BdUI {
	std::pair<Variable, double> Function::calculate(const std::map<Variable, double>& m) {
		return { dependent_variable,Meta::calculate(m) };
	}
	std::pair<Variable, double> Function::operator()(const std::map<Variable, double>& m) {
		return calculate(m);
	}
}