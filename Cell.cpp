/**
 * \file Cell.cpp
 *
 * Implementation of the Cell.hpp interface
 */

#include "Cell.hpp"
#include "eval.hpp"
#include "FunctionManager.hpp"
#include "DefinitionManager.hpp"

#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>

Cell* const nil = new SentinelCell();

using namespace std;


//////////////////////////////////////////
// CellABC

CellABC::~CellABC() {}

bool CellABC::is_int() const { 
  return 0;
}

bool CellABC::is_double() const {
  return 0;
}

bool CellABC::is_symbol() const {
  return 0;
}

bool CellABC::is_cons() const {
  return 0;
}

bool CellABC::is_lambda() const {
  return 0;
}

int CellABC::get_int() const throw (runtime_error) {
  throw runtime_error("Cell does not contain an integer");
}

double CellABC::get_double() const throw (runtime_error) {
  throw runtime_error("Cell does not contain a double");
}

double CellABC::get_numeral() const throw (runtime_error) {
  throw runtime_error("Cell does neither contain an integer nor a double");
}

string CellABC::get_symbol() const throw (runtime_error) {
  throw runtime_error("Cell does not contain an symbol");
}

CellABC* CellABC::get_car() const throw (runtime_error) {
  throw runtime_error("Cell is not a ConsPair");
}

CellABC* CellABC::get_cdr() const throw (runtime_error) {
  throw runtime_error("Cell is not a ConsPair");
}

CellABC* CellABC::get_formals() const throw (runtime_error) {
  throw runtime_error("Cell is not a ProcedurePair");
}

CellABC* CellABC::get_body() const throw (runtime_error) {
  throw runtime_error("Cell is not a ProcedurePair");
}

CellABC* CellABC::get_definition() const throw (runtime_error) {
  throw runtime_error("Cell is not a defined SymbolCell");
}

CellABC* CellABC::apply(CellABC* const args) const throw (runtime_error) {  
  throw runtime_error("Cell is not a FunctionCell");
}

void SentinelCell::print(std::ostream& os) const {
   os << "()";
}

//////////////////////////////////////////
// IntCell

IntCell::IntCell(int const i) : content_m(i) {}

bool IntCell::is_int() const {
  return 1;
}
  
int IntCell::get_int() const throw (runtime_error) {
  return content_m;
}

double IntCell::get_numeral() const throw (runtime_error) {
  return get_int();
}

void IntCell::print(std::ostream& os) const {
  os << content_m;
}

//////////////////////////////////////////
// DoubleCell

DoubleCell::DoubleCell(double const d) : content_m(d) {}


bool DoubleCell::is_double() const {
  return 1;
}
  
double DoubleCell::get_double() const throw (runtime_error) {
  return content_m;
}

double DoubleCell::get_numeral() const throw (runtime_error) {
  return get_double();
}

void DoubleCell::print(std::ostream& os) const {
  os << std::setprecision(6) << std::fixed;
  os << content_m;
}

//////////////////////////////////////////
// SynmbolCell

SymbolCell::SymbolCell(const char* const s) {
  //Make a deepcopy of the parameter value to ensure that the user's variable
  //will be untouched
  content_m = new char[strlen(s)+1];
  strcpy(content_m, s);
}

SymbolCell::~SymbolCell() {
  if(content_m != NULL) {
    delete content_m;
    content_m = NULL;
  }
}

bool SymbolCell::is_int() const {
  return get_definition()->is_int();
}

bool SymbolCell::is_double() const {
  return get_definition()->is_double();
}

bool SymbolCell::is_symbol() const {
  return 1;
}

Cell* SymbolCell::get_definition() const throw (runtime_error) {
  return DefinitionManager::Instance()->get_definition(content_m);
}

int SymbolCell::get_int() const throw (runtime_error) {
  return get_definition()->get_int();
}

double SymbolCell::get_double() const throw (runtime_error) {
  return get_definition()->get_double();
}

double SymbolCell::get_numeral() const throw (runtime_error) {
  return get_definition()->get_numeral();
}

