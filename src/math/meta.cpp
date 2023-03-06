#include "math/meta.hpp"

namespace BdUI {
	Variable::Variable(const Character& c, const std::string& str) : 
		std::pair < Character,
		std::variant<std::monostate,
		std::vector<int>,
		std::vector<Character>>>{ c , Variable::no_subscript }
	{
		std::u32string u32_string = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>{}.from_bytes(str);
		std::vector<Character>&& chars = std::vector<Character>();
		for (int i = 0; i < u32_string.size(); i++)
			chars.push_back(u32_string[i]);
		second = chars;
	}
	Variable::Variable(const Character& c, const std::initializer_list<char>& v) : std::pair < Character,
		std::variant<std::monostate,
		std::vector<int>,
		std::vector<Character>>>{ c, Variable::no_subscript } 
	{
		std::vector<Character>&& temp{};
		for (char i : v)
			temp.push_back(Character(i));
		second = temp;
	}
	bool Variable::operator<(const Variable& v) const {
		if (first == v.first) {
			if (second.index() == v.second.index()) {
				if (second.index() == 0) return false;
				if (second.index() == 1) {
					const std::vector<int>* i = std::get_if<1>(&second);
					const std::vector<int>* j = std::get_if<1>(&v.second);
					if (i->size() < j->size()) return true;
					else if (i->size() > j->size()) return false;
					else {
						for (int k = 0; k < i->size(); k++)
							if (i->at(k) < j->at(k))
								return true;
						return false;
					}
				}
				else {
					const std::vector<Character>* i = std::get_if<2>(&second);
					const std::vector<Character>* j = std::get_if<2>(&v.second);
					if (i->size() < j->size()) return true;
					else if (i->size() > j->size()) return false;
					else {
						for (int k = 0; k < i->size(); k++)
							if (i->at(k) < j->at(k))
								return true;
						return false;
					}
				}
			}
			else if (second.index() < v.second.index()) return true;
			else return false;
		}
		else if (first < v.first) return true;
		else return false;
	}
	Meta::Meta(const std::string& LATEX_expression) {
		std::string latex = LATEX_expression;
		size_t index = 0;


		latex.find('+');
		latex.find('-');
		latex.find('*');
		latex.find('/');
		latex.find('^');
	}

	double Meta::_calculate(const Meta& meta,std::deque<std::pair<Calculation, std::any>>& deq,std::map<Variable, double>& temp_date) {
		if (deq.size() > 0) {
			std::pair<Calculation, std::any> p = deq.back();
			deq.pop_back();
			switch (p.first)
			{
			case Calculation::sum: {
				std::array<Character, 2> arr = std::any_cast<std::array<Character, 2>>(p.second);
				double result = 0;
				for (int i = temp_date.at(arr[0]); i <= temp_date.at(arr[1]); i++){
					std::deque<std::pair<Calculation, std::any>> d(deq);
					temp_date[arr[0]] = i;
					result += _calculate(meta, d, temp_date);
				}
				return result;
			}
			case Calculation::derivate: {
				break;
			}
			}
			
		}

		double result;
		if (const double* e = std::get_if<double>(&meta.coefficient); e)
			result = *e;
		else {
			size_t i = std::get<size_t>(meta.coefficient);
			if (meta.all_variables[i].changable_subscript && meta.all_variables[i].second != Variable::no_subscript) {
				if (const std::vector<Character>* chars = std::get_if<std::vector<Character>>(&meta.all_variables[i].second); chars) {
					std::vector<int> arr;
					for (Character j : *chars) {
						if (j == meta.all_variables[i].first) throw error::Math::variable_subscript_error();
						if (std::vector<Variable>::const_iterator iter =
							std::find(meta.all_variables.cbegin(), meta.all_variables.cend(), j);
							iter != meta.all_variables.cend())
							arr.push_back(temp_date[*iter]);
					}
					result = temp_date[Variable{ meta.all_variables[i].first,arr }];
				}
				else throw error::Math::variable_subscript_error();
			}
			else result = temp_date[meta.all_variables[i]];
		}
		if (!meta.Content.empty()) {
			for (auto i = meta.Content.begin(); i != meta.Content.end();i++) {
				std::deque<std::pair<Calculation, std::any>> d = i->second.operation;
				switch (i->first) {
				case Calculation::plus:
					result += i->second._calculate(i->second,d,temp_date);
					break;
				case Calculation::minus:
					result -= i->second._calculate(i->second,d,temp_date);
					break;
				case Calculation::multi:
					result *= i->second._calculate(i->second,d,temp_date);
					break;
				case Calculation::divide:
					result /= i->second._calculate(i->second,d,temp_date);
					break;
				case Calculation::pow: {
					double res = i->second._calculate(i->second, d, temp_date);
					if (res != 0) result = ::pow(result, res);
					else result = 1;
					break;
				}
				case Calculation::log:
					result = ::log(result) / ::log(i->second._calculate(i->second,d,temp_date));
					break;
				case Calculation::fac:
					double&& res = 1;
					for (unsigned int j = 1; j <= ::floor(result); j++)
						res *= j;
					result = res;
					break;
				}
			}
		}
		return result;
	}

	double Meta::calculate(const std::map<Variable, double>& date) const {
		if (date.size() < all_variables.size()) throw error::Math::variable_num_error();
		std::map<Variable, double> temp_date = date;
		std::deque<std::pair<Calculation, std::any>> deq = operation;
		return _calculate( *this,deq,temp_date);
	}

