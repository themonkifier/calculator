#ifndef CALCULATOR_H
#define CALCULATOR_H

//#include <complex>
#include <iostream>

#include <cstdlib>

#include "tokenizer.hpp"

/**
 * @brief Converts the input token deque into Reverse Polish Notation
 * @param tokens The tokens to be converted to RPN
 * @return The tokens, ordered in RPN
 */
std::deque<Token> convert_to_rpn(std::deque<Token> tokens);

/**
 * @brief Returns the relative precedence of the given operator
 * @param op The operator to check
 * @return `2` if operator is "^",
 * @return `1` if operator is "*", "/", "%",
 * @return `0` if operator is "+", "-",
 * @return `-1` otherwise
 */
int precedence(std::string op);

/**
 * @brief Simplifies a Reverse Polish Notation deque
 * @param tokens The tokens, in RPN, to evaluate
 * @return The final result of evaluating the equation
 */
Number simplify_rpn(std::deque<Token> tokens);

/**
 * @brief Evaluates the given expression (ex. `1` `+` `2`)
 * @param num1 The first operand
 * @param op The operator
 * @param num2 The second operand
 * @return The value of (`num1`) (`op`) (`num2`)
 */
Number evaluate(Number num1, char op, Number num2);

/**
 * @brief Evaluates the given expression (ex. `sqrt`(`9`))
 * @param f The function
 * @param num The input to the function
 */
Number evaluate(std::string f, Number num);

#endif