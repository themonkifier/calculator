#ifndef RUN_H
#define RUN_H

#include <locale>

#include "include/calculator.hpp"

class comma_numpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const
    { return ','; }

    virtual std::string do_grouping() const
    { return "\03"; }
};

class space_numpunct : public std::numpunct<char>
{
  protected:
    virtual char do_thousands_sep() const
    { return ' '; }

    virtual std::string do_grouping() const
    { return "\03"; }
};

#endif