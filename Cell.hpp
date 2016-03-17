/**
 * \file Cell.hpp
 *
 * Encapsulates the abstract interface for a concrete class-based 
 * implementation of cells. Has CellABC (typedef alias Cell) as Abstract Base 
 * Class. Polymorphism is used in this case to get rid of numerous if-then 
 * statements, having cleaner implementations.
 */

#ifndef CELL_HPP
#define CELL_HPP

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdexcept>
#include <functional>

////////////////////////////////////////////////////////////////////////////////
///
///     ##Outline:##
///     1. The Abstract Base Class: CellABC
///     2. Cells containing Data: IntCell, DoubleCell, SymbolCell, ConsCell
///     3. Cells which are able to call functions: FunctionCell, 
///        ArithmeticCell 
///
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///   1. The Abstract Base Class
////////////////////////////////////////////////////////////////////////////////

/**
 * \class CellABC
 * \brief Abstract Base Class (ABC) for Cells. Polymorphism is used in this 
 *        case to get rid of numerous if-then statements, having cleaner 
 *        implementations.<br />
 *        <b>Important:</b> A typedef alias Cell is used in order to comply to 
 *        the given interface.
 */
class CellABC {
public:

  /**
   * \brief Make sure derived classes such as SymbolCell cleans up properly
   */
  virtual ~CellABC();
  
  /**
   * \brief Checks if it is an integer. Remarks: returns 0 (false) by default
   *        should be overritten by IntCell
   */
  virtual bool is_int() const;

  /**
   * \brief Checks if it is a double. Remarks: returns 0 (false) by default
   *        should be overritten by DoubleCell
   */
  virtual bool is_double() const;

  /**
   * \brief Checks if it is a Symbol. Remarks: returns 0 (false) by default
   *        should be overritten by SymbolCell
   */
  virtual bool is_symbol() const;

  /**
   * \brief Checks if it is a ConsPair. Remarks: returns 0 (false) by default
   *        should be overritten by ConsCell
   */
  virtual bool is_cons() const;

  /**
   * \brief Checks if it is a ProcedureCell. Remarks: returns 0 (false) by default
   *        should be overritten by ProcedureCell.
   */
  virtual bool is_lambda() const;
   
  /**
   * \brief Accessor (error if this is not an int cell). Remarks: IntConsCell has
   *        to override this method
   */
  virtual int get_int() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a double cell). Remarks: DoubleCell has
   *        to override this method
   */
  virtual double get_double() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not an int or double cell). Remarks: IntCell
   *        and DoubleCell have to override this method
   */
  virtual double get_numeral() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a symbol cell). Remarks: SymbolConsCell has
   *        to override this method
   */
  virtual std::string get_symbol() const throw (std::runtime_error);
  
  /**
   * \brief Accessor (error if this is not a cons cell). Remarks: ConsCell has
   *        to override this method
   */
  virtual CellABC* get_car() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a cons cell). Remarks: ConsCell has
   *        to override this method
   */
  virtual CellABC* get_cdr() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a procedure cell). Remarks:
   *        ProcedureCell has to override this method
   */
  virtual CellABC* get_formals() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a procedure cell). Remarks:
   *        ProcedureCell has to override this method
   */
  virtual CellABC* get_body() const throw (std::runtime_error);

  /**
   * \brief Accessor (error if this is not a SymbolCell with definition). Remarks: 
   *        SymbolCell has override this method
   */
  virtual CellABC* get_definition() const throw (std::runtime_error);

  /**
   * \brief Generalisation for all functions
   */
  virtual CellABC* apply(CellABC* const args) const throw (std::runtime_error);
  
  /**
   * \brief Requires the child class to specify how to print out its content.
   * \param os The output stream to print to.
   */
  virtual void print(std::ostream& os = std::cout) const = 0;

};

/// Using a typedef to comform to the type-namings of the given interface in
/// cons.hpp
typedef CellABC Cell;

/**
 * \class SentinelCell
 * \brief Is used for nil. Prevents SigFaults when accessing memberfunction on 
 *        nil.
 */
class SentinelCell : public Cell {
  virtual void print(std::ostream& os = std::cout) const;
};


// Reminder: cons.hpp expects nil to be defined somewhere (for this
// implementation, Cell.cpp is the logical place to define it).
// Here we promise this again, just to be safe.
extern Cell* const nil;


