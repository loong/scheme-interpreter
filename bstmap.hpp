/**
 * \file bstmap.hpp
 *
 * Joel Berago and Long Hoang have implemented together the first step
 * (because Long forgot to bring his charger to class). But most of
 * our code has been developed individually.
 */

#ifndef BSTMAP_HPP
#define BSTMAP_HPP

#include <utility>
#include <cstddef>
#include <iterator>
#include <iostream>

#include <cassert>
#include <stdexcept>

using namespace std;

/**
 * \class bstmap
 *
 * \brief bstmap is a template, which implements a binary search
 *        tree. It follows STL idiom for easy plug and play
 */
template <class Key, class T>
class bstmap
{
  typedef bstmap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;

private:
  /**
   * \class Node
   *
   * \brief Holds value (which is a keypair). Has two child
   *        (BinaryTree), but also point to * the parent for easier
   *        implementation
   */
  class Node {
  public:
    Node(value_type val, Node* parent = NULL,  Node* left = NULL, Node* right = NULL) :
      value_m(val), parent_m(parent), left_m(left), right_m(right) {}

    /**
     * \brief sets to NULL to prevent OS to reuse garbage
     */
    ~Node() {
      parent_m = NULL;
      left_m = NULL;
      right_m = NULL;
    }

    value_type value_m;
    Node* parent_m;
    Node* left_m;
    Node* right_m;
  };

  Node* root_m;

  ////////////////////////////////////////////////////////////////////////////////
  /// Iterators
  ////////////////////////////////////////////////////////////////////////////////
public: 
  /**
   * \brief Template in order to create iterator and const_iterator
   *        without copy pasting
   */
  template <typename _T>
  class _iterator {
    friend class bstmap;

  public:
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    _iterator(Node* n = NULL) : node_m(n) {}
    _iterator(const _iterator& x) : node_m(x.node_m) {}
    
    _iterator& operator=(const _iterator& x) {
      node_m = x.node_m;
      return *this;
    }

    reference operator*() {
      return node_m->value_m;
    }

    pointer operator->() {
      return &(node_m->value_m);
    }

    bool operator==(const _iterator& x) {
      return (node_m == x.node_m);
    }

    bool operator!=(const _iterator& x) {
      return (node_m != x.node_m);
    }

    _iterator& operator++() {
      node_m = _successor(node_m);
      return *this;
    }

    _iterator operator++(int) {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

    Node* node_m;
  };
  
  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;

public:
  ////////////////////////////////////////////////////////////////////////////////
  /// Constructors, Destructor, Assignment
  ////////////////////////////////////////////////////////////////////////////////

  bstmap() : root_m(NULL) {}

  ~bstmap() {
    clear();
  }

  /**
   * \brief overloads copy constructor for deep copy
   */
  bstmap(const Self& x) : root_m(NULL) {
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// insert one by one, which will be aweful for
		       /// the treestructure, unfortunatly not enough
		       /// time to balance tree properly --> Winterproject!
    }
  }

