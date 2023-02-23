#include "math/meta.hpp"

namespace BdUI {
	bool Variable::operator<(const Variable& v) const {
		if (first.GetUTF_32() < v.first.GetUTF_32())
			if (second < v.second)
				return true;
			else return false;
		else return false;
	}

	Meta::Meta(const Character& c) {
		coefficient = Variable(c,std::string());
		_variables.push_back(std::get<Variable>(coefficient));
	}
	Meta::Meta(std::string LATEX_expression) {

	}

	double Meta::calculate(const std::map<Variable,double>& date) const{
		if (date.size() < variables.size()) throw error::Math::variable_num_error(); 
		double result = 0;
		if (const double* k = std::get_if<double>(&coefficient); k)
			result = *k;
		else
			result = date.at(std::get<Variable>(coefficient));
		if (!Content.empty()) {
			for (std::pair<Calculation, Meta> i : Content) {
				switch (i.first) {
				case Calculation::plus:
					result += i.second.calculate(date);
					break;
				case Calculation::minus:
					result -= i.second.calculate(date);
					break;
				case Calculation::multi:
					result *= i.second.calculate(date);
					break;
				case Calculation::divide:
					result /= i.second.calculate(date);
					break;
				case Calculation::pow:
					result = ::pow(result, i.second.calculate(date));
					break;
				case Calculation::log:
					result = ::log(result) / ::log(i.second.calculate(date));
				}
			}
		}
		return result;
	}

	double Meta::calculate() const{
		if (variables.size() != 0) throw error::Math::variable_num_error();
		if (const Variable* v = std::get_if<Variable>(&coefficient); v) throw error::Math::variable_num_error();
		double result = std::get<double>(coefficient);
		if (!Content.empty()) {
			for (std::pair<Calculation, Meta> i : Content) {
				switch (i.first) {
				case Calculation::plus:
					result += i.second.calculate();
					break;
				case Calculation::minus:
					result -= i.second.calculate();
					break;
				case Calculation::multi:
					result *= i.second.calculate();
					break;
				case Calculation::divide:
					result /= i.second.calculate();
					break;
				case Calculation::pow:
					result = ::pow(result, i.second.calculate());
					break;
				case Calculation::log:
					result = ::log(result) / ::log(i.second.calculate());
				}
			}
		}
		return result;
	}

	void Meta::Addvariable(const Meta& meta) {
		if (meta.coefficient.index() == 0) return;
		if(std::find(_variables.cbegin(),_variables.cend(), std::get<Variable>(meta.coefficient)) != _variables.cend())
			_variables.push_back(std::get<Variable>(coefficient));
	}

	Meta& Meta::erase(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter) {
		Content.erase(iter);
		return *this;
	}
	Meta& Meta::emplace(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter, const std::pair<Calculation, Meta>& p) {
		Content.emplace(iter, p);
		return *this;
	}
	Meta& Meta::insert(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter, const std::pair<Calculation, Meta>& p) {
		Content.insert(iter, p);
		return *this;
	}

	Meta& Meta::pow(const Meta& meta) {
		Content.push_back({ Calculation::pow,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::log(const Meta& meta) {
		Content.push_back({ Calculation::log,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::plus(const Meta& meta) {
		Content.push_back({ Calculation::plus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::minus(const Meta& meta) {
		Content.push_back({ Calculation::minus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::multi(const Meta& meta) {
		Content.push_back({ Calculation::multi,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::divide(const Meta& meta) {
		Content.push_back({ Calculation::divide,meta });
		Addvariable(meta);
		return *this;
	}

	Meta& Meta::operator+(const Meta& meta) {
		Content.push_back({ Calculation::plus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator-(const Meta& meta) {
		Content.push_back({ Calculation::minus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator*(const Meta& meta) {
		Content.push_back({ Calculation::multi,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator/(const Meta& meta) {
		Content.push_back({ Calculation::divide,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator+=(const Meta& meta) {
		Content.push_back({ Calculation::plus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator-=(const Meta& meta) {
		Content.push_back({ Calculation::minus,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator*=(const Meta& meta) {
		Content.push_back({ Calculation::multi,meta });
		Addvariable(meta);
		return *this;
	}
	Meta& Meta::operator/=(const Meta& meta) {
		Content.push_back({ Calculation::divide,meta });
		Addvariable(meta);
		return *this;
	}

	double Meta::operator()(const std::map<Variable, double>& m) {
		return calculate(m);
	}
	double Meta::operator()() {
		return calculate();
	}

	bool Meta::operator==(const Meta& meta) const {
		if (coefficient != meta.coefficient) return false;
		else if (Content != Content) return false;
		else {
			if (_variables.size() != meta._variables.size()) return false;
			unsigned int k = 0;
			for (std::pair<Character, std::string> i : _variables)
				for (std::pair<Character, std::string> j : meta._variables)
					if (i == j) k++;
			if (k != _variables.size()) return false;
		}
		return true;
	}
	std::partial_ordering Meta::operator<=>(const Meta& meta) const{
		if (coefficient == meta.coefficient) {
			if (Content == meta.Content) return std::partial_ordering::equivalent;
			else return std::partial_ordering::unordered;
		}
		else {
			const double* i = std::get_if<double>(&coefficient);
			const double* j = std::get_if<double>(&meta.coefficient);
			if (i != nullptr && j != nullptr)
				return (*i) <=> (*j);
			else return std::partial_ordering::unordered;
		}
	}

	Meta& Meta::operator<<(Calculation&& c) {
		operator_calculation = c;
		return *this;
	}
	Meta& Meta::operator>>(const Meta& meta) {
		if (operator_calculation) {
			Content.push_back({ operator_calculation.value(),meta});
			Addvariable(meta);
			operator_calculation.reset();
		}
		else throw error::Math::operator_error();
		return *this;
	}
}