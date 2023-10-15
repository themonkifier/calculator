#include "../include/tokenizer.hpp"

#ifndef M_PI
#define M_PI 3.14159265
#endif

#ifndef M_E
#define M_E 2.71828183
#endif

enum CurrentType current_type(std::string str, int i)
{
    if (is_number(str.substr(i, 1))) return CurrentType::Number;
    else if (is_operator(str.substr(i, 1))) return CurrentType::Operator;
    else if (str[i] == '(' || str[i] == ')') return CurrentType::Parenthesis;
    else if (str[i] == '\'' || str[i] == ' ') return CurrentType::None;
    return CurrentType::Function;
}

/**
 * @brief Replaces instances of constants (ex. "e", "pi") with their numerical values (3.14159, 2.71828),
 *  and replaces E or ee (for exponents) with *10^
 * @param input The input string, to be changed
 */
void replace_constants(std::deque<Token> &tokens)
{
    for (auto it = tokens.begin(); it != tokens.end(); it++)
    {
        if (it->index() == 1)
        {
            if (std::get<1>(*it) == "e" && (it == tokens.begin() || (it - 1)->index() == 0
                || std::get<1>(*(it - 1)) == "(" || std::get<1>(*(it - 1)) == ")"))
            {
                it = tokens.erase(it);
                it = tokens.insert(it, Number(M_E));
                if (it != tokens.begin() && (it == tokens.begin() || (it - 1)->index() == 0 || std::get<1>(*(it - 1)) == ")")) it = tokens.insert(it, "*");
            }
            else if (std::get<1>(*it) == "pi" && (it == tokens.begin() || (it - 1)->index() == 0
                    || std::get<1>(*(it - 1)) == "(" || std::get<1>(*(it - 1)) == ")"))
            {
                it = tokens.erase(it);
                it = tokens.insert(it, Number(M_PI));
                if (it != tokens.begin() && (it == tokens.begin() || (it - 1)->index() == 0 || std::get<1>(*(it - 1)) == ")")) it = tokens.insert(it, "*");
            }
            else if ((std::get<1>(*it) == "E" || std::get<1>(*it) == "ee") && (it == tokens.begin() || (it - 1)->index() == 0
                    || std::get<1>(*(it - 1)) == "(" || std::get<1>(*(it - 1)) == ")"))
            {
                it = tokens.erase(it) - 1;
                it = tokens.insert(it, "(") + 2;
                it = tokens.insert(it, "*") + 1;
                it = tokens.insert(it, Number(10)) + 1;
                it = tokens.insert(it, "^") + 2;
                it = tokens.insert(it, ")");
            }
        }
    }
}

/**
 * @brief Deals with negative signs ("-"s) that are before numbers and after
 *  operators, close parentheses, or the beginning of the string); and inserts "*"s
 *  where necessary (before functions or open parentheses)
 */
void replace_scalars(std::deque<Token> &tokens)
{
    auto it = tokens.begin();
    if (it != tokens.end() - 1 && it->index() == 1 && std::get<1>(*it) == "-")
    {
        it = tokens.erase(it);
        if (it->index() == 1 && (std::get<1>(*it) == "(" || is_function(std::get<1>(*it))))
        {
            it = tokens.insert(it, "*");
            it = tokens.insert(it, "-1");
        }
        else std::get<0>(*it).val *= -1;
    }
    while (it != tokens.end() - 1)
    {
        if (it->index() == 1 && std::get<1>(*it) == "-" && (it - 1)->index() == 1 && (is_operator(std::get<1>(*(it - 1)))
            || std::get<1>(*(it - 1)) == "(") && ((it + 1)->index() == 0 || is_function(std::get<1>(*(it + 1)))))
        {
            it = tokens.erase(it);

            *it = "-" + std::get<1>(*it);
        }
        else if (it->index() == 0 && (it + 1)->index() == 1 && (std::get<1>(*(it + 1)) == "(" || is_function(std::get<1>(*(it + 1)))))
        {
            it++;
            it = tokens.insert(it, "*");
        }
        else it++;
    }
}

