export module math.operation;

import <concepts>;
import math.number;

namespace bd {
    /*
    export class plus_equation {
        template<typename Self, typename Arg> requires requires(Self self, Arg arg) { std::copyable<Self>; {self + arg} -> std::same_as<Self>; }
        Self& operator+=(this Self&& self, Arg arg) {
            self = self + arg;
            return self;
        }
    };
    export class plusplus_equation{
        template<typename Self> requires requires(Self self) { std::copyable<Self>; {self++} -> std::same_as<Self>;}
        Self& operator++(this Self&& self,int){
            Self tmp = self;
            self++;
            return tmp;
        }
    };
    export class minus_equation{
        template<typename Self, typename Arg> requires requires(Self self, Arg arg) { std::copyable<Self>; {self - arg} -> std::same_as<Self>; }
        Self& operator-=(this Self&& self, Arg arg) {
            self = self - arg;
            return self;
        }
    };
    export class minusminus_equation{
        template<typename Self> requires requires(Self self) { std::copyable<Self>; {self--} -> std::same_as<Self>; }
        Self& operator--(this Self&& self,int){
            Self tmp = self;
            self--;
            return tmp;
        }
    };
    export class times_equation{
        template<typename Self, typename Arg> requires requires(Self self, Arg arg) { std::copyable<Self>; {self * arg} -> std::same_as<Self>; }
        Self& operator*=(this Self&& self, Arg arg) {
            self = self - arg;
            return self;
        }
    };
    export class divide_equation{
        template<typename Self, typename Arg> requires requires(Self self, Arg arg) { std::copyable<Self>; {self / arg} -> std::same_as<Self>; }
        Self& operator/=(this Self&& self, Arg arg) {
            self = self - arg;
            return self;
        }
    };
    export class fomular_4_equation : public plus_equation, minus_equation,times_equation,divide_equation {};
    */
}