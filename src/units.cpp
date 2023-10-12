#include "../include/units.hpp"
#include <iostream>

std::unordered_map<std::string, int> prefixes;
std::unordered_set<Unit> units;
std::unordered_set<Unit>::iterator nullunit;

void setup_units()
{
    prefixes = {{"Q", 30}, {"R", 27}, {"Y", 24}, {"Z", 21}, {"E", 18}, {"P", 15}, {"T", 12}, {"G", 9}, {"M", 6}, {"k", 3}, {"h", 2}, {"da", 1},
                {"d", -1}, {"c", -2}, {"m", -3}, {"μ", -6}, {"n", -9}, {"p", -12}, {"f", -15}, {"a", -18}, {"z", -21}, {"y", -24}, {"r", -27}, {"q", -30}};

    /* base units */
    Unit second;
    second.symbol = "s";
    second.quantity_magnitudes[0] = 1;

    Unit meter;
    meter.symbol = "m";
    meter.quantity_magnitudes[1] = 1;
    
    Unit kilogram;
    kilogram.symbol = "kg";
    kilogram.quantity_magnitudes[2] = 1;
    kilogram.prefix_magnitude = 3;

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

    units = {second, meter, kilogram, ampere, degree_kelvin, mole, candela,
            radian, steradian, hertz, newton, pascal, joule, watt, coulomb,
            volt, ohm, siemens, weber, tesla, henry};
    nullunit = units.end();
}

std::unordered_set<Unit>::iterator find_unit_in_units(Unit unit)
{
    std::string symbol = unit.symbol;
    std::string temp;
    std::vector<std::string> units_in_symbol;

    std::size_t i = 0, previous = 0;
    do
    {
        while (i < symbol.size() && (symbol[i] == ' ' || symbol[i] == '*' || symbol[i] == '/')) i++;
        previous = i;
        if (isalpha(symbol[i])) while (i < symbol.size() && isalpha(symbol[i])) i++;
        
        // if ((temp = symbol.substr(previous, i)) != "asdfhj")
        temp = symbol.substr(previous, i);
        units_in_symbol.push_back(temp);

        previous = i++;
    } while (i < symbol.size());

    for (auto unit_to_find : units_in_symbol)
    {
        if (std::find_if(units.begin(), units.end(), [unit_to_find](const Unit& o){return unit_to_find == o.symbol;}) == nullunit)
            return nullunit;
    }

    // auto it = std::find_if(units.begin(), units.end(), [unit](const Unit& o) {return unit.symbol == o.symbol;});
    if (units_in_symbol.size() < 1) return std::find_if(units.begin(), units.end(), [unit](const Unit& o)
    {
        for (std::size_t i = 0; i < 7; i++)
        {
            if (unit.quantity_magnitudes[i] != o.quantity_magnitudes[i]) return false;
        }
        return unit.prefix_magnitude == o.prefix_magnitude;
    });
    return std::find_if(units.begin(), units.end(), [unit](const Unit& o){return unit.symbol == o.symbol;});
}

bool unit_in_units(Unit unit)
{
    return std::find_if(units.begin(), units.end(), [unit](const Unit& o){return unit.symbol == o.symbol;}) != nullunit;
}