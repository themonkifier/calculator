#include "../include/calculator.hpp"

std::deque<Token> convert_to_rpn(std::deque<Token> tokens)
{
    std::deque<std::string> operators;
    std::deque<Token> output;
    Token token;

    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop_front();

        if (token.index() == 0) output.push_back(token);
        else if (is_function(std::get<1>(token))) operators.push_back(std::get<1>(token));
        else if (is_operator(std::get<1>(token)))
        {
            std::string o2;
            while (!operators.empty() && (o2 = operators.back()) != "(" && (precedence(std::get<1>(token)) < precedence(o2)
                    || (precedence(std::get<1>(token)) == precedence(o2) && o2 != "^")))
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
            operators.push_back(std::get<1>(token));
        }
        else if (std::get<1>(token) == ",")
        {
            while (operators.back() != "(")
            {
                output.push_back(operators.back());
                operators.pop_back();
            }
        }
        else if (std::get<1>(token) == "(") operators.push_back(std::get<1>(token));
        else if (std::get<1>(token) == ")")
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

int precedence(std::string op)
{
    if (op == "^") return 2;
    else if (op == "*" || op == "/" || op == "%") return 1;
    else if (op == "+" || op == "-") return 0;
    return -1;
}

Number simplify_rpn(std::deque<Token> tokens)
{
    std::deque<Number> nums;
    Token token;

    while (tokens.size() > 0)
    {
        token = tokens.front();
        tokens.pop_front();
        if (token.index() == 0)
        {
            nums.push_back(std::get<0>(token));
        }
        else if (is_operator(std::get<1>(token)))
        {
            Number num1 = nums.back();
            nums.pop_back();
            Number num2 = nums.back();
            nums.pop_back();

            nums.push_back(evaluate(num2, std::get<1>(token)[0], num1));
        }
        else
        {
            Number num = nums.back();
            nums.pop_back();

            std::string str = std::get<1>(token);

            nums.push_back(evaluate(str, num));
        }
    }
    return nums.back();
}

Number evaluate(Number num1, char op, Number num2)
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
        return num1 % num2;
        case '^':
        return num1 ^ num2;
    }
    std::cout << "error" << std::endl;
    return Number(-INFINITY);
}

Number evaluate(std::string f, Number _num)
{
    if (_num.unit.symbol != "" && _num.unit.symbol != "deg") throw std::runtime_error("functions can only be called on numbers which are unitless or carry units of angle");

    num_t num = _num.val, ret = -INFINITY;

    int sign = 1;
    if (f.size() > 0 && f[0] == '-')
    {
        sign = -1;
        f = f.substr(1);
    }

    if (f == "sin") ret = sign * sinl(num);
    else if (f == "cos") ret = sign * cosl(num);
    else if (f == "tan") ret = sign * tanl(num);
    else if (f == "arcsin" || f == "asin") ret = sign * asinl(num);
    else if (f == "arccos" || f == "acos") ret = sign * acosl(num);
    else if (f == "arctan" || f == "atan") ret = sign * atanl(num);
    
    else if (f == "csc") ret = sign * 1/sinl(num);
    else if (f == "sec") ret = sign * 1/cosl(num);
    else if (f == "cot") ret = sign * 1/tanl(num);
    else if (f == "arccsc" || f == "acsc") ret = sign * asinl(1/num);
    else if (f == "arcsec" || f == "asec") ret = sign * acosl(1/num);
    else if (f == "arccot" || f == "acot") ret = sign * atanl(1/num);
    
    else if (f == "sinh") ret = sign * sinhl(num);
    else if (f == "cosh") ret = sign * coshl(num);
    else if (f == "tanh") ret = sign * tanhl(num);
    else if (f == "arcsinh" || f == "asinh") ret = sign * asinhl(num);
    else if (f == "arccosh" || f == "acosh") ret = sign * acoshl(num);
    else if (f == "arctanh" || f == "atanh") ret = sign * atanhl(num);
    
    else if (f == "csch") ret = sign * 1/sinhl(num);
    else if (f == "sech") ret = sign * 1/coshl(num);
    else if (f == "coth") ret = sign * 1/tanhl(num);
    else if (f == "arccsch" || f == "acsch") ret = sign * asinhl(1/num);
    else if (f == "arcsech" || f == "asech") ret = sign * acoshl(1/num);
    else if (f == "arccoth" || f == "acoth") ret = sign * atanhl(1/num);
    
    else if (f == "exp") ret = sign * expl(num);
    else if (f == "sqrt") ret = sign * sqrtl(num);
    else if (f == "cbrt") ret = sign * cbrtl(num);
    else if (f == "log") ret = sign * log10l(num);
    else if (f == "ln") ret = sign * logl(num);
    else if (f == "lg") ret = sign * log2l(num);
    return Number(sign * ret);
}