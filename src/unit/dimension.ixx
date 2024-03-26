export module unit:dimension;

namespace bd{
    export class dimension{
    public:
        char Length;
        char Mass;
        char Time;
        char Current;
        char Temperature;
        char Mole;
        char Luminance;

        constexpr dimension() : Length{0},Mass{0},Time{0},Current{0},Temperature{0},Mole{0},Luminance{0} {}
        constexpr dimension(char length,char mass,char time,char current,char temperature,char mole,char luminance)
            : Length(length),Mass(mass),Time(time),Current(current),Temperature(temperature),Mole(mole),Luminance(luminance) {}
        
        constexpr dimension operator*(const dimension&) const;
        constexpr dimension operator/(const dimension&) const;
        
        constexpr bool operator==(const dimension&) const;

        static const dimension L; // Length
        static const dimension M; // Mass
        static const dimension T; // Time
        static const dimension I; // Current
        static const dimension Θ; // Temperature
        static const dimension N; // Mole
        static const dimension J; // Luminance
    };
}