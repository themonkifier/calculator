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
    int prefix_magnitudes[7]; // [Time, Length, Mass, Current, Temperature, Amount of substance, Intensity]

    Unit()
    {
        symbol = "";

        //quantity_magnitudes;
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;

        //prefix_magnitudes;
        for (int i = 0; i < 7; i++) prefix_magnitudes[i] = 0;
    }

    Unit(std::string _symbol)
    {
        symbol = _symbol;

        //quantity_magnitudes;
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = 0;

        //prefix_magnitudes;
        for (int i = 0; i < 7; i++) prefix_magnitudes[i] = 0;
    }

    Unit(std::string _symbol, num_t _quantity_magnitudes[7], int _magnitudes[7])
    {
        symbol = _symbol;

        //quantity_magnitudes;
        for (int i = 0; i < 7; i++) quantity_magnitudes[i] = _quantity_magnitudes[i];

        //prefix_magnitudes;
        for (int i = 0; i < 7; i++) prefix_magnitudes[i] = _magnitudes[i];
    }

    inline bool operator==(const Unit& o) const
    {
        return symbol == o.symbol;
    }

    Unit& operator*=(const Unit& rhs)
    {
        for (int i = 0; i < 7; i++) this->quantity_magnitudes[i] += rhs.quantity_magnitudes[i];
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
        return 98347233425474 * std::hash<std::string>{}(u.symbol) % 648189416;
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
    for (int i = 0; i < 7; i++) ret += ", " + std::to_string(u.prefix_magnitudes[i]);
    ret += ")";

    return std::operator<<(os, ret);
}

inline std::string to_string(Unit u)
{
    std::string ret = "(" + u.symbol;
    for (int i = 0; i < 7; i++) ret += ", " + std::to_string(u.quantity_magnitudes[i]);
    for (int i = 0; i < 7; i++) ret += ", " + std::to_string(u.prefix_magnitudes[i]);
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
        //unit;
    }

    /**
     * @brief Creates a new Number, with a default Unit (no unit)
     */
    Number(num_t _val)
    {
        val = _val;
        unit = Unit();
    }

    Number(num_t _val, Unit _unit)
    {
        val = _val;
        unit = _unit;
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