////////////////////////////////////////////////////////////////////////////////
///   2. Cells containing Data
////////////////////////////////////////////////////////////////////////////////

/**
 * \class IntCell
 * \brief Implements CellABC for Cells containing an integer
 */
class IntCell : public Cell {
public:
  /**
   * \brief Constructor to make int cell.
   */
  IntCell(int const i);

  /**
   * \brief Implements type check of the Cell ABC
   * \return true if Cell is a IntCell
   */
  virtual bool is_int() const;

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual int get_int() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual double get_numeral() const throw (std::runtime_error);

  /**
   * \brief Specifies how the content of this type of Cell should be
   *        printed
   */
  virtual void print(std::ostream& os = std::cout) const;

private:
  int content_m;

};


/**
 * \class DoubleCell
 * \brief Implements CellABC for Cells containing a double
 */
class DoubleCell : public Cell {
public:
  /**
   * \brief Constructor to make double cell.
   */
  DoubleCell(double const d);

  /**
   * \brief Implements type check of the Cell ABC
   * \return true if Cell is a DoubleCell
   */
  virtual bool is_double() const;
  
  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual double get_double() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual double get_numeral() const throw (std::runtime_error);

  /**
   * \brief Specifies how the content of this type of Cell should be
   *        printed
   */
  virtual void print(std::ostream& os = std::cout) const;

private:
  double content_m;

};


/**
 * \class SymbolCell
 * \brief Implements CellABC for Cells containing a sybmol. _Note:_ Is also 
 *        BaseClass for FunctionCell and ArithmeticCell 
 */
class SymbolCell : public Cell {
public:
  /**
   * \brief Constructor to make symbol cell.
   */
  SymbolCell(const char* const s);

  /**
   * \brief Makes sure that char* content is freed on destruction (even when 
   *        Base class is deleted)
   */
  virtual ~SymbolCell();

  /**
   * \brief Implements type check of the Cell ABC, in this case used for runtime
   *        defined variables (definitions)
   * \return true if Cell is a SymbolIntCell
   */
  virtual bool is_int() const;

  /**
   * \brief Implements type check of the Cell ABC, in this case used for runtime
   *        defined variables (definitions)
   * \return true if Cell is a SymbolIntCell
   */
  virtual bool is_double() const;

  /**
   * \brief Implements type check of the Cell ABC
   * \return true if Cell is a SymbolIntCell
   */
  virtual bool is_symbol() const;

  /**
   * \brief Implements Accessor of the Cell ABC, in this case used for runtime
   *        defined variables (definitions)
   */
  virtual int get_int() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC, in this case used for runtime
   *        defined variables (definitions)
   */
  virtual double get_double() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC, in this case used for runtime
   *        defined variables (definitions)
   */
  virtual double get_numeral() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual std::string get_symbol() const throw (std::runtime_error);

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual Cell* get_definition() const throw (std::runtime_error);

  /**
   * \brief Specifies how the content of this type of Cell should be
   *        printed
   */
  virtual void print(std::ostream& os = std::cout) const;
  
  /**
   * \brief static func to add new definitions. _Note:_ static function, in order
   *        to append to static defs_m map. (independent, but shared within the
   *        class
   */
  static void add_definition(std::string key, Cell* val) throw (std::runtime_error);

private:
  char* content_m;
  
  /** 
   * \brief checks internally (private), if symbol is already defined
   */
  static bool is_defined(std::string key);
};


/**
 * \class ConsCell
 * \brief Implements CellABC for Cells containing a ConsPair. Uses Lisp ConsPair
 *        List approach in order to have a powerful general purpose data type.
 */
class ConsCell : public Cell {  
public:
  /**
   * \brief Constructor to make cons cell.
   */
  ConsCell(Cell* const my_car, Cell* const my_cdr);

  /**
   * \brief Make sure it cleans up properly
   */
  virtual ~ConsCell();

  /**
   * \brief Knowing the size can be convenient in many cases. E.g. to 
   *        get to know how many arguments there are
   * \return int size or in other words the number of hops until nil /
   *         terminating cell reached
   */
  static int get_list_size(Cell* head);
  
