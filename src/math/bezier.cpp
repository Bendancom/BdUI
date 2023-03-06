#include <math/bezier.hpp>

namespace BdUI {
	Bezier::Bezier(const std::initializer_list<Point>& points) : bezier_func{ {'x',1},{'y',1},{'z',1}},std::list<Point>(points) {
		bezier_func[0] = Meta(Variable{ 'x',{'i'} });
		bezier_func[1] = Meta(Variable{ 'y',{'i'} });
		bezier_func[2] = Meta(Variable{ 'z',{'i'} });
		for (int i = 0; i < 3; i++) {
			bezier_func[i] *= (Meta(1) - Meta('t')).pow(Meta('n') - Meta('i'));
			bezier_func[i] *= Meta('t').pow(Meta('i')) * Meta('n').fac();
			bezier_func[i] /= (Meta('n') - Meta('i')).fac() * Meta('i').fac();
			bezier_func[i] << Calculation::sum >> std::array<Character, 2>{'i', 'n'};
		}
	}
	std::array<std::pair<Character, double>, 3> Bezier::calculate(double t) const {
		std::map<Variable, double>&& temp{ {'i',0},{'n',this->size() - 1}};
		int k = 0;
		for (Point p : *this) {
			temp[Variable{'x', { k }}] = p.X;
			temp[Variable{'y', { k }}] = p.Y;
			temp[Variable{'z', { k }}] = p.Z;
			k++;
		}
		temp['t'] = t;
		std::vector<std::pair<Variable,double>> result = bezier_func.calculate(temp);
		return { 
			std::pair<Character,double>{result[0].first.first,result[0].second},
			std::pair<Character,double>{result[1].first.first,result[1].second},
			std::pair<Character,double>{result[2].first.first,result[2].second}
		};
	}
}