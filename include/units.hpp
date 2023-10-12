#ifndef UNITS_H
#define UNITS_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include <stdexcept>

#include <cmath>

typedef long double num_t;

/**
 * @param symbol Shortened symbol (ex. "m")
 * @param quantity_magnitudes Exponents on the base units, in the order [Time, Length, Mass, Current, Temperature, Amount of substance, Intensity]
 * @param prefix_magnitudes Exponents from prefixes, like 3 from km = m*10^3
 */
typedef struct Unit
{
    std::string symbol; // m
    num_t quantity_magnitudes[7]; // [Time, Length, Mass, Current, Temperature, Amount of substance, Intensity]
    int prefix_magnitude; // k from km -> 3 ()

    Unit()
    {
        symbol = "";
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;
        prefix_magnitude = 0;
    }

    Unit(std::string _symbol)
    {
        symbol = _symbol;
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;
        prefix_magnitude = 0;
    }

    Unit(std::string _symbol, num_t _quantity_magnitudes[7], int _prefix_magnitude)
    {
        symbol = _symbol;
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = _quantity_magnitudes[i];
        prefix_magnitude = _prefix_magnitude;
    }

    inline bool operator==(const Unit& o) const noexcept
    {
        for (int i = 0; i < 7; i++) if (quantity_magnitudes[i] != o.quantity_magnitudes[i]) return false;
        return symbol == o.symbol && prefix_magnitude == o.prefix_magnitude;
    }

    Unit& operator*=(const Unit& rhs)
    {
        for (int i = 0; i < 7; i++) this->quantity_magnitudes[i] += rhs.quantity_magnitudes[i];
        this->prefix_magnitude += rhs.prefix_magnitude;
        if (this->symbol != "") this->symbol += "*";
        this->symbol += rhs.symbol;
        return *this;
    }
 
    friend Unit operator*(Unit lhs, const Unit& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

    Unit& operator/=(const Unit& rhs)
    {
        for (int i = 0; i < 7; i++) this->quantity_magnitudes[i] -= rhs.quantity_magnitudes[i];
        this->prefix_magnitude -= rhs.prefix_magnitude;
        if (this->symbol != "") this->symbol += "/";
        this->symbol += rhs.symbol;
        return *this;
    }
 
    friend Unit operator/(Unit lhs, const Unit& rhs)
    {
        lhs /= rhs;
        return lhs;
    }

    Unit& operator^=(const int& rhs)
    {
        for (int i = 0; i < 7; i++) this->quantity_magnitudes[i] *= rhs;
        this->prefix_magnitude *= rhs;
        if (this->symbol != "") this->symbol += "^" + std::to_string(rhs);
        return *this;
    }
 
    friend Unit operator^(Unit lhs, const int& rhs)
    {
        lhs ^= rhs;
        return lhs;
    }

    std::size_t operator()(const Unit& u) const noexcept
    {
        return 98347233425474 * std::hash<std::string>{}(u.symbol) % 64818945416 + 89243487432;
    }
} Unit;

template<> struct std::hash<Unit>
{
    std::size_t operator()(const Unit& u) const noexcept
    {
        return 98347233425474 * std::hash<std::string>{}(u.symbol) % 648189416;
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

typedef struct Number
{
    num_t val;
    Unit unit;

    /**
     * @brief Creates a new Number, with a default Unit (no unit), and a value of 0
     */
    Number()
    {
        val = 0;
    }

    /**
     * @brief Creates a new Number, with a default Unit (no unit)
     */
    Number(num_t _val)
    {
        val = _val;
    }

    Number(num_t _val, Unit _unit)
    {
        val = _val;
        unit = _unit;
    }

    inline bool operator==(const Number& o) const noexcept
    {
        return unit == o.unit && val == o.val;
    }

    Number& operator+=(const Number& rhs)
    {
        for (int i = 0; i < 7; i++)
            if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
                throw std::runtime_error("can't add unconformable units");
        this->val += rhs.val;
        return *this;
    }
 
    friend Number operator+(Number lhs, const Number& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    
    Number& operator-=(const Number& rhs)
    {
        for (int i = 0; i < 7; i++)
            if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
                throw std::runtime_error("can't subtract unconformable units");
        this->val -= rhs.val;
        return *this;
    }
 
    friend Number operator-(Number lhs, const Number& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    
    Number& operator*=(const Number& rhs)
    {
        this->unit *= rhs.unit;
        this->val *= rhs.val;
        return *this;
    }
 
    friend Number operator*(Number lhs, const Number& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    
    Number& operator/=(const Number& rhs)
    {
        this->unit /= rhs.unit;
        this->val /= rhs.val;
        return *this;
    }
 
    friend Number operator/(Number lhs, const Number& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    
    Number& operator%=(const Number& rhs)
    {
        for (int i = 0; i < 7; i++)
            if (this->unit.quantity_magnitudes[i] != rhs.unit.quantity_magnitudes[i])
                throw std::runtime_error("can't mod unconformable units");
        this->val = fmodl(this->val, rhs.val);
        return *this;
    }
 
    friend Number operator%(Number lhs, const Number& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    
    Number& operator^=(const Number& rhs)
    {
        if (rhs.unit.symbol != "") throw std::runtime_error("can't raise to a power with units");
        this->unit ^= rhs.val;
        this->val = powl(this->val, rhs.val);
        return *this;
    }
 
    friend Number operator^(Number lhs, const Number& rhs)
    {
        lhs ^= rhs;
        return lhs;
    }
} Number;

inline std::ostream &operator<<(std::ostream &os, Number num)
{
    return std::operator<<(os, "[" + std::to_string(num.val) + " " + to_string(num.unit) + "]");
}

typedef std::variant<Number, std::string> Token;

extern std::unordered_map<std::string, int> prefixes;
extern std::unordered_set<Unit> units;
extern std::unordered_set<Unit>::iterator nullunit;

void setup_units();

std::unordered_set<Unit>::iterator find_unit_in_units(Unit unit);
bool unit_in_units(Unit unit);

#endif