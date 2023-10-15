#ifndef NUMBER_HPP
#define NUMBER_HPP

#include <stdexcept>
#include <cmath>

#include "unit.hpp"

struct Number
{
    num_t val;
    Unit unit;

    /**
     * @brief Creates a new Number, with a default Unit (no unit), and a value of 0
     */
    Number();

    /**
     * @brief Creates a new Number, with a default Unit (no unit)
     */
    Number(num_t _val);
    Number(num_t _val, Unit _unit);
    inline bool operator==(const Number& o) const noexcept
    {
        return unit == o.unit && val == o.val;
    }
    Number& operator+=(const Number& rhs);
    friend Number operator+(Number lhs, const Number& rhs);
    Number& operator-=(const Number& rhs);
    friend Number operator-(Number lhs, const Number& rhs);
    Number& operator*=(const Number& rhs);
    friend Number operator*(Number lhs, const Number& rhs);
    Number& operator/=(const Number& rhs);
    friend Number operator/(Number lhs, const Number& rhs);
    Number& operator%=(const Number& rhs);
    friend Number operator%(Number lhs, const Number& rhs);
    Number& operator^=(const Number& rhs);
    friend Number operator^(Number lhs, const Number& rhs);
};

inline std::ostream &operator<<(std::ostream &os, Number num)
{
    return std::operator<<(os, "[" + std::to_string(num.val) + " " + to_string(num.unit) + "]");
}

#endif