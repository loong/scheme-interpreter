#ifndef HASHTABLEMAP_HPP
#define HASHTABLEMAP_HPP

#define NO_BUCKETS 389
#define RADIX 64

#include "bstmap.hpp"

#include <iterator>
#include <sstream>

#include <cassert>

using namespace std;

/**
 * \class hashtablemap
 *
 * \brief hashtablemap is a map container template, which implements a
 *        hash table. It follows STL idiom for easy plug and play.
 *        _Note_: As opposed to std::map, this container does throw
 *        runtime_error
 */
template <class Key, class T>
class hashtablemap
{
  typedef hashtablemap<Key, T>     Self;

public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef T                  mapped_type;
  typedef pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;

private:
  /**
   * \struct Node
   *
   * \brief data structure which holds the data
   */
  struct Node {
  public:
    Node(value_type val) : value_m(val) {}
    value_type value_m;
  };

  typedef bstmap<Key, Node*> bucket_type;

  bucket_type* buckets_m;
  int size_m;


  ////////////////////////////////////////////////////////////////////////////////
  /// Iterators
  ////////////////////////////////////////////////////////////////////////////////
public:
  /**
   * \brief Template in order to create iterator and const_iterator
   *        without copy pasting. Implements a forward_iterator
   */
  template <typename _T>
  class _iterator {
    typedef input_iterator_tag iterator_category;
    typedef _T value_type;
    typedef int difference_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    friend class hashtablemap;
        
  public:
    _iterator(Node* n = NULL, const hashtablemap* ht = NULL) 
      : node_m(n), table_m(ht) {}
    
    _iterator(const _iterator& x) 
      : node_m(x.node_m), table_m(x.table_m) {}

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
      node_m = table_m->_get_next(node_m);
      return *this;
    }

    _iterator operator++(int) {
      _iterator temp = *this;
      ++(*this);
      return temp;
    }

    Node* node_m;
    const hashtablemap* table_m;
  };

  typedef _iterator<value_type> iterator;
  typedef _iterator<const value_type> const_iterator;


  ////////////////////////////////////////////////////////////////////////////////
  /// Constructors, Destructor, Assignment
  ////////////////////////////////////////////////////////////////////////////////

