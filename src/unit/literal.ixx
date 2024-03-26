export module unit:literal;

import :metric_prefix;
import :basic_unit;
import :dimension;
import math.algorithm.pow;

namespace bd
{
    export constexpr unit operator""m(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""m(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""g(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""g(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""s(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""s(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""A(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""A(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""K(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""K(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""mol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""mol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""cd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""cd(unsigned long long num){ return unit(num,dimension::J); }

    export constexpr unit operator""Qm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Qm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Qg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Qg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Qs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Qs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""QA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""QA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""QK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""QK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Qmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Qmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Qcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Qcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""Ym(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Ym(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Yg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Yg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Ys(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Ys(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""YA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""YA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""YK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""YK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Ymol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Ymol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Ycd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Ycd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""Zm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Zm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Zg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Zg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Zs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Zs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""ZA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""ZA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""ZK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""ZK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Zmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Zmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Zcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Zcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""Em(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Em(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Eg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Eg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Es(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Es(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""EA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""EA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""EK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""EK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Emol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Emol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Ecd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Ecd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""Pm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Pm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Pg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Pg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Ps(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Ps(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""PA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""PA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""PK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""PK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Pmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Pmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Pcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Pcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""Tm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""Tm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""Tg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""Tg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""Ts(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""Ts(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""TA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""TA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""TK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""TK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""Tmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""Tmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""Tcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""Tcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""GMkhdadm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""GMkhdadm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""GMkhdadg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""GMkhdadg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""GMkhdads(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""GMkhdads(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""GMkhdadA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""GMkhdadA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""GMkhdadK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""GMkhdadK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""GMkhdadmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""GMkhdadmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""GMkhdadcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""GMkhdadcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""cm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""cm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""cg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""cg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""cs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""cs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""cA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""cA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""cK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""cK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""cmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""cmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""ccd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""ccd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""mm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""mm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""mg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""mg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""ms(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""ms(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""mA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""mA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""mK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""mK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""mmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""mmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""mcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""mcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""μm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""μm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""μg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""μg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""μs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""μs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""μA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""μA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""μK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""μK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""μmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""μmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""μcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""μcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""nm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""nm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""ng(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""ng(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""ns(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""ns(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""nA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""nA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""nK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""nK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""nmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""nmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""ncd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""ncd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""pm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""pm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""pg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""pg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""ps(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""ps(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""pA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""pA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""pK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""pK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""pmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""pmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""pcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""pcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""fm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""fm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""fg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""fg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""fs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""fs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""fA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""fA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""fK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""fK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""fmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""fmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""fcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""fcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""am(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""am(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""ag(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""ag(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""as(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""as(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""aA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""aA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""aK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""aK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""amol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""amol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""acd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""acd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""zm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""zm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""zg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""zg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""zs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""zs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""zA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""zA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""zK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""zK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""zmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""zmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""zcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""zcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""ym(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""ym(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""yg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""yg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""ys(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""ys(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""yA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""yA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""yK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""yK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""ymol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""ymol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""ycd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""ycd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""rm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""rm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""rg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""rg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""rs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""rs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""rA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""rA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""rK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""rK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""rmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""rmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""rcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""rcd(unsigned long long num){ return unit(num,dimension::J); }
    export constexpr unit operator""qm(long double num){ return unit(num,dimension::L); }
    export constexpr unit operator""qm(unsigned long long num){ return unit(num,dimension::L); }
    export constexpr unit operator""qg(long double num){ return unit(num,dimension::M); }
    export constexpr unit operator""qg(unsigned long long num){ return unit(num,dimension::M); }
    export constexpr unit operator""qs(long double num){ return unit(num,dimension::T); }
    export constexpr unit operator""qs(unsigned long long num){ return unit(num,dimension::T); }
    export constexpr unit operator""qA(long double num){ return unit(num,dimension::I); }
    export constexpr unit operator""qA(unsigned long long num){ return unit(num,dimension::I); }
    export constexpr unit operator""qK(long double num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""qK(unsigned long long num){ return unit(num,dimension::Θ); }
    export constexpr unit operator""qmol(long double num){ return unit(num,dimension::N); }
    export constexpr unit operator""qmol(unsigned long long num){ return unit(num,dimension::N); }
    export constexpr unit operator""qcd(long double num){ return unit(num,dimension::J); }
    export constexpr unit operator""qcd(unsigned long long num){ return unit(num,dimension::J); }

}