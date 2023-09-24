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

    for (auto it = input.begin(); it != input.end(); it++) std::cout << *it;
    std::cout << std::endl;

    /* split into individual tokens */
    int i = 0, previous = 0;
    do
    {
        type = current_type(input, i);

        if (!(type == Operator || type == Parenthesis)) while (i < input.length() - 1 && type == current_type(input, i + 1)) i++;
        tokens.push_back(input.substr(previous, i - previous + 1));

        previous = ++i;
    } while (i < input.size());
    
    for (auto it = tokens.begin(); it != tokens.end(); it++) std::cout << *it << " ";
    std::cout << std::endl;

    /* deals with negative signs ("-"s that are and before numbers after operators/open parentheses/the beginning of the string)
       and inserts "*"s where necessary */
    auto it = tokens.begin();
    if (*it == "-")
    {
        it = tokens.erase(it);
        *it = "-" + *it;
    }
    for (; it != tokens.end();)
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

    for (auto it = tokens.begin(); it != tokens.end(); it++) std::cout << *it << " ";
    std::cout << std::endl;

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

    for (auto it = output.begin(); it != output.end(); it++) std::cout << *it << " ";
    std::cout << std::endl;

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

double simplify_rpn(std::deque<std::string> tokens)
{
    std::deque<double> nums;
    std::string token;

    while (tokens.size() > 0)
    {
        token = tokens.front();
        tokens.pop_front();
        if (is_number(token))
        {
            // double num = std::stod(token);
            // std::complex<double> num;
            // token >> num;
            nums.push_back(std::stod(token));
        }
        else if (is_operator(token))
        {
            double num1 = nums.back();
            nums.pop_back();
            double num2 = nums.back();
            nums.pop_back();

            nums.push_back(evaluate(num2, token[0], num1));
        }
        else
        {
            double num = nums.back();
            nums.pop_back();

            nums.push_back(evaluate(token, num));
        }
    }
    return nums.back();
}

double evaluate(double num1, char op, double num2)
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

double evaluate(std::string f, double num)
{
    int sign = 1;
    if (f.size() > 0 && f[0] == '-')
    {
        sign = -1;
        f = f.substr(1);
    }

    if (f == "sin") return sign * sin(num);
    else if (f == "cos") return sign * cos(num);
    else if (f == "tan") return sign * tan(num);
    else if (f == "arcsin" || f == "asin") return sign * asin(num);
    else if (f == "arccos" || f == "acos") return sign * acos(num);
    else if (f == "arctan" || f == "atan") return sign * atan(num);
    
    else if (f == "csc") return sign * 1/sin(num);
    else if (f == "sec") return sign * 1/cos(num);
    else if (f == "cot") return sign * 1/tan(num);
    else if (f == "arccsc" || f == "acsc") return sign * asin(1/num);
    else if (f == "arcsec" || f == "asec") return sign * acos(1/num);
    else if (f == "arccot" || f == "acot") return sign * atan(1/num);
    
    else if (f == "sinh") return sign * sinh(num);
    else if (f == "cosh") return sign * cosh(num);
    else if (f == "tanh") return sign * tanh(num);
    else if (f == "arcsinh" || f == "asinh") return sign * asinh(num);
    else if (f == "arccosh" || f == "acosh") return sign * acosh(num);
    else if (f == "arctanh" || f == "atanh") return sign * atanh(num);
    
    else if (f == "csch") return sign * 1/sinh(num);
    else if (f == "sech") return sign * 1/cosh(num);
    else if (f == "coth") return sign * 1/tanh(num);
    else if (f == "arccsch" || f == "acsch") return sign * asinh(1/num);
    else if (f == "arcsech" || f == "asech") return sign * acosh(1/num);
    else if (f == "arccoth" || f == "acoth") return sign * atanh(1/num);
    
    else if (f == "exp") return sign * exp(num);
    else if (f == "sqrt") return sign * sqrt(num);
    else if (f == "cbrt") return sign * cbrt(num);
    else if (f == "log") return sign * log10(num);
    else if (f == "ln") return sign * log(num);
    else if (f == "lg") return sign * log2(num);
    return -INFINITY;
}