#include "../include/units.hpp"
#include <iostream>

std::unordered_map<char, int> prefixes;
std::unordered_set<Unit> units;
std::unordered_set<Unit>::iterator nullunit;

void setup_units()
{
    prefixes = {{'Q', 30}, {'R', 27}, {'Y', 24}, {'Z', 21}, {'E', 18}, {'P', 15}, {'T', 12}, {'G', 9}, {'M', 6}, {'k', 3}, {'h', 2}, {'D', 1},
                {'d', -1}, {'c', -2}, {'m', -3}, {'u', -6}, {'n', -9}, {'p', -12}, {'f', -15}, {'a', -18}, {'z', -21}, {'y', -24}, {'r', -27}, {'q', -30}};

    /* base units */
    Unit second;
    second.symbol = "s";
    second.quantity_magnitudes[0] = 1;

    Unit meter;
    meter.symbol = "m";
    meter.quantity_magnitudes[1] = 1;


    Unit gram;
    gram.symbol = "g";
    gram.quantity_magnitudes[2] = 1;

    Unit kilogram;
    kilogram = 1000 * gram;
    kilogram.prefix_magnitude = 1000;
    kilogram.symbol = "kg";

    Unit ampere;
    ampere.symbol = "A";
    ampere.quantity_magnitudes[3] = 1;

    Unit degree_kelvin;
    degree_kelvin.symbol = "dK";
    degree_kelvin.quantity_magnitudes[4] = 1;

    Unit mole;
    mole.symbol = "mol";
    mole.quantity_magnitudes[5] = 1;

    Unit candela;
    candela.symbol = "cd";
    candela.quantity_magnitudes[6] = 1;

    /* derived units */
    Unit radian;
    radian.symbol = "rad";

    Unit steradian;
    steradian.symbol = "sr";

    Unit hertz;
    hertz = second^(-1);
    hertz.symbol = "Hz";

    Unit newton;
    newton = kilogram * meter / (second ^ 2);
    newton.symbol = "N";

    Unit pascal;
    pascal = newton / (meter^2);
    pascal.symbol = "Pa";

    Unit joule;
    joule = newton * meter;
    joule.symbol = "J";

    Unit watt;
    watt = joule / second;
    watt.symbol = "W";

    Unit coulomb;
    coulomb = ampere * second;
    coulomb.symbol = "C";

    Unit volt;
    volt = watt / ampere;
    volt.symbol = "V";

    Unit ohm;
    ohm = volt / ampere;
    ohm.symbol = "Ω";

    Unit siemens;
    siemens = ohm^(-1);
    siemens.symbol = "S";

    Unit weber;
    weber = volt * second;
    weber.symbol = "Wb";

    Unit tesla;
    tesla = weber / (meter^2);
    tesla.symbol = "T";

    Unit henry;
    henry = weber / ampere;
    henry.symbol = "H";

    Unit degree_celsius;
    degree_celsius = degree_kelvin;
    degree_celsius.symbol = "dC";

    Unit lumen;
    lumen = candela;
    lumen.symbol = "lm";

    Unit lux;
    lux = candela / (meter^2);
    lux.symbol = "lx";

    Unit bequerel;
    bequerel = second^(-1);
    bequerel.symbol = "Bq";

    Unit gray;
    gray = (meter^2) / (second^2);
    gray.symbol = "Gy";

    Unit sievert;
    sievert = (meter^2) / (second^2);
    sievert.symbol = "Sv";

    Unit katal;
    katal = mole / second;
    katal.symbol = "kat";
    
    Unit minute;
    minute = second * 60;
    minute.symbol = "min";

    Unit hour;
    hour = minute * 60;
    hour.symbol = "hr";

    Unit day;
    day = hour * 24;
    day.symbol = "day";

    Unit week;
    week = day * 7;
    week.symbol = "week";

    units = {second, meter, gram, ampere, degree_kelvin, mole, candela,
            radian, steradian, hertz, newton, pascal, joule, watt, coulomb,
            volt, ohm, siemens, weber, tesla, henry, degree_celsius, lumen,
            lux, bequerel, gray, sievert, katal, minute, hour, day, week};
    nullunit = units.end();
}

Unit find_unit_in_units(Unit unit)
{
    std::string_view symbol = unit.symbol;
    std::string_view temp;
    std::vector<std::string_view> units_in_symbol;
    long long multiplier = 1;

    std::size_t i = 0, previous = 0;
    do
    {
        while (i < symbol.size() && (symbol[i] == ' ' || symbol[i] == '*' || symbol[i] == '/')) i++;
        previous = i;
        if (isalpha(symbol[i])) while (i < symbol.size() && isalpha(symbol[i])) i++;
        
        if (prefixes.contains(symbol[previous]) && i > previous + 1)
        {
            multiplier *= powl(10, prefixes[symbol[previous]]);
            previous++;
        }
        temp = symbol.substr(previous, i - previous + 1);
        units_in_symbol.push_back(temp);

        previous = i++;
    } while (i < symbol.size());

    for (auto unit_to_find : units_in_symbol)
    {
        std::string_view symbol_without_prefix;
        if (unit_to_find.size() > 1) symbol_without_prefix = std::string_view(unit_to_find).substr(1);
        if (std::find_if(units.begin(), units.end(), [unit_to_find, symbol_without_prefix](const Unit& o){return unit_to_find == o.symbol || symbol_without_prefix == o.symbol;}) == nullunit)
            return Unit("nullunit");
    }

    if (units_in_symbol.size() < 1)
    {
        auto res = std::find_if(units.begin(), units.end(), [unit](const Unit& o)
            {
                for (std::size_t i = 0; i < 7; i++)
                {
                    if (unit.quantity_magnitudes[i] != o.quantity_magnitudes[i]) return false;
                }
                return unit.prefix_magnitude == o.prefix_magnitude;
            });
        if (res == nullunit) return Unit("nullunit");
        return multiplier * (*res);
    }
    std::string_view symbol_without_prefix;
    if (unit.symbol.size() > 1) symbol_without_prefix = std::string_view(unit.symbol).substr(1);
    auto res = std::find_if(units.begin(), units.end(), [unit, symbol_without_prefix](const Unit& o){return unit.symbol == o.symbol || symbol_without_prefix == o.symbol;});
    if (res == nullunit) return Unit("nullunit");
    return multiplier * (*res);
}

bool unit_in_units(Unit unit)
{
    std::string_view symbol_without_prefix;
    if (unit.symbol.size() > 1) symbol_without_prefix = std::string_view(unit.symbol).substr(1);
    return std::find_if(units.begin(), units.end(), [unit, symbol_without_prefix](const Unit& o){return unit.symbol == o.symbol || symbol_without_prefix == o.symbol;}) != nullunit;
}