void combine_numbers_and_units(std::deque<Token> &tokens)
{
    Unit unit;

    /* insert "(1" before a unit when it's directly after a unit or operator, and "*1" before a unit when it's after a ")";
       allows for expressions like "N s" or "N*s", where there are spaces or operators instead of the more explicit "1N*1s" */
    for (auto it = tokens.begin() + 1; it != tokens.end(); it++)
    {
        /* first case - "(1" */
        if (it->index() == 1 && unit_in_units(Unit(std::get<1>(*it))) && (it - 1)->index() == 1)
        {
            if ((unit_in_units(Unit(std::get<1>(*(it - 1)))) || is_operator(std::get<1>(*(it - 1)))))
            {
                it = tokens.insert(it, {"(", Number(1)}) + 2;
                std::size_t parentheses = 0;
                while (it != tokens.end() && (parentheses > 0 || (it != tokens.end() - 1 && ((it + 1)->index() == 0 || (std::get<1>(*(it + 1)) == "("
                        || std::get<1>(*(it + 1)) == ")" || std::get<1>(*(it + 1)) == "^")))))
                {
                    if (std::get<1>(*(it)) == "(")
                    {
                        parentheses++;
                    }
                    else if (std::get<1>(*(it)) == ")")
                    {
                        parentheses--;
                    }
                    it++;
                }
                if (it == tokens.end()) tokens.push_back(")");
                else it = tokens.insert(it + 1, ")");
            }
            /* second case - "*1" */
            else if (std::get<1>(*(it - 1)) == ")")
            {
                it = tokens.insert(it, {"*", Number(1)}) + 2;
            }
        }
    }

    for (auto it = tokens.begin(); it != tokens.end();)
    {
        if (it->index() == 1 
        && (unit = find_unit_in_units(Unit(std::get<1>(*it)))).symbol != "nullunit")
        {
            if (it != tokens.begin() && (it - 1)->index() == 0)
            {
                it = tokens.erase(it) - 1; // clear unit
                std::get<0>(*(it)).unit *= unit;
            }
            else
            {
                if (it != tokens.begin() && is_operator(std::get<1>(*(it - 1))))
                {
                    it = tokens.erase(it) - 1; // clear denominator unit
                    if (it == tokens.begin()) throw std::runtime_error("malformed expression: operator at the beginning of the input");
                    std::string op = std::get<1>(*(it));
                    it = tokens.erase(it) - 1; // clear operator
                    if (op == "*") std::get<0>(*(it)).unit *= unit;
                    else if (op == "/") std::get<0>(*(it)).unit /= unit;
                }
                else if (find_unit_in_units(Unit(std::get<1>(*(it - 1)))).symbol != "")
                {
                    it = tokens.erase(it) - 1;
                    std::get<0>(*(it)).unit *= unit;
                }
            }

            if (it->index() == 0 && std::get<0>(*it).unit.symbol != "" && (it + 1)->index() == 1 && std::get<1>(*(it + 1)) == "^")
            {
                it += 2;

                size_t exponentiations = 1;
                while (it != tokens.end() - 1 && it->index() == 0 && (it + 1)->index() == 1 && std::get<1>(*(it + 1)) == "^")
                {
                    it += 2;
                    exponentiations++;
                }
                
                if (it == tokens.end()) throw std::runtime_error("you ended with an operator");
                else if (it->index() != 0)
                    throw std::runtime_error("you tried raising to something that's not a number");
                
                num_t num = std::get<0>(*it).val;

                it = tokens.erase(it) - 1; // clear number
                it = tokens.erase(it) - 1; // clear "^"
                while (exponentiations > 1)
                {
                    it = tokens.erase(it) - 1;
                    it = tokens.erase(it) - 1;

                    num = powl(std::get<0>(*it).val, num);
                    exponentiations--;
                }

                std::get<0>(*it).unit ^= num;
            }
        }
        else it++;
    }
}

std::deque<Token> tokenize(std::string input, PrintFormat &pf)
{
    auto it = input.begin();
    while (it != input.end() && !isdigit(*it)) it++;
    while (it != input.end() && *it != ',' && *it != ' ') it++;
    if (*it == ',') pf = PrintFormat::Commas;
    else if (*it == ' ' && it != input.end() - 1 && isdigit(*(it + 1))) pf = PrintFormat::Spaces;

    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    input.erase(std::remove(input.begin(), input.end(), ','), input.end());
    
    std::deque<Token> tokens;
    enum CurrentType type = CurrentType::None;

    /* split into individual tokens */
    std::size_t i = 0, previous = 0;
    do
    {
        type = current_type(input, i);

        if (!(type == CurrentType::Operator || type == CurrentType::Parenthesis)) while (i < input.size() - 1 && type == current_type(input, i + 1) && input[i] != ' ') i++;
        if (type == CurrentType::Number) tokens.push_back(Number(std::stold(input.substr(previous, i - previous + 1)), Unit()));
        else tokens.push_back(input.substr(previous, i - previous + 1));

        previous = ++i;
    } while (i < input.size());

    replace_constants(tokens);
    replace_scalars(tokens);
    combine_numbers_and_units(tokens);
    if (tokens.back().index() == 1)
    {
        if (is_operator(std::get<1>(tokens.back()))) throw std::runtime_error("you ended with an operator");
        else if (is_function(std::get<1>(tokens.back()))) throw std::runtime_error("you ended with a function");
    }
    return tokens;
}

bool is_operator(std::string op)
{
    return op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "^";
}

bool is_number(std::string num)
{
    bool has_digits = false; /* we don't want "-" to be a number... before this caused so much extra work */
    for (std::string::iterator it = num.begin(); it != num.end(); it++)
    {
        if (!(isdigit(*it) || *it == '.' || *it == '-')) return false;
        else if (isdigit(*it) || *it == '.')
        {
            has_digits = true;
        }
    }
    return has_digits;
}

bool is_function(std::string f)
{
    if (f.size() > 0 && f[0] == '-') f = f.substr(1);

    return f == "sin" || f == "cos" || f == "tan"
        || f == "csc" || f == "sec" || f == "cot"

        || f == "arcsin" || f == "arccos" || f == "arctan"
        || f == "asin" || f == "acos" || f == "atan"
        || f == "arccsc" || f == "arcsec" || f == "arccot"
        || f == "acsc" || f == "asec" || f == "acot"
        
        || f == "sinh" || f == "cosh" || f == "tanh"
        || f == "csch" || f == "sech" || f == "coth"

        || f == "arcsinh" || f == "arccosh" || f == "arctanh"
        || f == "asinh" || f == "acosh" || f == "atanh"
        || f == "arccsch" || f == "arcsech" || f == "arccoth"
        || f == "acsch" || f == "asech" || f == "acoth"
        
        || f == "exp" || f == "sqrt" || f == "cbrt"
        || f == "log" || f == "ln" || f == "lg";
}