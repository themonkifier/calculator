#include <iostream>
#include <algorithm>
#include <stack>
#include <deque>

#include <cstdlib>
#include <cmath>

#include "calculator.hpp"

int main(int argc, char* argv[])
{
    std::string input;

    std::cin >> input;
    
    std::cout << simplify_rpn(convert_to_rpn(tokenize(input))) << std::endl;

    return 0;
}

std::deque<std::string> tokenize(std::string input)
{
    std::deque<std::string> tokens;
    
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());

    for (int i = 0; i < input.size(); i++)
    {
        input.substr(it, it + 2);
    }

    return tokens;
}

std::deque<std::string> convert_to_rpn(std::deque<std::string> tokens)
{
    std::stack<std::string> operators;
    std::stack<double> nums;
    std::deque<std::string> output;
    std::string token;

    while (!tokens.empty())
    {
        token = tokens.front();
        tokens.pop_front();

        if (is_number(token)) output.push_back(token);
        else if (is_function(token)) operators.push(token);
        else if (is_operator(token))
        {
            std::string o1 = token;
            std::string o2;
            while (!operators.empty() && (o2 = operators.top()) != "(" && (precedence(o2, o1) == 1 || (precedence(o1, o2) == 0 && (is_operator(token) && o1 != "^"))))
            {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(o1);
        }
        else if (token == ",")
        {
            while (operators.top() != "(")
            {
                output.push_back(operators.top());
                operators.pop();
            }
        }
        else if (token == "(") operators.push(token);
        else if (token == ")")
        {
            while (operators.top() != "(")
            {
                if (operators.empty())
                {
                    /* if the stack runs out without finding a left parenthesis, then there are mismatched parentheses */
                }
                output.push_back(operators.top());
                operators.pop();
            }
            if (operators.top() != "(")
            {
                ;
            }
            operators.pop();
            if (!operators.empty() && !is_operator(operators.top()))
            {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty() && is_operator(operators.top()))
            {
                output.push_back(operators.top());
                operators.pop();
            }
        }
    }
    
    /* after the first while loop, pop the remaining items from the operator stack into the output queue */
    while (!operators.empty())
    {
        if (operators.top() != "(")
        {
            /* if the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses */
        }
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

bool is_operator(std::string op)
{
    return op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "^";
}

bool is_number(std::string num)
{
    char *p;
    strtod(num.c_str(), &p);

    return !*p;
}

bool is_function(std::string op)
{
    return op == "sin" || op == "cos" || op == "tan"
        || op == "arcsin" || op == "arccos" || op == "arctan"
        || op == "csc" || op == "sec" || op == "cot"
        || op == "arccsc" || op == "arcsec" || op == "arccot"
        
        || op == "sinh" || op == "cosh" || op == "tanh"
        || op == "arcsinh" || op == "arccosh" || op == "arctanh"
        || op == "csch" || op == "sech" || op == "coth"
        || op == "arccsch" || op == "arcsech" || op == "arccoth"
        
        || op == "exp" || op == "sqrt" || op == "cbrt";
}

int precedence(std::string o1, std::string o2)
{
    if (o1 == "+" || o1 == "-")
    {
        if (o2 == "+" || o2 == "-") return 0;
        return 1;
    }
    else if (o1 == "*" || o1 == "/" || o1 == "%")
    {
        if (o2 == "+" || o2 == "-") return -1;
        else if (o2 == "*" || o2 == "/" || o2 == "%") return 0;
        return 1;
    }
    else
    {
        if (o2 == "^") return 0;
        return 1;
    }
}

double simplify_rpn(std::deque<std::string> tokens)
{
    std::stack<double> nums;
    std::string token;
    while (tokens.size() > 1)
    {
        token = tokens.front();
        tokens.pop_front();
        if (is_number(token)) nums.push(std::stod(token));
        else if (is_operator)
        {
            double num1 = std::stod(tokens.front());
            tokens.pop_front();
            double num2 = std::stod(tokens.front());
            tokens.pop_front();

            nums.push(evaluate(num1, token[0], num2));
        }
        else
        {
            double num = std::stod(tokens.front());
            tokens.pop_front();

            nums.push(evaluate(token, num));
        }
    }
}

int find_operator(std::deque<std::string> tokens)
{
    int i = 0;
    for (std::deque<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if (is_operator(*it)) return i;
        i++;
    }
    return -1;
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
}

double evaluate(std::string f, double num)
{
    if (f == "sin") return sin(num);
    else if (f == "cos") return cos(num);
    else if (f == "tan") return tan(num);
    else if (f == "arcsin" || f == "asin") return asin(num);
    else if (f == "arccos" || f == "acos") return acos(num);
    else if (f == "arctan" || f == "atan") return atan(num);
    
    else if (f == "csc") return 1/sin(num);
    else if (f == "sec") return 1/cos(num);
    else if (f == "cot") return 1/tan(num);
    else if (f == "arccsc" || f == "acsc") return asin(1/num);
    else if (f == "arcsec" || f == "asec") return acos(1/num);
    else if (f == "arccot" || f == "acot") return atan(1/num);
    
    else if (f == "sinh") return sinh(num);
    else if (f == "cosh") return cosh(num);
    else if (f == "tanh") return tanh(num);
    else if (f == "arcsinh" || f == "asinh") return asinh(num);
    else if (f == "arccosh" || f == "acosh") return acosh(num);
    else if (f == "arctanh" || f == "atanh") return atanh(num);
    
    else if (f == "csch") return 1/sinh(num);
    else if (f == "sech") return 1/cosh(num);
    else if (f == "coth") return 1/tanh(num);
    else if (f == "arccsch" || f == "acsch") return asinh(1/num);
    else if (f == "arcsech" || f == "asech") return acosh(1/num);
    else if (f == "arccoth" || f == "acoth") return atanh(1/num);
    
    else if (f == "exp") return exp(num);
    else if (f == "sqrt") return sqrt(num);
    else if (f == "cbrt") return cbrt(num);
    return (-1)/0;
}