  /**
   * \brief Implements type check of the Cell ABC
   * \return true if Cell is a ConsCell
   */
  virtual bool is_cons() const;

  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual Cell* get_car() const throw (std::runtime_error);
  
  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual Cell* get_cdr() const throw (std::runtime_error);

  /**
   * \brief Specifies how the content of this type of Cell should be
   *        printed
   */
  virtual void print(std::ostream& os = std::cout) const;

private:
  Cell* car;
  Cell* cdr;

  /**
   * \brief Prints out s-expression, resolves recursively. Is static since no
   *        instance is needed.
   */
  static std::string get_sexpr(Cell* const c);

};



////////////////////////////////////////////////////////////////////////////////
///   3. Cells which are able to call functions
////////////////////////////////////////////////////////////////////////////////

/**
 * \class ArithmeticCell
 * \brief Derived from SymbolCell. Is used for functions which are chainable
 *        or in other words, do not have fixed number of arguments.
 */
class ArithmeticCell : public SymbolCell {
public:

  /**
   * \brief Constructor for an ArithmeticCell, which is capable to compute
   *        simple calculation. _Note:_ You can check available operation
   *        with ArithmeticCell::is_arithmetic()
   */
  ArithmeticCell(const char* const s);


  /**
   * \brief Checks if operator is currently available. Static since no instance
   *        is required.
   */
  static bool is_arithmetic(std::string s);

  /**
   * \brief Method to execution a single calculation
   */
  Cell* calculate(Cell* c1, Cell* c2) const throw (std::runtime_error);
  
  /**
   * \brief Used for generalization, and getting rid of various if-else
   *        statements. Overrides CellABC's method.
   */
  virtual Cell* apply(Cell* const args) const throw (std::runtime_error);

private:  
  /**
   * \brief Used e.g. for zero argument calls. Most likely, can be useful
   *        for other tasks
   * \throw runtime_error when called by - or / operator
   */
  Cell* get_identity() const throw (std::runtime_error);

  /**
   * \brief Used for single argument calls. 
   */
  Cell* calculate(Cell* c) const;
};


/**
 * \class FunctionCell
 * \brief Derived from SymbolCell. Is used for functions which have fixed
 *        number of arguments.
 */
class FunctionCell : public SymbolCell {
public:

  /**
   * \brief Constructor for an FunctionCell, which is capable to handle 
   *        functions with fixed paramterss (as opposed to the
   *        ArithmeticCell). _Note:_ You can check available operation
   *        with FunctionCell::is_arithmetic()
   */
  FunctionCell(const char* const s);

  /**
   * \brief Checks if function is currentlyavailable. Static since no 
   *        instance is required.
   */
  static bool is_function(std::string fname);

  /**
   * \brief Used for generalization, and getting rid of various if-else
   *        statements. Overrides CellABC's method.
   */
  virtual Cell* apply(Cell* const args) const throw (std::runtime_error);
  
};


/**
 * \class ProcedureCell
 *
 * \brief Implements CellABC for Cells containing a procedure. Uses
 *        Lisp ConsPair List approach in order to have a powerful
 *        general purpose data type.
 */
class ProcedureCell : public Cell {  
public:
  /**
   * \brief Constructor to make cons cell.
   */
  ProcedureCell(Cell* const my_car, Cell* const my_cdr);

  /**
   * \brief Make sure it cleans up properly
   */
  virtual ~ProcedureCell();

  /**
   * \brief Implements type check of the Cell ABC
   * \return true if Cell is a ProcedureCell
   */
  virtual bool is_lambda() const;

  /// \todo do we need that?
  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual Cell* get_formals() const throw (std::runtime_error);
  
  /**
   * \brief Implements Accessor of the Cell ABC
   */
  virtual Cell* get_body() const throw (std::runtime_error);

  /**
   * \brief Used for generalization, and getting rid of various if-else
   *        statements. Overrides CellABC's method.
   */
  virtual Cell* apply(Cell* const args) const throw (std::runtime_error);

  /**
   * \brief Specifies how the content of this type of Cell should be
   *        printed
   */
  virtual void print(std::ostream& os = std::cout) const;

private:
  Cell* param;
  Cell* body;

  /** \brief -1 Indicates variable number of arguments */
  int num_param;

};


#endif // CELL_HPP
