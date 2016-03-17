/**
 * \file functions.cpp
 * 
 * Implementation of function.hpp
 *
 */

#include "functions.hpp"
#include "cons.hpp"
#include "eval.hpp"

#include "DefinitionManager.hpp"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////
/// Helpers

Cell* single_argument_eval(const SymbolCell* func, Cell* args) throw (runtime_error) {
  if (ConsCell::get_list_size(args) != 1) {  
    string msg = "NoOfArguments: "
      + string(func->get_symbol())      // provides function name for
      + " accepts exactly 1 argument";  // backtracking bugs
    
    throw runtime_error(msg);
  }

  return eval(car(args));

}

Cell* bool_2_cell(bool b) {
  if(b) {
    return make_int(1);
  }
  return make_int(0);
}

////////////////////////////////////////////////////////////////////////////////
/// Actual implementations of *_func functions
////////////////////////////////////////////////////////////////////////////////

Cell* ceiling_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);
  int res = (int) ceil(get_double(argument_cell));
  return make_int(res);
}

Cell* floor_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);
  int res = (int) floor(get_double(argument_cell));
  
  return make_int(res);
}

////////////////////////////////////////////////////////////////////////////////

Cell* cons_func(const FunctionCell* func, Cell* args) throw (runtime_error) {
  if (ConsCell::get_list_size(args) != 2) {
    throw runtime_error("NoOfArguments: cons only accepts exactly 2 arguments");
  }
  
  // For readability create tmp vars
  Cell* my_car = eval(car(args));
  Cell* my_cdr = eval(car(cdr(args)));

  /*if (!listp(my_cdr)) {
    delete my_car;
    delete my_cdr;

    throw runtime_error("IncorrectList: Are you trying to use dotted pairs?");
    }*/

  return cons(my_car, my_cdr);
}

////////////////////////////////////////////////////////////////////////////////

Cell* car_func(const FunctionCell* func, Cell* args) {
 Cell* argument_cell = single_argument_eval(func, args);
 return car(argument_cell);
}

Cell* cdr_func(const FunctionCell* func, Cell* args) {
 Cell* argument_cell = single_argument_eval(func, args);
 return cdr(argument_cell);
}

////////////////////////////////////////////////////////////////////////////////

Cell* quote_func(const FunctionCell* func, Cell* args) throw (runtime_error) {
  if (ConsCell::get_list_size(args) != 1) {                  
    throw runtime_error("NoOfArgumentError: quote only takes 1 argument");  
  }
  
  return car(args);
}

////////////////////////////////////////////////////////////////////////////////

/// \todo try to turn this into templates!

Cell* intp_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);

  return bool_2_cell(intp(argument_cell));
}

Cell* doublep_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);

  return bool_2_cell(doublep(argument_cell));
}

Cell* symbolp_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);

  return bool_2_cell(symbolp(argument_cell));
}

Cell* nullp_func(const FunctionCell* func, Cell* args) {
 Cell* argument_cell = single_argument_eval(func, args);

 return bool_2_cell(nullp(argument_cell));
}

Cell* listp_func(const FunctionCell* func, Cell* args) {
 Cell* argument_cell = single_argument_eval(func, args);

 return bool_2_cell(listp(argument_cell));
}

////////////////////////////////////////////////////////////////////////////////

Cell* if_func(const FunctionCell* func, Cell* args) {
  int num_args = ConsCell::get_list_size(args);
  
  if (num_args < 2 || num_args > 3) {
    throw runtime_error("NoOfArguments: if requires at least 2 and not more than 3 arguments"); 
  }

  Cell* curr_cell = eval(car(args));
    
  // if, then
  if (symbolp(curr_cell) || ( intp(curr_cell) && get_int(curr_cell) )
     || ( doublep(curr_cell) && get_double(curr_cell) ) )
    return eval( car(cdr(args)) );
    
  // else, no then
  if (cdr(cdr(args)) == nil) {  
    return nil; // unspecified
  }

  // else
  return eval( car(cdr(cdr(args))) );
} 

////////////////////////////////////////////////////////////////////////////////

Cell* define_func(const FunctionCell* func, Cell* args) {
  if (ConsCell::get_list_size(args) != 2) {
    throw runtime_error("NoOfArguments: define only accepts exactly 2 arguments");
  }

  Cell* symbol = car(args);  
  Cell* def = eval(car(cdr(args)));

  SymbolCell::add_definition(symbol->get_symbol(), def);

  return nil;                         /// always return nil according to specs
}

