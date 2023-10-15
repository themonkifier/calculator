#ifndef UNIT_HPP
#define UNIT_HPP

#include <string>
#include <cmath>

typedef long double num_t;

/**
 * @param symbol Shortened symbol (ex. "m")
 * @param quantity_magnitudes Exponents on the base units, in the order [Time, Length, Mass, Current, Temperature, Amount of substance, Intensity]
 * @param prefix_magnitudes Exponents from prefixes, like 3 from km = m*10^3
 */
struct Unit
{
    std::string symbol; // m
    num_t quantity_magnitudes[7]; // [Time, Length, Mass, Current, Temperature, Amount of substance, Intensity]
    int prefix_magnitude; // k from km -> 3 ()

    Unit();

    Unit(std::string _symbol);

    Unit(std::string _symbol, num_t _quantity_magnitudes[7], int _prefix_magnitude);

    inline bool operator==(const Unit o) const noexcept
    {
        for (int i = 0; i < 7; i++) if (quantity_magnitudes[i] != o.quantity_magnitudes[i]) return false;
        return symbol == o.symbol && prefix_magnitude == o.prefix_magnitude;
    }

    Unit& operator*=(const Unit& rhs);
    Unit& operator*=(const int rhs);
    friend Unit operator*(Unit lhs, const Unit& rhs);
    friend Unit operator*(Unit lhs, int rhs);
    friend Unit operator*(int lhs, Unit rhs);
    Unit& operator/=(const Unit& rhs);
    Unit& operator/=(const int rhs);
    friend Unit operator/(Unit lhs, const Unit& rhs);
    friend Unit operator/(Unit lhs, int rhs);
    friend Unit operator/(int lhs, Unit rhs);
    Unit& operator^=(const int& rhs);
    friend Unit operator^(Unit lhs, const int& rhs);
    std::size_t operator()(const Unit& u) const noexcept;
};

template<> struct std::hash<Unit>
{
    std::size_t operator()(const Unit& u) const noexcept
    {
        return 904982468069 * std::hash<std::string>{}(u.symbol) % 993523647409 + 830434557313;
    }
};

inline std::ostream &operator<<(std::ostream &os, Unit u)
{
    std::string ret = "(" + u.symbol;
    for (int i = 0; i < 7; i++) ret += ", " + std::to_string(u.quantity_magnitudes[i]);
    ret += ", " + std::to_string(u.prefix_magnitude);
    ret += ")";

    return std::operator<<(os, ret);
}

inline std::string to_string(Unit u)
{
    std::string ret = "(" + u.symbol;
    for (int i = 0; i < 7; i++) ret += ", " + std::to_string(u.quantity_magnitudes[i]);
    ret += ", " + std::to_string(u.prefix_magnitude);
    ret += ")";

    return ret;
}

#endif