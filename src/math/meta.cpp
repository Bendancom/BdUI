#include "math/meta.hpp"

namespace BdUI {
	const std::regex& Variable::variable_regex() {
		static const std::regex r("([A-Za-z])(?:_(?:([A-Za-z])|(?:\\{([\\w-,^_]+)\\})))?");
		return r;
	}
	const std::vector<std::variant<int, std::string>> Variable::no_subscript = std::vector<std::variant<int, std::string>>();

	const std::map<Meta::Calculation, unsigned char>& Meta::opera(){
		static const std::map<Calculation, unsigned char> o{
			{Calculation::plus,0},	{Calculation::minus,0},
			{Calculation::multi,1},	{Calculation::divide,1},
			{Calculation::log,2},	{Calculation::pow,2},	
			{Calculation::sin,3},	{Calculation::cos,3},	{Calculation::fac,3},{Calculation::ceil,3},{Calculation::floor,3},{Calculation::abs,3},
			{Calculation::sum,4},	{Calculation::intergeral,4},	{Calculation::derivate,4},	{Calculation::multiply,4}
		};
		return o;
	}
	
	const std::map<std::string, Meta::Calculation>& Meta::function_name(){
		static const std::map<std::string, Calculation> f{
			{"sum",Calculation::sum}, {"cos",Calculation::cos}, {"sin",Calculation::sin},
			{"abs",Calculation::abs},{"floor",Calculation::floor},{"ceil",Calculation::ceil},
			{"pow",Calculation::pow},{"fac",Calculation::fac}
		};
		return f;
	}

	const std::map<char, Meta::Calculation>& Meta::opera_calculation(){
		static const std::map<char, Calculation> o{
			{'+',Calculation::plus},	{'-',Calculation::minus},
			{'*',Calculation::multi},	{'/',Calculation::divide},
			{'!',Calculation::fac},		{'^',Calculation::pow},
			{'(',Calculation::left_bracket},{')',Calculation::right_bracket},
		};
		return o;
	}
	
	const std::map<Meta::Calculation, unsigned char>& Meta::operation_parameter_number(){
		static const std::map<Calculation, unsigned char> o{
			{Calculation::fac,0},{Calculation::sin,0},{Calculation::cos,0},{Calculation::floor,0},{Calculation::ceil,0},{Calculation::abs,0},
			{Calculation::plus,1}, {Calculation::multi,1}, {Calculation::divide,1}, {Calculation::minus,1},
			{Calculation::derivate,1}, {Calculation::pow,1}, {Calculation::log,1},
			{Calculation::sum,2},{Calculation::multiply,2},{Calculation::intergeral,2}
		};
		return o;
	}
	const std::regex& Meta::vari_with_num_regex() {
		static const std::regex r("([0-9\\.\\-]+)?([A-Za-z_{}]*)");
		return r;
	}
	
	
	Variable::Variable(const char* ch) : std::pair < Character,
		std::vector<std::variant<int, std::string>>>{ '\0',std::vector<std::variant<int,std::string>>() } {
		*this = Variable(std::string(ch));
	}
	Variable::Variable(const std::string& Latex) : std::pair < Character,
		std::vector<std::variant<int, std::string>>>{'\0',std::vector<std::variant<int,std::string>>()}
	{
		if (std::smatch match; std::regex_match(Latex, match, variable_regex())) {
			first = match[1].str().c_str();
			if (match[2].matched)
				second.push_back(match[2].str());
			else if (match[3].matched) {
				size_t pos_next = 0;
				for (size_t pos = 0; pos != std::string::npos;pos = match[3].str().find(',', pos_next + 1)) {
					pos_next = match[3].str().find(',', pos + 1);
					std::string str = match[3].str().substr(pos, pos_next - pos);
					bool IsNUm = true;
					for (char c : str)
						if ((c < '0' || c >'9') && c != '-') {
							IsNUm = false;
							break;
						}
					if (IsNUm) second.push_back(std::stoi(str));
					else second.push_back(str);
				}
			}
		}
		else throw error::Math::variable_string_error();
	}
	std::vector<Variable> Variable::GetVariables(const std::string& Latex) {
		std::vector<Variable>&& variables{};
		std::smatch match;
		for (std::string str = Latex;!str.empty(); str = match.suffix().str())
			if (std::regex_search(str, match, variable_regex()))
				variables.push_back(match[0].str());
			else throw error::Math::variable_string_error();
		return variables;
	}

