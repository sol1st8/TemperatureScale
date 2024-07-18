#include <cassert>
#include <cmath>

const double EPSILON = 0.001;

bool AreEqual(double d1, double d2) { return std::abs(d1 - d2) < EPSILON; }

namespace temperature {

enum class Scale { CELSIUS, FAHRENHEIT, KELVIN };

template <Scale S>
class Quantity {
  public:
    explicit Quantity(double a) : amount(a) {}

    explicit operator double() const {
        return amount;
    }

  private:
    const double amount;
};

template <Scale S>
inline bool operator==(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return AreEqual(static_cast<double>(lhs), static_cast<double>(rhs));
}

template <Scale S>
inline bool operator!=(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return !(lhs == rhs);
}

template <Scale S>
inline bool operator<(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return lhs < rhs;
}

template <Scale S>
inline bool operator>(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return rhs < lhs;
}

template <Scale S>
inline bool operator<=(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return !(lhs > rhs);
}

template <Scale S>
inline bool operator>=(const Quantity<S>& lhs, const Quantity<S>& rhs) {
    return !(lhs < rhs);
}

template <Scale S>
Quantity<S> operator+(const Quantity<S>& q1, const Quantity<S>& q2) {
    return Quantity<S>(static_cast<double>(q1) + static_cast<double>(q2));
}

template <Scale S>
Quantity<S> operator-(const Quantity<S>& q1, const Quantity<S>& q2) {
    return Quantity<S>(static_cast<double>(q1) - static_cast<double>(q2));
}

template <Scale S, Scale R>
struct ConversionTraits {
    static double Convert(double value) = delete;
};

template <>
struct ConversionTraits<Scale::CELSIUS, Scale::KELVIN> {
    static double Convert(double value) {
        return value + 273.15;
    }
};

template <>
struct ConversionTraits<Scale::KELVIN, Scale::CELSIUS> {
    static double Convert(double value) {
        return value - 273.15;
    }
};

template <>
struct ConversionTraits<Scale::CELSIUS, Scale::FAHRENHEIT> {
    static double Convert(double value) {
        return (value * 9) / 5 + 32;
    }
};

template <>
struct ConversionTraits<Scale::FAHRENHEIT, Scale::CELSIUS> {
    static double Convert(double value) {
        return (value - 32) * 5 / 9;
    }
};

template <>
struct ConversionTraits<Scale::FAHRENHEIT, Scale::KELVIN> {
    static double Convert(double value) {
        return (value + 459.67) * 5 / 9;
    }
};

template <>
struct ConversionTraits<Scale::KELVIN, Scale::FAHRENHEIT> {
    static double Convert(double value) {
        return (value * 9) / 5 - 459.67;
    }
};

template <Scale R, Scale S>
Quantity<R> temperature_cast(const Quantity<S>& q) {
    return Quantity<R>(ConversionTraits<S, R>::Convert(static_cast<double>(q)));
}

namespace temperature_scale_literals {

Quantity<Scale::CELSIUS> operator""_deg(long double amount) {
    return Quantity<Scale::CELSIUS>{static_cast<double>(amount)};
}

Quantity<Scale::FAHRENHEIT> operator""_f(long double amount) {
    return Quantity<Scale::FAHRENHEIT>{static_cast<double>(amount)};
}

Quantity<Scale::KELVIN> operator""_k(long double amount) {
    return Quantity<Scale::KELVIN>{static_cast<double>(amount)};
}

} // namespace temperature_scale_literals

} // namespace temperature

int main() {
    using namespace temperature;
    using namespace temperature_scale_literals;

    auto t1{36.5_deg};
    auto t2{79.0_f};
    auto t3{100.0_k};

    {
        auto tf = temperature_cast<Scale::FAHRENHEIT>(t1);
        auto tc = temperature_cast<Scale::CELSIUS>(tf);
        assert(t1 == tc);
    }

    {
        auto tk = temperature_cast<Scale::KELVIN>(t1);
        auto tc = temperature_cast<Scale::CELSIUS>(tk);
        assert(t1 == tc);
    }

    {
        auto tc = temperature_cast<Scale::CELSIUS>(t2);
        auto tf = temperature_cast<Scale::FAHRENHEIT>(tc);
        assert(t2 == tf);
    }

    {
      auto tk = temperature_cast<Scale::KELVIN>(t2);
      auto tf = temperature_cast<Scale::FAHRENHEIT>(tk);
      assert(t2 == tf);
   }

   {
      auto tc = temperature_cast<Scale::CELSIUS>(t3);
      auto tk = temperature_cast<Scale::KELVIN>(tc);
      assert(t3 == tk);
   }

   {
      auto tf = temperature_cast<Scale::FAHRENHEIT>(t3);
      auto tk = temperature_cast<Scale::KELVIN>(tf);
      assert(t3 == tk);
   }
}