std::string SymbolCell::get_symbol() const throw (runtime_error) {
  return content_m;
}
  
void SymbolCell::print(std::ostream& os) const {
  os << content_m;
}

bool SymbolCell::is_defined(string key) {
  return DefinitionManager::Instance()->is_definition(key);
}

void SymbolCell::add_definition(string key, Cell* val) throw (runtime_error) {
  DefinitionManager::Instance()->add_definition(key, val);
}

//////////////////////////////////////////
// ConsCell

ConsCell::ConsCell(Cell* const my_car, Cell* const my_cdr) : car(my_car), cdr(my_cdr) {}

ConsCell::~ConsCell() {
  if(car != nil) {
    delete car;
    car = NULL;
  }
  if(cdr != nil) {
    delete cdr;
    cdr = NULL;
  }
}

int ConsCell::get_list_size(Cell* head) {
  if (head == nil) {
    return 0;
  }

  int counter = 0;

  while(head != nil) {
    head = head->get_cdr();
    ++counter;  
  }

  return counter;
}

bool ConsCell::is_cons() const {
  return 1;
}
  
Cell* ConsCell::get_car() const throw (runtime_error) {
  return car;
}

Cell* ConsCell::get_cdr() const throw (runtime_error) {
  return cdr;
}

void ConsCell::print(ostream& os) const {
  string cdr_sexpr = get_sexpr(get_cdr());
  string car_sexpr = get_sexpr(get_car());
  
  /// In short it makes sure that not every bracket of cons will be shown
  /// (2 (3 (4 ()))) ==> (2 3 4) 
  if (cdr_sexpr == "()") { 
    os << "(" + car_sexpr + ")"; 
    return;
  } else if (cdr_sexpr[0] == '(') {
    os << "(" + car_sexpr
      + " "
      + cdr_sexpr.substr(1, cdr_sexpr.length() - 1);
    return;
  } else {
    throw runtime_error("Error: List is not correct");
  }
}

string ConsCell::get_sexpr(Cell* c) {
  ostringstream outs;

  /// recursive approach to print out ConsPair Lists
  c->print(outs);

  return outs.str();
}



//////////////////////////////////////////
// ArithmeticCell

ArithmeticCell::ArithmeticCell(const char* const s) : SymbolCell(s) {};

bool ArithmeticCell::is_arithmetic(string s) {
  if (s == "+" || s == "-" || s == "/" || s == "*") {
    return 1;
  }
  return 0;
}

Cell* ArithmeticCell::get_identity() const throw (runtime_error) {
  string op = get_symbol();
    
  if (op == "+") {
    return (Cell*) new IntCell(0);
  }
  else if (op == "*") {
    return (Cell*) new IntCell(1);
  }
  else {
    throw runtime_error("- and / cannot have zero arguments!");
  }
}

Cell* ArithmeticCell::apply(Cell* const args) const throw (runtime_error) {
  if (nullp(args)) {                        /// no arguments
    return get_identity();
  } 
  else if (nullp(cdr(args))) {              /// only one argument
    Cell* argument = eval(car(args));
    return calculate(argument);
  }
  else {
    Cell* pos = args;
    Cell* result = eval(car(pos));
    pos = cdr(pos);

    while (!nullp(pos)) {
      Cell* c1 = eval(car(pos));

      // Arithmetic Cell itself deals with the calculations
      result = this->calculate(result, c1);

      pos = cdr(pos);
    }

    return result;
  }
}

Cell* ArithmeticCell::calculate(Cell* c) const {
  string op = get_symbol();
    
  double num = c->get_numeral();
    
  if (op == "-") {
    num = -1*num;
  } 
  else if (op == "/") {
    if (num == 0) {
      throw runtime_error("Can not devide by zero");
    }
    num = 1/num;
  }

  /// Nothing to do for + and * operation
  
  if (c->is_double()) {
    return (Cell*) new DoubleCell(num);
  }
  return (Cell*) new IntCell((int) num);
    
}