  /**
   * \brief overloads assignment for deep copy
   */
  Self& operator=(const Self& x) {
    // guard against self assignment
    if (this == &x) {
      return *this;
    }

    clear();
    
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// same balancing problem as for copy constructor above
    }
  }

  ////////////////////////////////////////////////////////////////////////////////
  /// Accessors
  ////////////////////////////////////////////////////////////////////////////////

  iterator begin() {
    return iterator(leftmost_node());
  }

  const_iterator begin() const {
    return const_iterator(leftmost_node());
  }

  iterator end() {
    if (empty()) {
      return begin();
    }

    return iterator(NULL); // basically a wraper around NULL
  }

  const_iterator end() const {
    if (empty()) {
      return begin();
    }
    
    return const_iterator(NULL); // basically a wraper around NULL
  }

  bool empty() const {
    return root_m == NULL;
  }

  size_type size() const {
    return _size(root_m);
  }

  /**
   * \return pair<iterator, bool> iterator to node in tree, bool is
   *         true if an insertion has been done
   */
  pair<iterator, bool> insert(const value_type& x) {
    pair<Node*, bool> res = _find(x.first, root_m);

    if (res.second == true) {  // means value is already in tree
      return pair<iterator, bool>(iterator(res.first), false);
    }
    
    if (res.first == NULL) {
      root_m = new Node(x); // First Node insertaion
      return pair<iterator, bool>(iterator(root_m), true);
    }

    // create new node
    Node* new_node = new Node(x, res.first);
    if (x.first < (res.first->value_m).first) {
      res.first->left_m = new_node;      
    }
    else {
      res.first->right_m = new_node;
    }

    return pair<iterator, bool>(iterator(new_node), true);
  }

  void erase(iterator pos) throw (runtime_error) {
    if (pos == NULL) {
      throw runtime_error("Cannot erase Null iterator");
    }

    Node* n = pos.node_m;
    Node* parent = n->parent_m;
    
    if (n == NULL) {
      return;
    }

    // Case 3: x has 2 childs
    if (n->left_m != NULL && n->right_m != NULL) {
      Node* successor = _successor(n);

      // replace to deleted node with successor
      Node* _parent = parent;
      Node* _left = n->left_m;
      Node* _right = n->right_m;

      delete n;
      n = new Node(successor->value_m, _parent, _left, _right);
      
      // delete original successor
      erase(iterator(successor));

    } 
    else if (n->left_m == NULL && n->right_m == NULL) {
      // Case 1: x is a leaf      
      if (parent != NULL) {
	if (parent->left_m == n) {
	  parent->left_m = NULL;
	}
	else {
	  parent->right_m = NULL;
	}
      }
      else {
	/// \todo verify!
	root_m = NULL;
      }
      delete n;

    } else {

      // Case 2: x has exactly one child
      Node* child = n->left_m;
      if (child == NULL) {
	child = n->right_m;
      }
      
      // connect child of deleted node with its parent directly
      // special case: deleting root
      if (n == root_m) {
	root_m = child;
      }
      else {
	if (n == parent->left_m) {
	  parent->left_m = child;
	}
	else {
	  parent->right_m = child;
	}
      }

      child->parent_m = parent;
      
      delete n;
    }
    
    return;
  }
  
  /**
   * \brief erase which uses Key value 
   * 
   * \return size_type number of elements erased (in this case can
   *         only be 1 or 0)
   */
  size_type erase(const Key& x) {
    iterator it = find(x);
    
    if (it == end()) { // Key not found
      return 0;
    }

    erase(it);

    return 1; // since Key in maps are unique, can only be 1
  }
  
  /**
   * \brief clears all elements inside of the map
   */
  void clear() {
    _recursive_delete(root_m);
    root_m = NULL;
  }

  
  iterator find(const Key& x) {
    pair<Node*, bool> res = _find(x, root_m);

    if (res.second) { // found key
      return iterator(res.first);
    }
    
    return end();
  }

  const_iterator find(const Key& x) const {
    pair<Node*, bool> res = _find(x, root_m);

    if (res.second) { // found key
      return const_iterator(res.first);
    }
    
    return end();
  }

  /**
   * \brief Because all elements in a map container are unique, the
   * function can only return 1 (if the element is found) or zero
   * (otherwise).
   */
  size_type count(const Key& x) const {
    if (find(x) != end()) {
      return 1;
    }
    return 0;
  }

  /**
   * \brief subscript operator. Remarks: if Key not available, Key
   *        with default value will be initialised
   */
  T& operator[](const Key& k) {
    iterator it = find(k);
    
    if (it != end()) {   // found key
      return (*it).second;
    }
    else {               // not found -> create empty
      insert( value_type(k, T()));
      return operator[](k);   // a bit ugly
    }
  }

  value_type& min() throw (runtime_error) {
    if (empty()) {
      throw runtime_error("Map is empty");
    }
    return leftmost_node()->value_m;
  }

  value_type& max() throw (runtime_error) {
    if (empty()) {
      throw runtime_error("Map is empty");
    }
    return rightmost_node()->value_m;
  }
  
private:

  ////////////////////////////////////////////////////////////////////////////////
  /// Helpers
  ////////////////////////////////////////////////////////////////////////////////
  
  /**
   * \return Node* nearest node to found
   * \return bool true if found
   */
  pair<Node*, bool> _find(const key_type& key, Node* subtree, Node* parent = NULL) const {
    if (subtree == NULL) {
      return pair<Node*, bool>(parent, false);
    }
    
    if (subtree->value_m.first == key) {
      return pair<Node*, bool>(subtree, true);
    }
    else if (subtree->value_m.first > key) {
      return _find(key, subtree->left_m, subtree);
    }
    else {
      return _find(key, subtree->right_m, subtree);
    }
  }

  /**
   * \brief used as const wrapper for global calls
   */
  Node* leftmost_node() const { 
    return _leftmost_rec(root_m); 
  }

  /**
   * \brief used as const wrapper for global calls
   */
  Node* rightmost_node() const { 
    return _rightmost_rec(root_m); 
  }

  /**
   * \brief get the left most, generalized so it can run on any subtree
   */
  static Node* _leftmost_rec(Node* subtree) {
    if (subtree != NULL) {
      while (subtree->left_m != NULL) {
	subtree = subtree->left_m;
      }
    }
    return subtree;
  }

  /**
   * \brief get the right most, generalized so it can run on any subtree
   */
  static Node* _rightmost_rec(Node* subtree) {    
    if (subtree != NULL) {
      while (subtree->right_m != NULL) {
	subtree = subtree->right_m;
      }
    }

    return subtree;
  }

  /**
   * \brief get successor for any node
   */
  static Node* _successor(Node* n) {
    if (n->right_m != NULL) {
      return _leftmost_rec(n->right_m);
    }
    else {
      Node* pos = n;
      Node* pos_parent = n->parent_m;
      while (pos_parent != NULL && pos == pos_parent->right_m) {
	pos = pos_parent;
	pos_parent = pos_parent->parent_m;
      }
	
      return pos_parent;
    }
  }

  /**
   * \brief use this for tree-wise deletion, NOT for single deletion!
   */
  static void _recursive_delete(Node* n) {
    if (n == NULL) {
      return;
    }

    _recursive_delete(n->right_m);
    _recursive_delete(n->left_m);  

    delete n;
  }

  /**
   * \brief get size recursively
   */
  static size_type _size(Node* n) {
    if (n == NULL) {
      return 0;
    }

    return 1 + _size(n->left_m) + _size(n->right_m);
  }

};

#endif