////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Weird, but is equivalent to a "not equal"
 *
 * \todo verfiy!
 */
bool string_compare(Cell* const args) {
  Cell* pos = args;
  
  string s1;
  string s2;

  bool is_true = true;

  while (!nullp(cdr(pos))) {
    s1 = eval(car(pos))->get_symbol();
    s2 = eval(car(cdr(pos)))->get_symbol();
    
    if (s1.compare(s2) == 0) {
      /// can return false here, but need to check the syntax of following args
      is_true = false;       
    }
    pos = cdr(pos);
  }

  return is_true;
}

bool numeral_compare(Cell* const args) {
  Cell* pos = args;
  
  double num1;
  double num2;

  bool is_true = true;

  while (!nullp(cdr(pos))) {
    num1 = eval(car(pos))->get_numeral();
    num2 = eval(car(cdr(pos)))->get_numeral();
    
    if (! (num1 < num2) ) {
      /// can return false here, but need to check the syntax of following args
      is_true = false;
    }
    pos = cdr(pos);
  }

  return is_true;
}

Cell* less_than_func(const FunctionCell* func, Cell* args) {
  int num_args = ConsCell::get_list_size(args);
  if (num_args < 2) {
    return make_int(1);
  }
  
  bool is_true;
  
  if (symbolp(eval(car(args)))) {
    is_true = string_compare(args);
  }
  else {
    is_true = numeral_compare(args); // automatically throws error if
				     // type is wrong
  }

  return bool_2_cell(is_true);
}

////////////////////////////////////////////////////////////////////////////////

Cell* not_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);
  
  if (argument_cell->is_int() || argument_cell->is_double()) {
    if (argument_cell->get_numeral() == 0) {
      return make_int(1);
    } 
  }
  return make_int(0);
}

////////////////////////////////////////////////////////////////////////////////

Cell* print_func(const FunctionCell* func, Cell* args) {
  Cell* argument_cell = single_argument_eval(func, args);

  cout << *argument_cell;
  cout << endl;
  
  return nil;   /// always return nil according to specs
}

////////////////////////////////////////////////////////////////////////////////

Cell* eval_func(const FunctionCell* func, Cell* args) {
  return eval(single_argument_eval(func, args));
}

////////////////////////////////////////////////////////////////////////////////

Cell* lambda_func(const FunctionCell* func, Cell* args) {
  if (ConsCell::get_list_size(args) < 2) {
    throw runtime_error("lambda need at least 2 arguments");
  }
  
  Cell* _args = car(args);
  Cell* _body = cdr(args);

  /// checks if all args are symbols for both cases: fixed and
  /// variable numbers of arguments
  if (listp(_args)) {
    Cell* pos = _args;
    while (!nullp(pos)) {
      if (!symbolp(car(pos))) {
	throw runtime_error("arguments have to be symbols!");
      }
      pos = cdr(pos);
    }
  }
  else if (!symbolp(_args)) {
    throw runtime_error("arguments have to be symbols!");
  }

  return lambda(_args, _body);
}

////////////////////////////////////////////////////////////////////////////////

Cell* apply_func(const FunctionCell* func, Cell* args) {
  if (nullp(args) || nullp(cdr(args))) {
    throw runtime_error("Number of arguments mismatch");
  }

  Cell* proc = eval(car(args));
  Cell* arguments = eval(car(cdr(args)));

  return proc->apply(arguments);
}

////////////////////////////////////////////////////////////////////////////////

Cell* let_func(const FunctionCell* func, Cell* args) {
  if (ConsCell::get_list_size(args) != 2) {
    throw runtime_error("let need exactly 2 arguments");
  }

  DefinitionManager::Instance()->add_stackframe();

  try {
    Cell* pos = car(args);
    Cell* func = car(cdr(args));

    /// create local variables
    while (!nullp (pos)) {
      string key = car(car(pos))->get_symbol();
      Cell* c = eval(car(cdr(car(pos))));
      DefinitionManager::Instance()->add_definition(key, c);

      pos = cdr(pos);
    }
   
    Cell* res = eval(func);

    DefinitionManager::Instance()->pop_stackframe();

    return res;
  }
  catch (runtime_error) {
    /// makes sure stackframe gets pop in case of an error
    DefinitionManager::Instance()->pop_stackframe();
    
    throw;
  }  
}
