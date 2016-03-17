#include "FunctionManager.hpp"
#include "functions.hpp"

#include "cons.hpp"

/// define static members
map<string, FunctionManager::func> FunctionManager::func_defs_m;
FunctionManager* FunctionManager::instance = NULL;

FunctionManager::FunctionManager() {
  /// init -> is only supposed to be called once
  add_function("ceiling", &ceiling_func);
  add_function("floor",   &floor_func);
  add_function("quote",   &quote_func);
  add_function("cons",    &cons_func);
  add_function("car",     &car_func);
  add_function("cdr",     &cdr_func);
  add_function("intp",    &intp_func);
  add_function("doublep", &doublep_func);
  add_function("symbolp", &symbolp_func);
  add_function("nullp",   &nullp_func);
  add_function("listp",   &listp_func);
  add_function("if",      &if_func);
  add_function("define",  &define_func);
  add_function("<",       &less_than_func);
  add_function("not",     &not_func);
  add_function("print",   &print_func);
  add_function("eval",    &eval_func);
  add_function("lambda",  &lambda_func);
  add_function("apply",   &apply_func);
  add_function("let",     &let_func);

  /// CSI compatability
  add_function("int?",    &intp_func);
  add_function("double?", &doublep_func);
  add_function("symbol?", &symbolp_func);
  add_function("null?",   &nullp_func);
  add_function("list?",   &listp_func);
  add_function("'",       &quote_func);
}

/// Singleton Pattern
FunctionManager* FunctionManager::Instance() {
  if (instance == NULL) {
    instance = new FunctionManager();
  }
  return instance;
}

void FunctionManager::add_function(string key, func function) throw (logic_error) {
  pair<map<string, func>::iterator,bool> ret;
  ret = func_defs_m.insert(pair<string, func>(key, function));
  
  if (ret.second == false) {
    /// logic_error, since the user can not define functions by themselves (yet)
    throw logic_error("Can not redefine function!");
  }
}

bool FunctionManager::is_function(string fname) {
  return !(func_defs_m.find(fname) == func_defs_m.end());
}

Cell* FunctionManager::call_function(const FunctionCell* func_cell, Cell* args) throw (runtime_error) {
  string fname = func_cell->get_symbol();

  if (!is_function(fname)) {
    throw runtime_error( fname + " is undefined" );
  }

  /// resolve function pointer to a callable function
  Cell* (*func)(const FunctionCell*, Cell*);
  func = func_defs_m[fname];

  return func(func_cell, args);
}