	Meta& Meta::erase(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter) {
		if (iter->second.coefficient.index() == 1) {
			size_t k = 0;
			for (std::pair<Calculation,Meta> i : Content)
				if (i.second.coefficient == iter->second.coefficient)
					k++;
			if (k == 1) all_variables.erase(all_variables.begin() + std::get<size_t>(iter->second.coefficient));
		}
		Content.erase(iter);
		Minus(iter->second);
		return *this;
	}
	Meta& Meta::emplace(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter, const std::pair<Calculation, Meta>& p) {
		Content.emplace(iter, std::pair<Calculation, Meta>{ p.first, p.second });
		Add(iter->second);
		return *this;
	}
	Meta& Meta::insert(const std::list<std::pair<Calculation, Meta>>::const_iterator& iter, const std::pair<Calculation, Meta>& p) {
		Content.insert(iter, std::pair<Calculation, Meta>{ p.first, p.second });
		Add(iter->second);
		return *this;
	}

	Meta& Meta::pow(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::pow, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::log(const Meta& meta) {
		Content.push_back(std::pair<Calculation,Meta>{ Calculation::log, meta });
		Add(meta);
		return *this;
	}
	Meta& Meta::plus(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::plus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::minus(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::minus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::multi(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::multi, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::divide(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::divide, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::sum(const std::array<Character, 2>& ac) {
		if (ac.size() == 2) {
			for (Variable i : ac) {
				bool have = false;
				for (Variable j : all_variables)
					if (i == j) {
						have = true;
						break;
					}
				if (!have) all_variables.push_back(i);
			}
			operation.push_back({ operator_calculation.value(), ac });
		}
		else throw error::Math::variable_num_error();
		return *this;
	}
	Meta& Meta::fac() {
		Content.push_back({ Calculation::fac,Meta(0) });
		return *this;
	}

	Meta& Meta::operator+(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::plus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator-(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::minus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator*(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::multi, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator/(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::divide, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator+=(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::plus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator-=(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::minus, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator*=(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::multi, meta});
		Add(meta);
		return *this;
	}
	Meta& Meta::operator/=(const Meta& meta) {
		Content.push_back(std::pair<Calculation, Meta>{Calculation::divide, meta});
		Add(meta);
		return *this;
	}

	void Meta::Add(const Meta& meta) {
		for (auto i : meta.all_variables) {
			bool have = false;
			for (auto j : all_variables) {
				if (i == j) {
					have = true;
					break;
				}
			}
			if (!have) all_variables.push_back(i);
		}
	}
	void Meta::Minus(const Meta& meta) {
		std::vector<size_t> num;
		size_t c = 0;
		for (auto i : meta.all_variables) {
			num[c] = 0;
			for (auto j : Content) {
				for (auto k : j.second.all_variables)
					if (k == i) num[c]++;
			}
			c++;
		}
		for (auto i : num)
			if (i == 1) all_variables.erase(all_variables.begin() + i);
	}

	double Meta::operator()(const std::map<Variable, double>& m) {
		return calculate(m);
	}
	Meta& Meta::operator=(const Meta& meta) {
		all_variables = meta.all_variables;
		coefficient = meta.coefficient;
		operator_calculation = meta.operator_calculation;
		Content = meta.Content;
		operation = meta.operation;
		return *this;
	}
	bool Meta::operator==(const Meta& meta) const {
		if (coefficient != meta.coefficient) return false;
		else if (all_variables != meta.all_variables) return false;
		else if (Content != meta.Content) return false;
		else return true;
	}
	std::partial_ordering Meta::operator<=>(const Meta& meta) const{
		if (coefficient == meta.coefficient) {
			if (all_variables == meta.all_variables) {
				if (Content == meta.Content) return std::partial_ordering::equivalent;
				else return std::partial_ordering::unordered;
			}
			else return std::partial_ordering::unordered;
		}
		else return std::partial_ordering::unordered;
	}

	Meta& Meta::operator<<(Calculation&& c) {
		switch (c)
		{
		case Calculation::fac:
			fac();
			break;
		default:
			operator_calculation = c;
			break;
		}
		return *this;
	}
	Meta& Meta::operator>>(const Meta& meta) {
		if (operator_calculation) {
			switch (operator_calculation.value())
			{
			case Calculation::derivate:
				if (const size_t* k = std::get_if<size_t>(&meta.coefficient); k) {
					if (meta.Content.empty()) {
						operation.push_back({ operator_calculation.value(), meta.all_variables[*k] });
						operator_calculation.reset();
					}
					else throw error::Math::operator_error();
				}
				else throw error::Math::no_variable();
				break;
			case Calculation::sum:
				throw error::Math::variable_num_error();
			default:
				Content.push_back(std::pair<Calculation, Meta>{operator_calculation.value(), meta});
				operator_calculation.reset();
				break;
			}
			Add(meta);
		}
		else throw error::Math::operator_error();
		return *this;
	}
	Meta& Meta::operator>>(const std::array<Character, 2>& a) {
		if (operator_calculation) {
			switch (operator_calculation.value()) {
			case Calculation::sum:
				if (a.size() == 2) {
					for (Variable i : a) {
						bool have = false;
						for (Variable j : all_variables)
							if (i == j) {
								have = true;
								break;
							}
						if (!have) all_variables.push_back(i);
					}
					operation.push_back({ operator_calculation.value(), a });
					operator_calculation.reset();
				}
				else throw error::Math::variable_num_error();
				break;
			default:
				throw error::Math::operator_error();
				break;
			}
		}
		else throw error::Math::operator_error();
	}
}