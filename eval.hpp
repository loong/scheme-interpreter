/**
 * \file eval.hpp
 *
 * Encapsulates the interface for the expression evaluator function.
 */

#ifndef EVAL_HPP
#define EVAL_HPP

#include "cons.hpp"

using namespace std;

/**
 * \brief Evaluate the expression tree whose root is pointed to by c
 * (error if c does not hold a well-formed expression).
 *
 * \return The value resulting from evaluating the expression.
 */
Cell* eval(Cell* const c);

/**
 * \brief Eval function just for the first element - or in other words for 
 *        cells which can call functions
 */
Cell* eval_function(Cell* const c);

#endif // EVAL_HPP
