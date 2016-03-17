#include "DefinitionManager.hpp"

#include "cons.hpp"

/// define static members
vector<DefinitionManager::DefMap> DefinitionManager::defs_stack_m;
DefinitionManager* DefinitionManager::instance = NULL;

DefinitionManager::DefinitionManager() {
  add_stackframe();  /// creates global definition table
};

/// Singleton Pattern
DefinitionManager* DefinitionManager::Instance() {
  if (instance == NULL) {
    instance = new DefinitionManager();
  }
  
  return instance;
}

void DefinitionManager::add_stackframe() {
  defs_stack_m.push_back(DefMap());
}

void DefinitionManager::pop_stackframe() throw (logic_error) {
  if (defs_stack_m.size() < 1) {
    throw logic_error("Logic error in the frame management of definition stack");
  }
  defs_stack_m.pop_back();
}

void DefinitionManager::add_definition(string key, Cell* c) throw (runtime_error) {
  DefMap& def_map = defs_stack_m.back();

  pair<DefMap::iterator, bool> ret = def_map.insert(pair<string, Cell*>(key, c));

  if (ret.second == false) {
    throw runtime_error("Can not redefine symbol!");
  }  
}

bool DefinitionManager::is_definition(string key) {
  for (vector<DefMap>::reverse_iterator i = defs_stack_m.rbegin(); i < defs_stack_m.rend(); ++i) {
    if (!((*i).find(key) == (*i).end())) {
	return true;
    }
  }
  return false; 
}

Cell* DefinitionManager::get_definition(string key) const throw (runtime_error) {
  for (vector<DefMap>::reverse_iterator i = defs_stack_m.rbegin(); i < defs_stack_m.rend(); ++i) {
    if (!((*i).find(key) == (*i).end())) {
      return (*i)[key];
    }
  }
  throw runtime_error("Symbol is not defined!");
}
