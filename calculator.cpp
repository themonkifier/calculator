#include <iostream>
#include <algorithm>
#include <stack>
#include <deque>

#include "calculator.hpp"

int main()//int argc, char* argv[])
{
    std::string input;

    std::cin >> input;

    clean_input(input);
    evaluate(input);
    
    std::cout << input << std::endl;

    return 0;
}

bool clean_input(std::string &input)
{
    /*for (std::string::iterator it = input.begin(); it != input.end(); it++)
    {
        if (*it == ',')
    }*/
    return true;
}

double evaluate(std::deque<std::string> &tokens)
{
    std::stack<char> operations;
    std::stack<double> nums;

    while (there are tokens to be read):
    {
        read a token
        if the token is:
        - a number:
            put it into the output queue
        - a function:
            push it onto the operator stack 
        - an operator o1:
            while (there is an operator o2 at the top of the operator stack which is not a left parenthesis, && (o2 has greater precedence than o1 || (o1 && o2 have the same precedence && o1 is left-associative)))
                pop o2 from the operator stack into the output queue
            push o1 onto the operator stack
        - a ",":
            while the operator at the top of the operator stack is not a left parenthesis:
                pop the operator from the operator stack into the output queue
        - a left parenthesis (i.e. "("):
            push it onto the operator stack
        - a right parenthesis (i.e. ")"):
            while (the operator at the top of the operator stack is not a left parenthesis)
                {assert the operator stack is not empty}
                /* If the stack runs out without finding a left parenthesis, then there are mismatched parentheses. */
                pop the operator from the operator stack into the output queue
            {assert there is a left parenthesis at the top of the operator stack}
            pop the left parenthesis from the operator stack && discard it
            if there is a function token at the top of the operator stack, then:
                pop the function from the operator stack into the output queue
    }
/* After the while loop, pop the remaining items from the operator stack into the output queue. */
while there are tokens on the operator stack:
    /* If the operator token on the top of the stack is a parenthesis, then there are mismatched parentheses. */
    {assert the operator on top of the stack is not a (left) parenthesis}
    pop the operator from the operator stack onto the output queue
}