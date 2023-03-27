#include "math/meta.hpp"

namespace BdUI {
	const std::regex Variable::variable_regex("([A-Za-z])_\\{([\\w-^_]+)(?:,([\\w-^_]+))*\\}|([A-Za-z])(?:_([A-Za-z]))?");
	const std::vector<std::variant<int, std::string>> Variable::no_subscript = std::vector<std::variant<int, std::string>>();

	const std::map<Meta::Calculation, unsigned char> Meta::opera{
		{Calculation::plus,0},	{Calculation::minus,0},
		{Calculation::multi,1},	{Calculation::divide,1},
		{Calculation::log,2},		{Calculation::pow,2},
		{Calculation::sin,2},		{Calculation::fac,2},		{Calculation::cos,2},
		{Calculation::sum,3},		{Calculation::intergeral,3},	{Calculation::derivate,3},	{Calculation::multiply,3}
	};
	const std::map<char, Meta::Calculation> Meta::opera_calculation{
			{'+',Calculation::plus},	{'-',Calculation::minus},
			{'*',Calculation::multi},	{'/',Calculation::divide},
			{'!',Calculation::fac},		{'^',Calculation::pow},
			{'(',Calculation::left_bracket},{')',Calculation::right_bracket},
	};

	
	const std::map<std::string, Meta::Calculation> Meta::function_name{
		{"sum",Calculation::sum}, {"cos",Calculation::cos}, {"sin",Calculation::sin}
	};
	
	const std::map<Meta::Calculation, unsigned char> Meta::operation_parameter_number{
		{Calculation::fac,0},{Calculation::sin,0},{Calculation::cos,0},
		{Calculation::plus,1}, {Calculation::multi,1}, {Calculation::divide,1}, {Calculation::minus,1},
		{Calculation::derivate,1}, {Calculation::pow,1}, {Calculation::log,1},
		{Calculation::sum,2},{Calculation::multiply,2},{Calculation::intergeral,2}
	};

	const std::map<Meta::Calculation, std::regex> Meta::function_regex{
	{Calculation::sum,std::regex("\\\\sum\\{([\\w^_])\\}\\{([\\w^_])\\}\\{(.+)\\}")},
	{Calculation::cos,std::regex("\\\\cos\\{(.+)\\}")},
	{Calculation::sin,std::regex("\\\\sin\\{(.+)\\}")},
	};
	const std::regex Meta::function_name_regex("(\\\\([A-Za-z]+)(?:\\{.+?\\})+)");
	const std::regex Meta::vari_with_num_regex("([0-9\\.\\-]+)?(\\.*)");
	
	Variable::Variable(const std::string& Latex) : std::pair < Character,
		std::vector<std::variant<int, std::string>>>{'\0',std::vector<std::variant<int,std::string>>()}
	{
		if (std::smatch match; std::regex_match(Latex, match, variable_regex)) {
			size_t i = 1;
			for (; i < match.size(); i++)
				if (match[i].matched)
					break;
			first = match[i].str()[0];
			for (i++; i < match.size(); i++) {
				if (!match[i].matched) continue;
				std::string&& s = match[i].str();
				bool IsNum = true;
				for (char c : s) {
					if ((c > '9' || c < '0') && c != '-') {
						IsNum = false;
						break;
					}
				}
				if (IsNum) second.push_back(std::stoi(s));
				else second.push_back(s);
			}
		}
		else throw error::Math::variable_string_error();
	}
	std::vector<Variable> Variable::GetVariables(const std::string& Latex) {
		std::vector<Variable>&& variables{};
		std::smatch match;
		for (std::string str = Latex;!str.empty(); str = match.suffix().str())
			if (std::regex_search(str, match, variable_regex))
				variables.push_back(match[0].str());
			else throw error::Math::variable_string_error();
		return variables;
	}

	Meta Meta::StrToVariables(const std::string& str) {
		if (std::smatch match; std::regex_match(str, match, vari_with_num_regex)) {
			std::unique_ptr<Meta> meta;
			if (match[1].matched)
				meta.reset(new Meta(std::stold(match[1].str())));
			if (match[2].matched) {
				std::vector<Variable> vs = Variable::GetVariables(match[2].str());
				if (meta != nullptr)
					for (Variable v : vs)
						meta->multi(v);
				else {
					meta.reset(new Meta(vs[0]));
					for (size_t i = 1; i < vs.size(); i++)
						meta->multi(vs[i]);
				}
			}
			return *meta.get();
		}
		else throw error::Math::latex_string_invalid();
	}

