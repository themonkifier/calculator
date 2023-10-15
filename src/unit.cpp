#include "../include/unit.hpp"

/* impl for struct Unit */
Unit::Unit()
{
    symbol = "";
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;
    prefix_magnitude = 1;
}

Unit::Unit(std::string _symbol)
{
    symbol = _symbol;
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;
    prefix_magnitude = 1;
}

Unit::Unit(std::string _symbol, num_t _quantity_magnitudes[7], int _prefix_magnitude)
{
    symbol = _symbol;
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] = _quantity_magnitudes[i];
    prefix_magnitude = _prefix_magnitude;
}

Unit& Unit::operator*=(const Unit& rhs)
{
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] += rhs.quantity_magnitudes[i];
    prefix_magnitude *= rhs.prefix_magnitude;
    if (symbol != "") symbol += "*";
    symbol += rhs.symbol;
    return *this;
}

Unit& Unit::operator*=(const int rhs)
{
    prefix_magnitude *= rhs;
    return *this;
}

Unit operator*(Unit lhs, const Unit& rhs)
{
    lhs *= rhs;
    return lhs;
}

Unit operator*(Unit lhs, int rhs)
{
    lhs *= rhs;
    return lhs;
}

Unit operator*(int lhs, Unit rhs)
{
    rhs *= lhs;
    return rhs;
}

Unit& Unit::operator/=(const Unit& rhs)
{
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] -= rhs.quantity_magnitudes[i];
    prefix_magnitude /= rhs.prefix_magnitude;
    if (symbol != "") symbol += "/";
    symbol += rhs.symbol;
    return *this;
}

Unit& Unit::operator/=(const int rhs)
{
    prefix_magnitude /= rhs;
    return *this;
}

Unit operator/(Unit lhs, const Unit& rhs)
{
    lhs /= rhs;
    return lhs;
}

Unit operator/(Unit lhs, int rhs)
{
    lhs /= rhs;
    return lhs;
}

Unit operator/(int lhs, Unit rhs)
{
    rhs /= lhs;
    return rhs;
}

Unit& Unit::operator^=(const int& rhs)
{
    for (int i = 0; i < 7; i++) quantity_magnitudes[i] *= rhs;
    prefix_magnitude = powl(prefix_magnitude, rhs);
    if (symbol != "") symbol += "^" + std::to_string(rhs);
    return *this;
}

Unit operator^(Unit lhs, const int& rhs)
{
    lhs ^= rhs;
    return lhs;
}

std::size_t Unit::operator()(const Unit& u) const noexcept
{
    return 98347233425474 * std::hash<std::string>{}(u.symbol) % 64818945416 + 89243487432;
}