	Meta Meta::StrToVariables(const std::string& str) {
		if (std::smatch match; std::regex_match(str, match, vari_with_num_regex())) {
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
	const Meta& Meta::CheckVariable(const Meta& meta, std::vector<Variable>& variables) {
		switch (meta.content.index()) {
		case 0:break;
		case 1: if (auto iter = *std::get<1>(meta.content);
			std::find(variables.begin(), variables.end(), iter) == variables.end()) variables.push_back(iter);
			break;
		case 2: {
			std::vector<Variable>& temp = std::get<2>(meta.content)->first;
			for (Variable v : temp)
				if (std::find(variables.begin(), variables.end(), v) == variables.end()) variables.push_back(v);
			break;
		}
		}
		return meta;
	}
	void Meta::CalculationAdd(const Calculation& c, std::stack<Calculation>& calcul, std::stack<std::variant<Meta, Calculation>>& stack) {
		if (c == Calculation::left_bracket)
			calcul.push(c);
		else if (c == Calculation::right_bracket) {
			for (; calcul.top() != Calculation::left_bracket; calcul.pop())
				stack.push(calcul.top());
			calcul.pop();
		}
		else while (true) {
			if (calcul.empty())
				calcul.push(c);
			else if (calcul.top() == Calculation::left_bracket)
				calcul.push(c);
			else if (opera().at(calcul.top()) < opera().at(c))
				calcul.push(c);
			else {
				stack.push(calcul.top());
				calcul.pop();
				continue;
			}
			break;
		}
	}
	void Meta::Initialization(const std::string& latex) {
		try { content = StrToVariables(latex).content; }
		catch (...) {
			std::string variable_num;
			std::stack<std::variant<Meta, Calculation>> stack;
			std::stack<Calculation> calcul;
			std::vector<Variable>&& variables{};
			for (size_t i = 0; i < latex.size(); i++) {
				if (latex[i] == ' ')
					continue;
				else if (std::map<char, Calculation>::const_iterator o = opera_calculation().find(latex[i]); o != opera_calculation().end()) {
					if (!variable_num.empty())
						stack.push(CheckVariable(StrToVariables(variable_num), variables));
					variable_num = std::string();
					CalculationAdd(o->second, calcul, stack);
				}
				else if (latex[i] == '\\') {
					if (!variable_num.empty()) throw error::Math::latex_string_invalid();

					std::string&& func_name{};
					size_t&& j = i + 1;
					for (; i < latex.size(); i++)
						if (latex[i] == '{') {
							func_name = latex.substr(j, i - j);
							break;
						}
					Calculation cal = function_name().at(func_name);

					std::string expression;
					long long n = 0;
					for (; i < latex.size(); i++) {
						if (latex[i] == '{')
							n++;
						else if (latex[i] == '}') {
							n--;
							if (n == 0) {
								Meta meta(expression);
								stack.push(CheckVariable(meta, variables));
								expression = std::string();
							}
							else if (n < 0) throw error::Math::latex_string_invalid();
							else expression += latex[i];
						}
						else expression += latex[i];
					}
					CalculationAdd(cal, calcul, stack);
				}
				else variable_num += latex[i];
			}
			if (!variable_num.empty())
				stack.push(CheckVariable(StrToVariables(variable_num), variables));
			for (; !calcul.empty(); calcul.pop())
				stack.push(calcul.top());
			std::stack<std::variant<Meta, Calculation>> r_stack;
			for (; !stack.empty(); stack.pop())
				r_stack.push(stack.top());
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>{ variables, r_stack });
		}
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
	Meta::Meta(const char* ch) { Initialization(std::string(ch)); };
	Meta::Meta(const std::string& latex) { 
		Initialization(latex); }

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
				for (size_t i = 0; i < operation_parameter_number().at(calcul) + 1; i++) {
					metas.push_back(temp.top());
					temp.pop();
				}

				static std::map<Calculation, std::string> str_name{
					{Calculation::plus ,"+"},{Calculation::minus,"-"},{Calculation::multi,"*"},{Calculation::divide,"/"},
					{Calculation::fac,"fac"},{Calculation::sum,"sum"},{Calculation::pow,"pow"}
				};

				switch (calcul) {
				case Calculation::fac: {
					long double floor = ::floor(_calculate(metas[0], date));
					if (floor < 0) throw error::Math::operator_error();
					else if (floor == 0) temp.push((long double)1);
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
				case Calculation::floor: temp.push(::floorl(_calculate(metas[0], date))); break;
				case Calculation::ceil: temp.push(::ceill(_calculate(metas[0], date))); break;
				case Calculation::abs: temp.push(::abs(_calculate(metas[0], date))); break;

				case Calculation::plus: temp.push(_calculate(metas[1], date) + _calculate(metas[0], date)); break;
				case Calculation::minus: temp.push(_calculate(metas[1], date) - _calculate(metas[0], date)); break;
				case Calculation::multi: temp.push(_calculate(metas[1], date) * _calculate(metas[0], date)); break;
				case Calculation::divide: temp.push(_calculate(metas[1], date) / _calculate(metas[0], date)); break;

				case Calculation::derivate: break;
				case Calculation::pow: temp.push(::powl(_calculate(metas[1], date), _calculate(metas[0], date))); break;
				case Calculation::log: temp.push(::logl(_calculate(metas[1], date) / ::logl(_calculate(metas[0], date)))); break;

				case Calculation::sum: {
					long double sum = 0;
					size_t i = _calculate(metas[2], date);
					size_t n = _calculate(metas[1], date);
					if (i < n) {
						for (; i <= n; i++) {
							date[*std::get<std::unique_ptr<Variable>>(metas[2].content)] = (long double)i;
							Meta meta = metas[0];
							sum += _calculate(meta, date);
						}
					}
					else {
						for (; i >= n;i--) {
							date[*std::get<std::unique_ptr<Variable>>(metas[2].content)] = (long double)i;
							Meta meta = metas[0];
							sum += _calculate(meta, date);
						}
					}
					temp.push(sum);
					break;
				}
				case Calculation::multiply: {
					long double multiply = 1;
					size_t i = _calculate(metas[2], date);
					size_t n = _calculate(metas[1], date);
					if (i < n) {
						for (; i <= n; i++) {
							date[*std::get<std::unique_ptr<Variable>>(metas[2].content)] = (long double)i;
							Meta meta = metas[0];
							multiply *= _calculate(meta, date);
						}
					}
					else {
						for (; i >= n; i--) {
							date[*std::get<std::unique_ptr<Variable>>(metas[2].content)] = (long double)i;
							Meta meta = metas[0];
							multiply *= _calculate(meta, date);
						}
					}
					break;
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
		case 1: {
			Variable v = *std::get<std::unique_ptr<Variable>>(meta.content);
			if (v.changable_subscript && !v.second.empty())
				for (size_t i =0; i < v.second.size();i++)
					if (v.second[i].index() == 1)
						if (auto j = date.find(std::get<1>(v.second[i])[0]); j != date.end())
							v.second[i] = (int)j->second;
			return date.at(v); 
		}
		case 2: return _calculate(std::get<2>(meta.content)->second, date);
		}
	}

	long double Meta::calculate(const std::map<Variable, long double>& date) const {
		switch (content.index()) {
		case 0: return std::get<long double>(content);
		case 1: return date.at(*std::get<std::unique_ptr<Variable>>(content));
		case 2: {
			if (std::get<2>(content)->first.size() > date.size()) throw error::Math::variable_num_error();
			std::map<Variable, long double> temp_date = date;
			std::stack<std::variant<Meta, Calculation>> temp_content = std::get<2>(content)->second;
			return _calculate(temp_content, temp_date);
		}
		}
	}

	Meta& Meta::pow(const Meta& meta) { MetaAdd(Calculation::pow, { meta }); return *this; }
	Meta& Meta::log(const Meta& meta) { MetaAdd(Calculation::log, { meta }); return *this; }
	Meta& Meta::plus(const Meta& meta) { MetaAdd(Calculation::plus, { meta }); return *this; }
	Meta& Meta::minus(const Meta& meta) { MetaAdd(Calculation::minus, { meta }); return *this; }
	Meta& Meta::multi(const Meta& meta) { MetaAdd(Calculation::multi, { meta }); return *this; }
	Meta& Meta::divide(const Meta& meta) { MetaAdd(Calculation::divide, { meta }); return *this; }
	Meta& Meta::sum(const Meta& i,const Meta& n) { MetaAdd(Calculation::sum, { i,n }); return *this; }
	Meta& Meta::fac() { MetaAdd(Calculation::fac); return *this; }
	Meta& Meta::sin() { MetaAdd(Calculation::sin); return *this; }
	Meta& Meta::cos() { MetaAdd(Calculation::cos); return *this; }
	Meta& Meta::floor() { MetaAdd(Calculation::floor); return *this; }
	Meta& Meta::ceil() { MetaAdd(Calculation::ceil); return *this; }
	Meta& Meta::abs() { MetaAdd(Calculation::abs); return *this; }

	Meta& Meta::operator+(const Meta& meta) { plus(meta); return *this; }
	Meta& Meta::operator-(const Meta& meta) { minus(meta); return *this; }
	Meta& Meta::operator*(const Meta& meta) { multi(meta); return *this; }
	Meta& Meta::operator/(const Meta& meta) { divide(meta); return *this; }
	Meta& Meta::operator+=(const Meta& meta) { plus(meta); return *this; }
	Meta& Meta::operator-=(const Meta& meta) { minus(meta); return *this; }
	Meta& Meta::operator*=(const Meta& meta) { multi(meta); return *this; }
	Meta& Meta::operator/=(const Meta& meta) { divide(meta); return *this; }

	void Meta::MetaAdd(Calculation c,const std::vector<Meta>& meta) {
		if (meta.size() != operation_parameter_number().at(c))
			throw error::Math::expresssion_num_error();
		std::vector<Variable>&& variables{};
		for (Meta m : meta)
			switch (m.content.index()) {
			case 0: break;
			case 1: variables.push_back(*std::get<1>(m.content)); break;
			case 2: variables.insert(variables.end(), std::get<2>(m.content)->first.begin(), std::get<2>(m.content)->first.end()); break;
			}
		switch (content.index()) {
		case 0:break;
		case 1:
			if (Variable& v = *std::get<std::unique_ptr<Variable>>(content);
				std::find(variables.begin(), variables.end(), v) == variables.end())
			variables.push_back(v);
			break;
		case 2: {
			std::vector<Variable>& content_variables = std::get<2>(content)->first;
			for (Variable v : variables)
				if (std::find(content_variables.begin(), content_variables.end(), v) == content_variables.end())
					content_variables.push_back(v);
			break;
		}
		}
		if (opera().find(c)->second == 4) {
			Meta&& meta = Meta(*this);
			std::stack<std::variant<Meta, Calculation>>&& stack{};
			stack.push(meta);
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(variables, stack);
		}
		switch (content.index()) {
		case 0: {
			std::stack<std::variant<Meta, Calculation>>&& stack{};
			stack.push(c);
			for (size_t i = 0; i < meta.size(); i++)
				stack.push(meta[i]);
			stack.push(std::get<long double>(content));
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>{ variables, stack });
			break;
		}
		case 1: {
			std::stack<std::variant<Meta, Calculation>>&& stack{};
			Variable& v = *std::get<std::unique_ptr<Variable>>(content);
			stack.push(c);
			for (size_t i = 0; i < meta.size(); i++)
				stack.push(meta[i]);
			stack.push(v);
			content = std::make_unique<std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>>(
				std::pair<std::vector<Variable>, std::stack<std::variant<Meta, Calculation>>>{ variables, stack });
			break;
		}
		case 2: {
			std::stack<std::variant<Meta, Calculation>>& stack = std::get<2>(content)->second;
			std::stack<std::variant<Meta, Calculation>> temp;
			for (; !stack.empty(); stack.pop()) {
				if (auto i = std::get_if<Calculation>(&stack.top()); i) {
					if (opera().at(c) > opera().at(*i)) {
						for (size_t j = 0; j < meta.size(); j++)
							temp.push(meta[meta.size() - 1 - j]);
						temp.push(c);
						break;
					}
				}
				temp.push(stack.top());
			}
			stack.push(c);
			for (size_t i = 0; i < meta.size(); i++)
				stack.push(meta[i]);
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