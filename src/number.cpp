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
        if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't add unconformable units");
    this->val += rhs.val;
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
        if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't subtract unconformable units");
    this->val -= rhs.val;
    return *this;
}

Number operator-(Number lhs, const Number& rhs)
{
    lhs -= rhs;
    return lhs;
}

Number& Number::operator*=(const Number& rhs)
{
    this->unit *= rhs.unit;
    this->val *= rhs.val;
    return *this;
}

Number operator*(Number lhs, const Number& rhs)
{
    lhs *= rhs;
    return lhs;
}

Number& Number::operator/=(const Number& rhs)
{
    this->unit /= rhs.unit;
    this->val /= rhs.val;
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
        if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
            throw std::runtime_error("can't mod unconformable units");
    this->val = fmodl(this->val, rhs.val);
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
    this->unit ^= rhs.val;
    this->val = powl(this->val, rhs.val);
    return *this;
}

Number operator^(Number lhs, const Number& rhs)
{
    lhs ^= rhs;
    return lhs;
}