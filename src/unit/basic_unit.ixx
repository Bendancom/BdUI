export module unit:basic_unit;

import :dimension;
import math.number;
import math.operation;

namespace bd{
    // SI unit
    export template<rational Num>
    class basic_unit {
    private:
        Num value;
        dimension dimen;
    public:
        constexpr basic_unit() : value{0},dimen(dimension()) {}
        constexpr basic_unit(const Num& value,const dimension& dimen = dimension()) : value(value),dimen(dimen){}
        constexpr basic_unit(const basic_unit<Num>& b_u) : value(b_u.value),dimen(b_u.dimen){}
        
        constexpr Num getValue() const noexcept { return value; }
        constexpr dimension getDimension() const noexcept { return dimen; }
        template<std::constructible_from<basic_unit<Num>>... T>
        constexpr void set(T... args) noexcept { basic_unit(args...); }

        constexpr basic_unit<Num> operator-() const noexcept {
            return basic_unit(-value,dimen);
        }
        constexpr basic_unit<Num> operator+(const basic_unit<Num>& b_u) const{
            static_assert(dimen == b_u.dimen,"Error:the dimension isn't equal");
            if (dimen == b_u.dimen){
                basic_unit<Num> tmp(value,dimen);
                tmp.value += b_u.value;
                return tmp;
            }
            else {

            }
        }
        constexpr basic_unit<Num> operator-(const basic_unit<Num>& b_u) const {
            static_assert(dimen == b_u.dimen,"Error:the dimension isn't equal");
            if (dimen == b_u.dimen){
                basic_unit<Num> tmp(value,dimen);
                tmp.value -= b_u.value;
                return tmp;
            }
            else {
                
            }
        }
        constexpr basic_unit<Num> operator*(const basic_unit<Num>& b_u) const noexcept {
            return basic_unit<Num>(value * b_u.value,dimen*b_u.dimen);
        }
        constexpr basic_unit<Num> operator/(const basic_unit<Num>& b_u) const noexcept {
            return basic_unit<Num>(value / b_u.value,dimen/b_u.dimen);
        }
    };

    export using unit = basic_unit<long double>;
}