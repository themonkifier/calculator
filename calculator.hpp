#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

enum CurrentType
{
    Number,
    Operator,
    Parenthesis,
    Function
};

/**
 * @brief Splits the input string into tokens
 * 
 */
std::deque<std::string> tokenize(std::string);

enum CurrentType current_type(std::string, int);

/**
 * @brief Converts the input token deque into Reverse Polish Notation
 * 
 */
std::deque<std::string> convert_to_rpn(std::deque<std::string>);

/**
 * @brief Returns whether or not the input string is an operator
 *  ('+', '-', '*', '/', '%', '^')
 * @return `true` If the string is an operator,
 * @return `false` otherwise
 */
bool is_operator(std::string);

/**
 * @brief Returns whether or not the input string is a number
 * @return `true` If the string is a number,
 * @return `false` otherwise
 */
bool is_number(std::string);

/**
 * @brief Returns whether or not the input string is a function
 * @return `true` If the string is a function,
 * @return `false` otherwise
 */
bool is_function(std::string);

/**
 * @brief Returns whether one operator has precedence over the other
 * @return `-1` if the first operator has precedence,
 * @return `0` if the operators have equal precedence,
 * @return `1` if the second operator has precedence
*/
int precedence(std::string, std::string);

/**
 * @brief Simplifies a Reverse Polish Notation deque
*/
double simplify_rpn(std::deque<std::string>);

/**
 * @brief Returns the address of the first operator if the input deque has
 *  an operator, or -1 if there is none
 * @return The `index` of the first operator if the deque has an operator,
 * @return `-1` otherwise
*/
int find_operator(std::deque<std::string>);

/**
 * @brief Evaluates the given expression
*/
double evaluate(double, char, double);

/**
 * @brief Evaluates the given expression
*/
double evaluate(std::string, double);

#endif