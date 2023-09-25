#include "calculator.hpp"

int main(int argc, char* argv[])
{
    std::string input;

    std::getline(std::cin, input);

    std::cout << simplify_rpn(convert_to_rpn(tokenize(input))) << std::endl;

    return 0;
}



std::deque<std::string> tokenize(std::string input)
{
    std::deque<std::string> tokens;
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    input.erase(std::remove(input.begin(), input.end(), ','), input.end());
    
    enum CurrentType type;
    type = (CurrentType) None;
    std::string token;
    
    /* replace "e"s and "pi"s with their numerical values */
    for (auto it = input.begin(); it != input.end(); it++)
    {
        if (*it == 'e')
        {
            it = input.erase(it);
            std::string e = std::to_string(M_E);
            for (auto str_it = e.rbegin(); str_it != e.rend(); str_it++) it = input.insert(it, *str_it);
            it = input.insert(it, '\''); /* marker to show that a constant was inserted */
        }
        else if (it != input.end() - 1 && *it == 'p' && *(it + 1) == 'i')
        {
            it = input.erase(it);
            it = input.erase(it);
            std::string e = std::to_string(M_PI);
            for (auto str_it = e.rbegin(); str_it != e.rend(); str_it++) it = input.insert(it, *str_it);
            it = input.insert(it, '\'');
        }
    }

    /* split into individual tokens */
    int i = 0, previous = 0;
    do
    {
        type = current_type(input, i);

        if (!(type == Operator || type == Parenthesis)) while (i < input.length() - 1 && type == current_type(input, i + 1)) i++;
        tokens.push_back(input.substr(previous, i - previous + 1));

        previous = ++i;
    } while (i < input.size());

    /* deals with negative signs ("-"s that are before numbers and after operators/open parentheses/the beginning of the string)
       and inserts "*"s where necessary */
    auto it = tokens.begin();
    if (it != tokens.end() - 1 && *it == "-")
    {
        it = tokens.erase(it);
        *it = "-" + *it;
    }
    while (it != tokens.end() - 1)
    {
        if (*it == "-" && (is_operator(*(it - 1)) || *(it - 1) == "(") && (is_number(*(it + 1)) || is_function(*(it + 1))))
        {
            it = tokens.erase(it);
            *it = "-" + *it;
        }
        else if (is_number(*it) && (*(it + 1) == "(" || is_function(*(it + 1))))
        {
            it++;
            it = tokens.insert(it, "*");
        }
        else it++;
    }

    return tokens;
}

enum CurrentType current_type(std::string str, int i)
{
    if (is_number(str.substr(i, 1))) return Number;
    else if (is_operator(str.substr(i, 1))) return Operator;
    else if (str[i] == '(' || str[i] == ')') return Parenthesis;
    else if (str[i] == '\'') return None;
    return Function;
}

std::deque<std::string> convert_to_rpn(std::deque<std::string> tokens)
{
    std::deque<std::string> operators;
    std::deque<std::string> output;
    std::string token;

    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop_front();

        if (is_number(token)) output.push_back(token);
        else if (is_function(token)) operators.push_back(token);
        else if (is_operator(token))
        {
            std::string o2;
            while (!operators.empty() && (o2 = operators.back()) != "(" && (precedence(token) < precedence(o2)
                    || (precedence(token) == precedence(o2) && o2 != "^")))
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
            operators.push_back(token);
        }
        else if (token == ",")
        {
            while (operators.back() != "(")
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
        }
        else if (token == "(") operators.push_back(token);
        else if (token == ")")
        {
            while (!operators.empty() && operators.back() != "(")
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
            operators.pop_back();
            if (!operators.empty() && is_function(operators.back()))
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
        }
    }
    
    /* after the first while loop, pop the remaining items from the operator stack into the output queue */
    while (!operators.empty())
    {
        if (operators.back() != "(")
        {
            /* if the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses */
        }
        output.push_back(operators.back());
        operators.pop_back();
    }

    return output;
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

int precedence(std::string op)
{
    if (op == "^") return 2;
    else if (op == "*" || op == "/" || op == "%") return 1;
    else if (op == "+" || op == "-") return 0;
    return -1;
}

num_t simplify_rpn(std::deque<std::string> tokens)
{
    std::deque<num_t> nums;
    std::string token;

    while (tokens.size() > 0)
    {
        token = tokens.front();
        tokens.pop_front();
        if (is_number(token))
        {
            nums.push_back(std::stod(token));
        }
        else if (is_operator(token))
        {
            num_t num1 = nums.back();
            nums.pop_back();
            num_t num2 = nums.back();
            nums.pop_back();

            nums.push_back(evaluate(num2, token[0], num1));
        }
        else
        {
            num_t num = nums.back();
            nums.pop_back();

            nums.push_back(evaluate(token, num));
        }
    }
    return nums.back();
}

num_t evaluate(num_t num1, char op, num_t num2)
{
    switch (op)
    {
        case '+':
        return num1 + num2;
        case '-':
        return num1 - num2;
        case '*':
        return num1 * num2;
        case '/':
        return num1 / num2;
        case '%':
        return fmod(num1, num2);
        case '^':
        return pow(num1, num2);
    }
    std::cout << "error" << std::endl;
    return -INFINITY;
}

num_t evaluate(std::string f, num_t num)
{
    int sign = 1;
    if (f.size() > 0 && f[0] == '-')
    {
        sign = -1;
        f = f.substr(1);
    }

    if (f == "sin") return sign * sinl(num);
    else if (f == "cos") return sign * cosl(num);
    else if (f == "tan") return sign * tanl(num);
    else if (f == "arcsin" || f == "asin") return sign * asinl(num);
    else if (f == "arccos" || f == "acos") return sign * acosl(num);
    else if (f == "arctan" || f == "atan") return sign * atanl(num);
    
    else if (f == "csc") return sign * 1/sinl(num);
    else if (f == "sec") return sign * 1/cosl(num);
    else if (f == "cot") return sign * 1/tanl(num);
    else if (f == "arccsc" || f == "acsc") return sign * asinl(1/num);
    else if (f == "arcsec" || f == "asec") return sign * acosl(1/num);
    else if (f == "arccot" || f == "acot") return sign * atanl(1/num);
    
    else if (f == "sinh") return sign * sinhl(num);
    else if (f == "cosh") return sign * coshl(num);
    else if (f == "tanh") return sign * tanhl(num);
    else if (f == "arcsinh" || f == "asinh") return sign * asinhl(num);
    else if (f == "arccosh" || f == "acosh") return sign * acoshl(num);
    else if (f == "arctanh" || f == "atanh") return sign * atanhl(num);
    
    else if (f == "csch") return sign * 1/sinhl(num);
    else if (f == "sech") return sign * 1/coshl(num);
    else if (f == "coth") return sign * 1/tanhl(num);
    else if (f == "arccsch" || f == "acsch") return sign * asinhl(1/num);
    else if (f == "arcsech" || f == "asech") return sign * acoshl(1/num);
    else if (f == "arccoth" || f == "acoth") return sign * atanhl(1/num);
    
    else if (f == "exp") return sign * expl(num);
    else if (f == "sqrt") return sign * sqrtl(num);
    else if (f == "cbrt") return sign * cbrtl(num);
    else if (f == "log") return sign * log10l(num);
    else if (f == "ln") return sign * logl(num);
    else if (f == "lg") return sign * log2l(num);
    return -INFINITY;
}