/**
 * \file FunctionManager
 *
 * \brief Enables a more flexibility in adding new functions
 */

#ifndef FUNCTIONMANAGER_HPP
#define FUNCTIONMANAGER_HPP

#include <map>
#include <stdexcept>
#include "Cell.hpp"

using namespace std;

/**
 * \class FunctionManager
 * \brief FunctionManager implements a singleton pattern. Therefore the constructor
 *        is private and the instance shall be called via FunctionMangager::Instance().
 *        In the constructor the by default available functions are added. This 
 *        approach enables the FunctionManager to initialize itself only ones (and also
 *        the default functions), without the need to hook into the entrypoint of the
 *        programm.
 */
class FunctionManager {
public:
  
  /**
   * \brief Should be used to get the instance of this class. Will instantiate itself
   *        if is is not done yet. --> Singleton pattern
   */
  static FunctionManager* Instance();

  /**
   * \typedef func typedef for our function pointers
   */
  typedef Cell*(*func)(const FunctionCell*, Cell*);
  
  /**
   * \brief Adds function to the function pointer map
   * \throw logic_error if function already defined
   */
  void add_function(string key, func function) throw (logic_error);

  /**
   * \brief Checks if symbol is mapped with function pointer
   */
  bool is_function(string key);
  
  /**
   * \brief calls function given the key
   */
  Cell* call_function(const FunctionCell* func_cell, Cell* args) throw (runtime_error);

private:
  static FunctionManager* instance;
  static std::map<string, func> func_defs_m;

  /// Singleton Pattern
  /**
   * \brief Constructor is private --> Singleton Pattern
   */
  FunctionManager();
  
  /**
   * \brief Makes sure there is no copy constructor
   */
  FunctionManager(FunctionManager const&);
  
  /** 
   * \brief Makes sure no assignments are possible
   */
  void operator=(FunctionManager const&);

};

#endif
