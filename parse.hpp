/**
 * \file parse.hpp
 *
 * Encapsulates the interface for the expression parsing function,
 * which analyzes a string containing an  s-expression, and determines
 * its tree structure.
 */

#ifndef PARSE_HPP
#define PARSE_HPP

#include "cons.hpp"

using namespace std;

/**
 * \brief Recursively parse sexpr and build the parse tree.  \param
 * sexpr The s-expression stored in a string variable (note that this
 * version of parse has side effects: it may alter the contents of
 * sexpr).
 *
 * \return A pointer to the conspair cell at the root of the parse tree.
 */
Cell* parse(string sexpr);

/**
 * \brief Check whether the character is whitespace.
 * \return True if it is character, false else.
 * \param ch The character to check.
 */
bool iswhitespace(char ch);

#endif // PARSE_HPP
