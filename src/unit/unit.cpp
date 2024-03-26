module unit;

namespace bd{
    constexpr dimension dimension::operator*(const dimension& d) const {
        dimension tmp = *this;
        tmp.Length += d.Length;
        tmp.Mass += d.Mass;
        tmp.Time += d.Time;
        tmp.Current += d.Current;
        tmp.Temperature += d.Temperature;
        tmp.Mole += d.Mole;
        tmp.Luminance += d.Luminance;
        return tmp;
    }
    constexpr dimension dimension::operator/(const dimension& d) const{
        dimension tmp = *this;
        tmp.Length -= d.Length;
        tmp.Mass -= d.Mass;
        tmp.Time -= d.Time;
        tmp.Current -= d.Current;
        tmp.Temperature -= d.Temperature;
        tmp.Mole -= d.Mole;
        tmp.Luminance -= d.Luminance;
        return tmp;
    }

    constexpr bool dimension::operator==(const dimension& d) const{
        return  Length == d.Length & 
                Mass == d.Mass & 
                Time == d.Time & 
                Current == d.Current &
                Temperature == d.Temperature &
                Mole == d.Mole &
                Luminance == d.Luminance;
    }

    const dimension dimension::L = dimension{1,0,0,0,0,0,0};
    const dimension dimension::M = dimension{0,1,0,0,0,0,0};
    const dimension dimension::T = dimension{0,0,1,0,0,0,0};
    const dimension dimension::I = dimension{0,0,0,1,0,0,0};
    const dimension dimension::Θ = dimension{0,0,0,0,1,0,0};
    const dimension dimension::N = dimension{0,0,0,0,0,1,0};
    const dimension dimension::J = dimension{0,0,0,0,0,0,1};
}