Cell* ArithmeticCell::calculate(Cell* c1, Cell* c2) const throw (std::runtime_error) {
  string op = get_symbol();
  double result = 0;

  double num1 = c1->get_numeral();
  double num2 = c2->get_numeral();

  if (op == "+") {
    result = num1 + num2;
  }
  else if (op == "-") {
    result = num1 - num2;
  }
  else if (op == "*") {
    result = num1 * num2;
  }
  else if (op == "/") {
    if (num2 == 0) {
      throw runtime_error("Can not devide by zero");
    }
    result = num1 / num2;
  }
  if (c1->is_double() || c2->is_double()) {
    return (Cell*) new DoubleCell(result);
  }
  return (Cell*) new IntCell((int) result);

}


//////////////////////////////////////////
// FunctionCell

FunctionCell::FunctionCell(const char* const s) : SymbolCell(s) {};

bool FunctionCell::is_function(string fname) {
  return FunctionManager::Instance()->is_function(fname);
}

Cell* FunctionCell::apply(Cell* const args) const throw (runtime_error) {
  string fname = get_symbol();                    // just for readability

  if(args == nil && fname != "<") {
     string msg = fname                           // provides function name
      + " cannot be called without any argument"; // for 'backtracking' bugs
    throw runtime_error(msg.c_str());
  }
  
  /// this pointer is given to the program in order to give the
  /// function more information. E.g. for generalised error_handlers it can
  /// dump a simple backtrace
  return FunctionManager::Instance()->call_function(this, args);
}



//////////////////////////////////////////
// ProcedureCell

ProcedureCell::ProcedureCell(Cell* const my_param, Cell* const my_body) : param(my_param), body(my_body) {
  if (!listp(my_param)) {
    
    /// Indicates variable number of arguments
    num_param = -1;
  }
  else {
    num_param = ConsCell::get_list_size(my_param);
  }
}

ProcedureCell::~ProcedureCell() {
  if(param != nil) {
    delete param;
    param = NULL;
  }
  if(body != nil) {
    delete body;
    body = NULL;
  }
}


bool ProcedureCell::is_lambda() const {
  return 1;
}

Cell* ProcedureCell::get_formals() const throw (runtime_error) {
  return param;
}

Cell* ProcedureCell::get_body() const throw (runtime_error) {
  return body;
}

Cell* ProcedureCell::apply(Cell* const args) const throw (std::runtime_error) {

  /// In other words: if num_param -1 then there is a variabe number
  /// of arguments
  if (num_param != -1 && ConsCell::get_list_size(args) != num_param) {
    stringstream ss;
    ss << "Mismatch of number of arguments in:" << endl;
    ss << "\t" << *this->get_body() << endl;

    throw runtime_error(ss.str());
  }

  Cell* pos_args  = args;
  Cell* pos_param = this->get_formals();
  Cell* pos_body  = this->get_body();

  DefinitionManager::Instance()->add_stackframe();

  try {

    /// define local variables in local stack frame
    if (num_param == -1) {
      string key = pos_param->get_symbol();
      Cell* c = pos_args;
      DefinitionManager::Instance()->add_definition(key, c);
    }
    else {
      while (!nullp(pos_args)) {
	string key = (car(pos_param))->get_symbol();
	Cell* c = eval(car(pos_args));
	DefinitionManager::Instance()->add_definition(key, c);
	
	pos_args  = cdr(pos_args);
	pos_param = cdr(pos_param);
      }
    }

    /// evaluates bodies, remembers last result
    Cell* res = nil;
    while (!nullp(pos_body)) {
      res = eval(car(pos_body));
      
      /// in case of variable number of args, eval has to be called,
      /// since we are forced to skipped the step above in defining the
      /// variable
      if (!nullp(res) && num_param == -1) {
	res = eval(res);
      }
      
      pos_body = cdr(pos_body);
    }
    
    DefinitionManager::Instance()->pop_stackframe();
    
    /// Note: only returns last remembered result according to specs
    return res;
    
  }
  catch (runtime_error) {
    /// makes sure stackframe gets pop in case of an error
    DefinitionManager::Instance()->pop_stackframe();
    
    throw;
  }
}

void ProcedureCell::print(ostream& os) const {
  os << "#<function>";
}
