#include "../include/number.hpp"

Number::Number()
{
    val = 0;
}

Number::Number(num_t _val)
{
    val = _val;
}

Number::Number(num_t _val, Unit _unit)
{
    val = _val;
    unit = _unit;
}

Number& Number::operator+=(const Number& rhs)
{
    for (int i = 0; i < 7; i++)
        if (unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't add unconformable units");
    val += rhs.val * rhs.unit.prefix_magnitude / unit.prefix_magnitude;
    return *this;
}

Number operator+(Number lhs, const Number& rhs)
{
    lhs += rhs;
    return lhs;
}

Number& Number::operator-=(const Number& rhs)
{
    for (int i = 0; i < 7; i++)
        if (unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't subtract unconformable units");
    val -= rhs.val * rhs.unit.prefix_magnitude / unit.prefix_magnitude;
    return *this;
}

Number operator-(Number lhs, const Number& rhs)
{
    lhs -= rhs;
    return lhs;
}

Number& Number::operator*=(const Number& rhs)
{
    unit *= rhs.unit;
    val *= rhs.val;
    return *this;
}

Number operator*(Number lhs, const Number& rhs)
{
    lhs *= rhs;
    return lhs;
}

Number& Number::operator/=(const Number& rhs)
{
    unit /= rhs.unit;
    val /= rhs.val;
    return *this;
}

Number operator/(Number lhs, const Number& rhs)
{
    lhs /= rhs;
    return lhs;
}

Number& Number::operator%=(const Number& rhs)
{
    for (int i = 0; i < 7; i++)
        if (unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't mod unconformable units");
    val = fmodl(val, rhs.val);
    return *this;
}

Number operator%(Number lhs, const Number& rhs)
{
    lhs /= rhs;
    return lhs;
}

Number& Number::operator^=(const Number& rhs)
{
    if (rhs.unit.symbol != "") throw std::runtime_error("can't raise to a power with units");
    unit ^= rhs.val;
    val = powl(val, rhs.val);
    return *this;
}

Number operator^(Number lhs, const Number& rhs)
{
    lhs ^= rhs;
    return lhs;
}