	Meta::Meta(const Meta& meta) {
		switch (meta.content.index()) {
		case 0: content = std::get<long double>(meta.content); break;
		case 1: content = std::make_unique<Variable>(*std::get<std::unique_ptr<Variable>>(meta.content));
			break;
		case 2: content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
			*std::get<2>(meta.content));
			break;
		}
	}
	Meta::Meta(const char* ch) { content = Meta::Meta(std::string(ch)).content; };
	Meta::Meta(const std::string& str) {
		std::string variable_num;
		std::stack<std::variant<Meta, Calculation>> stack;
		std::stack<Calculation> calcul;
		std::vector<Variable>&& variables{};
		for (char ch : str) {
			if (ch == ' ') continue;
			else if (auto o = opera_calculation.find(ch); o != opera_calculation.end()) {
				if (!variable_num.empty()) {
					stack.push(StrToVariables(variable_num));
					switch (std::get<Meta>(stack.top()).content.index()) {
					case 0:break;
					case 1: if (auto i = *std::get<1>(std::get<Meta>(stack.top()).content);
						std::find(variables.begin(), variables.end(), i) == variables.end()) variables.push_back(i);
						break;
					case 2: {
						std::vector<Variable>& temp = std::get<2>(std::get<Meta>(stack.top()).content)->first;
						for (Variable v : temp)
							if (std::find(variables.begin(), variables.end(), v) == variables.end()) variables.push_back(v);
						break;
					}
					}
				}
				variable_num = std::string();
				if (o->second == Calculation::left_bracket)
					calcul.push(o->second);
				else if (o->second == Calculation::right_bracket) {
					for (; calcul.top() != Calculation::left_bracket; calcul.pop())
						stack.push(calcul.top());
					calcul.pop();
				}
				else while (true) {
					if (calcul.empty())
						calcul.push(o->second);
					else if (calcul.top() == Calculation::left_bracket)
						calcul.push(o->second);
					else if (opera.at(calcul.top()) < opera.at(o->second))
						calcul.push(o->second);
					else {
						stack.push(calcul.top());
						calcul.pop();
						continue;
					}
					break;
				}
			}
			else if (ch == '\\') {

			}
			else variable_num += ch;
		}
		if (!variable_num.empty()) {
			stack.push(StrToVariables(variable_num));
			switch (std::get<Meta>(stack.top()).content.index()) {
			case 0:break;
			case 1: if (auto i = *std::get<1>(std::get<Meta>(stack.top()).content);
				std::find(variables.begin(), variables.end(), i) == variables.end()) variables.push_back(i);
				break;
			case 2: {
				std::vector<Variable>& temp = std::get<2>(std::get<Meta>(stack.top()).content)->first;
				for (Variable v : temp)
					if (std::find(variables.begin(), variables.end(), v) == variables.end()) variables.push_back(v);
				break;
			}
			}
		}
		for (; !calcul.empty(); calcul.pop())
			stack.push(calcul.top());
		std::stack<std::variant<Meta, Calculation>> r_stack;
		for (; !stack.empty(); stack.pop())
			r_stack.push(stack.top());
		content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
			std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>{ variables,r_stack });
	}

	long double Meta::_calculate(std::stack<std::variant<Meta, Calculation>>& content,std::map<Variable, long double>& date) {
		std::stack<Meta> temp;
		for (; !content.empty(); ) {
			std::variant<Meta,Calculation> con = content.top();
			content.pop();
			if (con.index() == 0)
				temp.push(std::get<Meta>(con));
			else {
				Calculation& calcul = std::get<Calculation>(con);
				std::vector<Meta> metas;
				for (size_t i = 0; i < operation_parameter_number.at(calcul) + 1; i++) {
					metas.push_back(temp.top());
					temp.pop();
				}

				switch (calcul) {
				case Calculation::fac: {
					long double floor = ::floor(_calculate(metas[0], date));
					if (floor < 0) throw error::Math::operator_error();
					else if (floor == 0) return 1;
					else {
						size_t i = 1;
						for (; i < floor; i++)
							i *= i;
						temp.push((long double)i);
					}
					break;
				}
				case Calculation::sin: temp.push(::sinl(_calculate(metas[0], date))); break;
				case Calculation::cos: temp.push(::cosl(_calculate(metas[0], date))); break;

				case Calculation::plus: temp.push(_calculate(metas[0], date) + _calculate(metas[1], date)); break;
				case Calculation::minus: temp.push(_calculate(metas[0], date) - _calculate(metas[1], date)); break;
				case Calculation::multi: temp.push(_calculate(metas[0], date) * _calculate(metas[1], date)); break;
				case Calculation::divide: temp.push(_calculate(metas[0], date) / _calculate(metas[1], date)); break;

				case Calculation::derivate:break;
				case Calculation::pow: temp.push(::powl(_calculate(metas[0], date), _calculate(metas[1], date))); break;
				case Calculation::log: temp.push(::logl(_calculate(metas[0], date) / ::logl(_calculate(metas[1], date)))); break;

				case Calculation::sum: {
					content.push(metas[2]);
					for (; !temp.empty(); temp.pop())
						content.push(temp.top());
					long double sum = 0;
					size_t i = _calculate(metas[1], date);
					size_t n = _calculate(metas[0], date);
					if (i < n) {
						for (; i <= n; i++) {
							std::stack<std::variant<Meta, Calculation>> stack = content;
							date[*std::get<std::unique_ptr<Variable>>(metas[1].content)] = (long double)i;
							sum += _calculate(stack, date);
						}
					}
					else {
						for (; i >= n;i--) {
							std::stack<std::variant<Meta, Calculation>> stack = content;
							date[*std::get<std::unique_ptr<Variable>>(metas[1].content)] = (long double)i;
							sum += _calculate(stack , date);
						}
					}
					temp.push(sum);
					content = std::stack<std::variant<Meta, Calculation>>();
					break;
				}
				case Calculation::multiply: {
					content.push(metas[2]);
					for (; !temp.empty(); temp.pop())
						content.push(temp.top());
					break;
					long double multiply = 1;
					size_t i = _calculate(metas[1], date);
					size_t n = _calculate(metas[0], date);
					if (i < n) {
						for (; i <= n; i++) {
							std::stack<std::variant<Meta, Calculation>> stack = content;
							date[*std::get<std::unique_ptr<Variable>>(metas[1].content)] = (long double)i;
							multiply *= _calculate(stack, date);
						}
					}
					else {
						for (; i >= n; i--) {
							std::stack<std::variant<Meta, Calculation>> stack = content;
							date[*std::get<std::unique_ptr<Variable>>(metas[1].content)] = (long double)i;
							multiply *= _calculate(stack, date);
						}
					}
				}
				case Calculation::intergeral: break;
				}
			}
		}
		return _calculate(temp.top(),date);
	}
	long double Meta::_calculate(Meta& meta, std::map<Variable, long double>& date) {
		switch (meta.content.index()) {
		case 0: return std::get<long double>(meta.content);
		case 1: return date.at(*std::get<std::unique_ptr<Variable>>(meta.content));
		case 2: return _calculate(std::get<2>(meta.content)->second, date);
		}
	}

	long double Meta::calculate(const std::map<Variable, long double>& date) const {
		switch (content.index()) {
		case 0: return std::get<long double>(content);
		case 1: return date.at(*std::get<std::unique_ptr<Variable>>(content));
		case 2: {
			if (content.index() == 2 && std::get<2>(content)->first.size() > date.size()) throw error::Math::variable_num_error();
			std::map<Variable, long double> temp_date = date;
			std::stack<std::variant<Meta, Calculation>> temp_content = std::get<2>(content)->second;
			return _calculate(temp_content, temp_date);
		}
		}
	}

	Meta& Meta::pow(const Meta& meta) { CalculationAdd(Calculation::pow, { meta }); return *this; }
	Meta& Meta::log(const Meta& meta) { CalculationAdd(Calculation::log, { meta }); return *this; }
	Meta& Meta::plus(const Meta& meta) { CalculationAdd(Calculation::plus, { meta }); return *this; }
	Meta& Meta::minus(const Meta& meta) { CalculationAdd(Calculation::minus, { meta }); return *this; }
	Meta& Meta::multi(const Meta& meta) { CalculationAdd(Calculation::multi, { meta }); return *this; }
	Meta& Meta::divide(const Meta& meta) { CalculationAdd(Calculation::divide, { meta }); return *this; }
	Meta& Meta::sum(const std::pair<Meta, Meta>& pair) { CalculationAdd(Calculation::sum, { pair.first,pair.second }); return *this; }
	Meta& Meta::fac() { CalculationAdd(Calculation::fac); return *this; }
	Meta& Meta::sin() { CalculationAdd(Calculation::sin); return *this; }
	Meta& Meta::cos() { CalculationAdd(Calculation::cos); return *this; }

	Meta& Meta::operator+(const Meta& meta) { plus(meta); return *this; }
	Meta& Meta::operator-(const Meta& meta) { minus(meta); return *this; }
	Meta& Meta::operator*(const Meta& meta) { multi(meta); return *this; }
	Meta& Meta::operator/(const Meta& meta) { divide(meta); return *this; }
	Meta& Meta::operator+=(const Meta& meta) { plus(meta); return *this; }
	Meta& Meta::operator-=(const Meta& meta) { minus(meta); return *this; }
	Meta& Meta::operator*=(const Meta& meta) { multi(meta); return *this; }
	Meta& Meta::operator/=(const Meta& meta) { divide(meta); return *this; }

	void Meta::CalculationAdd(Calculation c,const std::vector<Meta>& meta) {
		if (meta.size() != operation_parameter_number.at(c))
			throw error::Math::expresssion_num_error();
		std::vector<Variable>&& variables{};
		for (Meta m : meta)
			switch (m.content.index()) {
			case 0: break;
			case 1: variables.push_back(*std::get<1>(m.content)); break;
			case 2: variables.insert(variables.end(), std::get<2>(m.content)->first.begin(), std::get<2>(m.content)->first.end()); break;
			}

		switch (content.index()) {
		case 0: {
			std::stack<std::variant<Meta, Calculation>>&& stack{};
			stack.push(c);
			for (size_t i = 0;i < meta.size();i++)
				stack.push(meta[i]);
			stack.push(std::get<long double>(content));
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				std::pair<std::vector<Variable>,std::stack<std::variant<Meta,Calculation>>>{ variables,stack });
			break;
		}
		case 1: {
			std::stack<std::variant<Meta, Calculation>>&& stack{};
			Variable v = *std::get<std::unique_ptr<Variable>>(content);
			stack.push(c);
			for (size_t i = 0; i < meta.size(); i++)
				stack.push(meta[i]);
			stack.push(v);
			if (std::find(variables.begin(), variables.end(), v) == variables.end())
				variables.push_back(v);
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>{ variables, stack });
			break;
		}
		case 2: {
			std::vector<Variable>& content_variables = std::get<2>(content)->first;
			for (Variable v : variables)
				if (std::find(content_variables.begin(), content_variables.end(), v) == content_variables.end())
					content_variables.push_back(v);

			std::stack<std::variant<Meta, Calculation>>& stack = std::get<2>(content)->second;
			std::stack<std::variant<Meta, Calculation>> temp;
			for (; !stack.empty(); stack.pop()) {
				if (auto i = std::get_if<Calculation>(&stack.top()); i) {
					if (opera.at(c) > opera.at(*i)) {
						for (size_t j = 0; j < meta.size(); j++)
							temp.push(meta[j]);
						temp.push(c);
						break;
					}
				}
				temp.push(stack.top());
			}
			for (; !temp.empty(); temp.pop())
				stack.push(temp.top());
			break;
		}
		}
	}

	long double Meta::operator()(const std::map<Variable, long double>& m) { return calculate(m); }

	Meta& Meta::operator=(const Meta& meta) {
		switch (meta.content.index()){
			case 0: content = std::get<long double>(meta.content); break;
			case 1: content = std::make_unique<Variable>(*std::get<std::unique_ptr<Variable>>(meta.content));
				break;
			case 2: content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				*std::get<2>(meta.content));
				break;
		}
		return *this;
	}
}