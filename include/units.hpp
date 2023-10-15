#ifndef UNITS_H
#define UNITS_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <variant>
#include <algorithm>

#include "unit.hpp"
#include "number.hpp"

typedef std::variant<Number, std::string> Token;

extern std::unordered_map<char, int> prefixes;
extern std::unordered_set<Unit> units;
extern std::unordered_set<Unit>::iterator nullunit;

void setup_units();

Unit find_unit_in_units(Unit unit);
bool unit_in_units(Unit unit);

#endif