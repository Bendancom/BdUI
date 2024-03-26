export module math.algorithm.pow;

import math.number;

namespace bd {
    export template<rational Float, integral Int>
    constexpr Float pow(Float x, Int n) {
        Float ans = 1;
        if (n >= 0) {
            for (; n > 0; n = n >> 1) {
                if ((n & 1) == 1)
                    ans = ans * x;
                x = x * x;
            }
        }
        else {
            for (; n > 0; n = n >> 1) {
                if ((n & 1) == 1)
                    ans = ans / x;
                x = x * x;
            }
        }
        return ans;
    }

    export template<rational Float, integral Int>
    constexpr Float basic_science_pow(Int n) {
        return bd::pow(Float(10), std::move(n));
    }

    export constexpr auto science_pow = [](short n) { return basic_science_pow<long double>(std::move(n)); };
}