public:
  hashtablemap() : buckets_m(new bucket_type[NO_BUCKETS]), size_m(0) {}
  
  /**
   * \brief overloads copy constructor for deep copy
   */
  hashtablemap(const Self& x) : buckets_m(new bucket_type[NO_BUCKETS]), size_m(0) {
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
    /// guard against self assignment
    if (this == &x) {
      return *this;
    }

    clear();
    
    for (const_iterator i = x.begin(); i != x.end(); ++i) {
      insert(*i);      /// same balancing problem as for copy constructor above
    }
  }

  /**
   * \brief free dynamic allocated members
   */
  ~hashtablemap() {
    delete[] buckets_m;
  }

  
  ////////////////////////////////////////////////////////////////////////////////
  /// Accessors
  ////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief subscript operator. Remarks: if Key not available, Key
   *        with default value will be initialised
   */
  T& operator[](const Key& k) {
    iterator it = find(k);
    
    if (it != end()) {            // found key
      return (*it).second;
    }
    else {                        // not found -> create empty
      insert(value_type(k, T()));
      return operator[](k);       // a bit ugly
    }
  }

  /** 
   * \return iterator keypair in first non-empty bucket
   */
  iterator begin() {
    if (empty()) {
      return end();
    }

    // finds the first node in the first non-empty bucket
    const bucket_type* first_filled_bucket = _find_next_nonempty_bucket(0);
    assert(first_filled_bucket != NULL); // logic error if this occurs

    Node* first_node = first_filled_bucket->begin()->second;
    
    return iterator(first_node, this);
  }

  /** 
   * \return const_iterator keypair in first non-empty bucket
   * \todo use template!
   */  
  const_iterator begin() const {
    if (empty()) {
      return end();
    }

    // finds the first node in the first non-empty bucket
    const bucket_type* first_filled_bucket = _find_next_nonempty_bucket(0);
    assert(first_filled_bucket != NULL); // logic error if this occurs

    Node* first_node = first_filled_bucket->begin()->second;
    
    return const_iterator(first_node, this);
  }

  /**
   * \return iterator "past-the-end" element
   */
  iterator end() {
    return iterator(NULL, this);
  }

  /**
   * \return const+iterator "past-the-end" element
   */
  const_iterator end() const {
    return const_iterator(NULL, this);
  }
  
  bool empty() const {
    return (size_m == 0);
  }

  size_type size() const {
    return size_m; // hashtablemap is keeping size_m up to date when
		   // inserting or erasing (don't forget clear())
		   // elements
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Insert and Erase
  ////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief seperate chaining approach for insertion in order to
   *        prevent primary clustering
   *
   * \return pair<iterator, bool> iterator to node in tree, bool is
   *         true if an insertion has been done
   */
  pair<iterator, bool> insert(const value_type& x) {
    iterator it = find(x.first);

    // element exists already
    if (it != end()) {
      return pair<iterator, bool>(iterator(it.node_m, this), false);
    }

    // insert new node
    int hash = _hash(x.first);
    Node* new_node = new Node(x);
      
    const pair<Key, Node*> keypair(x.first, new_node);
    buckets_m[hash].insert(keypair);

    ++size_m;

    return pair<iterator, bool>(iterator(new_node, this), true);
  }

  /**
   * \brief erase by iterator
   */
  void erase(iterator pos) throw (runtime_error) {
    if (pos == NULL) {
      throw runtime_error("Cannot erase Null iterator");
    }

    erase(pos->first);
  }

  /**
   * \brief erase by Key value 
   * 
   * \return size_type number of elements erased (in this case can
   *         only be 1 or 0)
   */
  size_type erase(const Key& x) {
    iterator it = find(x);
    
    if (it == end()) { // Key not found
      return 0;
    }
    
    int hash = _hash(x);
    assert(buckets_m[hash].find(x) != buckets_m[hash].end());

    buckets_m[hash].erase(x);
    --size_m;
    
    return 1; // since Key in maps are unique, can only be 1
  }

  /**
   * \brief Empty all buckets
   * 
   * Does not uses iterator in order to save the "searching for
   * non-empty buckets" step.
   */
  void clear() {
    if (empty()) {
      return;
    }

    delete[] buckets_m;
    buckets_m = new bucket_type[NO_BUCKETS];

    size_m = 0;
  }


  ////////////////////////////////////////////////////////////////////////////////
  /// Find and count
  ////////////////////////////////////////////////////////////////////////////////

  iterator find(const Key& x) {
    int hash = _hash(x);
    
    bucket_type* b = &buckets_m[hash];   // Use pointer to prevent copying
    typename bucket_type::iterator it = b->find(x);

    if (it == b->end()) {
      return end();
    }

    return iterator((*it).second, this);
  }

  /// \todo use template
  const_iterator find(const Key& x) const {
    int hash = _hash(x);
    
    bucket_type* b = &buckets_m[hash];   // Use pointer to prevent copying whole map
    typename bucket_type::iterator it = b->find(x);

    if (it == b->end()) {
      return end();
    }

    return const_iterator((*it).second, this);
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

  ////////////////////////////////////////////////////////////////////////////////
  /// Helpers
  ////////////////////////////////////////////////////////////////////////////////
private:
  
  /**
   * \brief Hash function uses simple modulus approach. Additionally
   *        radix is used to further randomize the bucket location a
   *        bit more
  */
  int _hash(Key k) const {
    // lazy stringstream method
    // \todo use bytewise hashing rather than characterwise
    stringstream ss; ss << k;

    const char* str = ss.str().c_str(); // just use cstring so we
					// don't need to include
					// strlen
    int hash = 0;

    for (int i = 0; str[i] != 0; ++i) {
      // takes modulus in every step to prevent loosing precision due
      // to too big numbers
      hash = (hash + (( (int) str[i] * RADIX^i ) % NO_BUCKETS) ) % NO_BUCKETS;
    }

    return hash;
  }

  /**
   * returns a constant type to add security (there should be no
   * need to change the content of the bucket)
   */
  const bucket_type* _find_next_nonempty_bucket(int const curr_index) const {
    for (int i = curr_index + 1; i < NO_BUCKETS; ++i) {
      if (!buckets_m[i].empty()) {
	return &buckets_m[i];
      }
    }

    return NULL;
  }

  /**
   * \brief Used for iteration.
   * \return Node* returns NULL if there are no following Nodes
   */
  Node* _get_next(Node* n) const {    
    Key k = n->value_m.first;
    int hash = _hash(k);

    /// \todo memory is wasted here
    bucket_type curr_bucket = buckets_m[hash]; 
    typename bucket_type::iterator next = ++(curr_bucket.find(k));

    // is there an element in current bucket?
    if (next != curr_bucket.end()) {
      return next->second;
    }

    const bucket_type* next_bucket = _find_next_nonempty_bucket(hash);
      
    // is this the last element?
    if (next_bucket == NULL) {
      return NULL;  /// similarly, end()
    }

    // return first node of next non-empty bucket
    return next_bucket->begin()->second;
  }

};

#endif
