/**
 * \file eval.cpp
 *
 * Evaluates the s-expression. Recursive approach. Each death level returns
 * its result as a cell to the upper level.
 */

#include "eval.hpp"
#include "DefinitionManager.hpp"

#include <stdexcept>

Cell* eval(Cell* const c) {

  /// just returns Cell if deepest level reached
  /// this approach gets rid of loads of if-then checks
  if (!listp(c)) {
    if (symbolp(c)) {
      	return c->get_definition();
    }
    return c;
  }

  Cell* func = eval_function(car(c));
  Cell* args = cdr(c);

  
#ifdef L_DEBUG
  cout << "func: " << *func << endl;
  cout << "  args: " << *args << endl;
#endif


  /// The right operation/ function will be called through overwriting
  return func->apply(args);

}

Cell* eval_function(Cell* const c) {
  if (listp(c)) {
    /// in case for nested cases such as ((quote if) 1 0)
    return eval(c);
  }

  if (symbolp(c)) {
    string sym = get_symbol(c);

    /// checks wether it is a user-defined function or a built in function
    if (DefinitionManager::Instance()->is_definition(sym)) {
      if (c->get_definition()->is_lambda()) {
	return c->get_definition();
      }
    }
    
    if (FunctionCell::is_function(sym)) {
      return new FunctionCell(sym.c_str());
    }
    
    if (ArithmeticCell::is_arithmetic(sym)) {
      return new ArithmeticCell(sym.c_str());
    }
  }

  throw runtime_error("No operator/ function in front");   
}
