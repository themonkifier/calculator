#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <deque>
#include <iostream> // ?

#include "units.hpp"

/**
 * @brief The current type of a character in the input string
 */
enum class CurrentType
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
enum class PrintFormat
{
    Default,
    Commas,
    Spaces
};

/**
 * @brief Gets the type of index `i` in string `input` (as a `CurrentType`)
 * @param input The input string, to be checked
 * @param i The index to check
 */
enum CurrentType current_type(std::string input, int i);

/**
 * @brief Splits the input string into tokens
 * @param input The input string
 * @return the input string, split into individual tokens
 * 
 */
std::deque<Token> tokenize(std::string input, PrintFormat &pf);

/**
 * @brief Returns whether or not the input string is an operator
 *  ('+', '-', '*', '/', '%', '^')
 * @param op The string to check
 * @return `true` If the string is an operator,
 * @return `false` otherwise
 */
bool is_operator(std::string op);

/**
 * @brief Returns whether or not the input string is a number
 * @param num The string to check
 * @return `true` If the string is a number,
 * @return `false` otherwise
 */
bool is_number(std::string num);

/**
 * @brief Returns whether or not the input string is a function
 * @param f the string to check
 * @return `true` If the string is a function,
 * @return `false` otherwise
 */
bool is_function(std::string f);

#endif