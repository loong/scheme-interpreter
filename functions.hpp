#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <stdexcept>

#include "Cell.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
/// Helpers

/**
 * \brief Since most of the functions take only one argument, a small helper for
 *        better reuse of code is provided
 */
Cell* single_argument_eval(const SymbolCell* func, Cell* args) throw (runtime_error);

////////////////////////////////////////////////////////////////////////////////
/// Actual implementations of *_func functions

/**
 * \brief Implements the ceiling function, using cmath for convenience
 * \param args expects a single ConsCell with DoubleCell as car
 */
Cell* ceiling_func(const FunctionCell* func, Cell* args);

/**
 * \brief Implements the floor function, using cmath for convenience
 * \param Cell*args expects a single ConsCell with DoubleCell as car
 */
Cell* floor_func(const FunctionCell* func, Cell* args);

/**
 * \brief Prevents eval to evaluate the s-expression.
 */
Cell* quote_func(const FunctionCell* func, Cell* args) throw (runtime_error);

/**
 * \brief Creates a ConsPair with car and cdr similar to those of LISP
 * \param args expects a cons list of size 2 containing the car and then cdr in
 *        that order.
 */
Cell* cons_func(const FunctionCell* func, Cell* args) throw (runtime_error);

/**
 * \brief Exposes car from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 */
Cell* car_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes cdr from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 */
Cell* cdr_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes intp from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 * \return an IntCell containing 0 or 1 indicating true or false
 */
Cell* intp_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes doublep from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 * \return an IntCell containing 0 or 1 indicating true or false
 */
Cell* doublep_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes symbolp from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 * \return an IntCell containing 0 or 1 indicating true or false
 */
Cell* symbolp_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes nullp from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 * \return an IntCell containing 0 or 1 indicating true or false
 */
Cell* nullp_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposes listp from cons.hpp, also evaluates sub-trees
 * \param Cell* expects a single ConsCell
 * \return an IntCell containing 0 or 1 indicating true or false
 */
Cell* listp_func(const FunctionCell* func, Cell* args);

/**
 * \brief If-then branching in form of a callable function. Evaluates only relevant 
          sub-trees. <b>Special Cow Feature!</b>
 * \param Cell* expects a ConsCell with two or three arguments
 */
Cell* if_func(const FunctionCell* func, Cell* args);

/**
 * \brief Defines a variable by putting a new definition into the
 *        definitions map table
 */
Cell* define_func(const FunctionCell* func, Cell* args);

/**
 * \brief Compares a variable number of numbers if they are smaller
 * than the number on their right side
 */
Cell* less_than_func(const FunctionCell* func, Cell* args);

/**
 * \brief Negates argument returning boolean in c style (0 and 1)
 */
Cell* not_func(const FunctionCell* func, Cell* args);

/**
 * \brief Printing out evaluated argument
 * \return nil Always returns nil
 */
Cell* print_func(const FunctionCell* func, Cell* args);

/**
 * \brief Exposed eval from cons.hpp
 */
Cell* eval_func(const FunctionCell* func, Cell* args);

/**
 * \brief Creates procedure cell, after doing security checks
 */
Cell* lambda_func(const FunctionCell* func, Cell* args);

/**
 * \brief Calls apply on a user defined procedure, created with lambda
 */
Cell* apply_func(const FunctionCell* func, Cell* args);

/**
 * \brief Syntactic sugar to add local variables more similar to c
 */
Cell* let_func(const FunctionCell* func, Cell* args);

/**
 * \brief used for random variables, takes the range of random values as a range
 */
Cell* rand_func(const FunctionCell* func, Cell* args);

/**
 * \brief converts any Cell type to symbol type using print method
 */
Cell* str_func(const FunctionCell* func, Cell* args);

/**
 * \brief substring for symbol cells, takes symbol and a range (start
 *        and end of substr)
 */
Cell* substr_func(const FunctionCell* func, Cell* args);

/**
 * \brief appends symbols, takes two symbols
 */
Cell* appstr_func(const FunctionCell* func, Cell* args);

/**
 * \brief parses s-expression inside a symbol cell and evaluates it
 */
Cell* parse_func(const FunctionCell* func, Cell* args);

/**
 * \brief parses s-expression inside a symbol cell and evaluates it
 */
Cell* parse_eval_func(const FunctionCell* func, Cell* args);

#endif
