/**
 * \file DefinitionManager
 *
 * \brief Enables a more flexibility in adding new definitions
 */

#ifndef DEFINITIONMANAGER_HPP
#define DEFINITIONMANAGER_HPP

#include "hashtablemap.hpp"

#include <map>
#include <vector>
#include <stdexcept>
#include "Cell.hpp"

using namespace std;

/**
 * \class DefinitionManager
 *
 * \brief Singleton Class which manages the "stack" of definition
 *        tables
 *
 * DefinitionManager implements a singleton pattern. Therefore the
 * constructor is private and the instance shall be called via
 * DefinitionMangager::Instance(). This approach enables the
 * DefinitionManager to initialize itself only ones (and also the
 * default global definition table), without the need to hook into the
 * entrypoint of the programm.
 */
class DefinitionManager {
public:
  
  /**
   * \brief Should be used to get the instance of this class. Will
   *        instantiate itself if is is not done yet. --> Singleton
   *        pattern
   */
  static DefinitionManager* Instance();
  
  /**
   * \brief Adds a local stack frame
   */
  void add_stackframe();

  /**
   * \brief Pops local stack frame
   */
  void pop_stackframe() throw (logic_error);

  /**
   * \brief Adds definition to the current stack frame
   */
  void add_definition(string key, Cell* c) throw (runtime_error);

  /**
   * \brief Checks if definition is available. Goes from through all "stack" frames
   */
  bool is_definition(string key);
  
  /**
   * \brief get the definition, will look through the whole vector "stack" in order
   *        to find occurence
   */
  Cell* get_definition(string key) const throw (runtime_error);

private:
  /// typedef aliases for readability
  typedef hashtablemap<string, Cell*> DefMap;

  static DefinitionManager* instance;
  static vector< DefMap > defs_stack_m;

  /**
   * \brief Constructor is private --> Singleton Pattern
   */
  DefinitionManager();
  
  /**
   * \brief Makes sure there is no copy constructor
   */
  DefinitionManager(DefinitionManager const&);
  
  /** 
   * \brief Makes sure no assignments are possible
   */
  void operator=(DefinitionManager const&);

};

#endif
