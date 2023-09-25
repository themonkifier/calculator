#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <iostream>
#include <algorithm>
#include <deque>
#include <complex>

#include <cstdlib>
#include <cmath>

#include "constants.hpp"

typedef long double num_t;

/**
 * @brief The current type of a character in the input string
*/
enum CurrentType
{
    None,
    Number,
    Operator,
    Parenthesis,
    Function
};

/**
 * @brief The format of numbers, for printing at the end
 */
enum PrintFormat
{
    Default,
    Commas,
    Spaces
};

/**
 * @brief Splits the input string into tokens
 * @param input The input string
 * @return the input string, split into individual tokens
 * 
 */
std::deque<std::string> tokenize(std::string);

/**
 * @brief Gets the type of index `i` in string `input` (as a `CurrentType`)
 * @param input The input string, to be checked
 * @param i The index to check
 */
enum CurrentType current_type(std::string, int);

/**
 * @brief Converts the input token deque into Reverse Polish Notation
 * @param tokens The tokens to be converted to RPN
 * @return The tokens, ordered in RPN
 */
std::deque<std::string> convert_to_rpn(std::deque<std::string>);

/**
 * @brief Returns whether or not the input string is an operator
 *  ('+', '-', '*', '/', '%', '^')
 * @param op The string to check
 * @return `true` If the string is an operator,
 * @return `false` otherwise
 */
bool is_operator(std::string);

/**
 * @brief Returns whether or not the input string is a number
 * @param num The string to check
 * @return `true` If the string is a number,
 * @return `false` otherwise
 */
bool is_number(std::string);

/**
 * @brief Returns whether or not the input string is a function
 * @param f the string to check
 * @return `true` If the string is a function,
 * @return `false` otherwise
 */
bool is_function(std::string);

/**
 * @brief Returns the relative precedence of the given operator
 * @param op The operator to check
 * @return `2` if operator is "^",
 * @return `1` if operator is "*", "/", "%",
 * @return `0` if operator is "+", "-",
 * @return `-1` otherwise
 */
int precedence(std::string);

/**
 * @brief Simplifies a Reverse Polish Notation deque
 * @param tokens The tokens, in RPN, to evaluate
 * @return The final result of evaluating the equation
 */
num_t simplify_rpn(std::deque<std::string>);

/**
 * @brief Evaluates the given expression (ex. `1` `+` `2`)
 * @param num1 The first operand
 * @param op The operator
 * @param num2 The second operand
 * @return The value of (`num1`) (`op`) (`num2`)
 */
num_t evaluate(num_t, char, num_t);

/**
 * @brief Evaluates the given expression (ex. `sqrt`(`9`))
 * @param f The function
 * @param num The input to the function
 */
num_t evaluate(std::string, num_t);

#endif