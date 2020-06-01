#pragma once

#include <math.h>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "PowerTable.h"
#include "PowerList.h"


template<typename IntType>
class IntVector
{

private:

  PowerList<IntType> powers;
  IntType container;

  
public:


  IntVector(unsigned short max_elt, IntType container_in)
    : powers(max_elt), container(container_in)
  {}

  IntVector(unsigned short max_elt)
    : IntVector(max_elt, 0)
  {}
  
  IntVector(unsigned short max_elt, const std::vector<unsigned short>& input)
    : IntVector(max_elt)
  {
    for (unsigned short i = 0; i < input.size(); ++i) {
      assign_to_empty(i, input[i]);
    }
  }

  IntVector(const IntVector<IntType>& other)
    : powers(other.powers), container(other.container)
  {}


  void swap(IntVector& other) {
    std::swap(container, other.container);
    powers.swap(other.powers);
  }


  IntVector& operator=(IntVector other) {
    swap(other);
    return *this;
  }


  // size is the number of elements in an IntVector with an IntType container
  unsigned short size() const { return powers.greatest_exponent(); }


  static unsigned short size(unsigned short max_elt) {
    return PowerTable<IntType>::calc_greatest_exponent(max_elt);
  }


  IntType get_container() const { return container; }


  /*
    This allows an IntVector to be treated as an IntType for comparisons, arithmetic,
    hashing, etc. 
    Note that comparisons between IntVectors with different max elements are not defined.
  */
  operator IntType() const { return container; }


  // returns an IntType representing the subvector in the range [start, end)
  IntType slice(unsigned short start, unsigned short end) const {
    IntType result = container;
    if (end <= powers.greatest_exponent()) {
      result %= powers[end];
    }
    return result / powers[start];
  }


  unsigned short get(unsigned short i) const {
    return slice(i, i + 1);
  }


  // Assigns number to index i, with the assumption that the current value at i is 0.
  void assign_to_empty(unsigned short i, unsigned short number) {
    container += number * powers[i];
  }


  // Assigns number to index i
  void assign(unsigned short i, unsigned short number) {
    container -= slice(0, i + 1) - slice(0, i);
    assign_to_empty(i, number);
  }


  template <typename T>
  bool operator==(const std::vector<T>& vec) const {
    return *this == IntVector<IntType>(powers[1], vec);
  }


  // Returns a vector of values in the range [start, end).
  // More efficient than the version that takes an arbitrary set of indices
  std::vector<unsigned short> vectorize(unsigned short start, unsigned short end) const {

    std::vector<unsigned short> data(end - start);
    IntType result = container;
    if (end <= powers.greatest_exponent()) {
      result %= powers[end];
    }

    for (int i = data.size() - 1; i >= 0; --i) {
      --end;
      IntType magnitude = powers[end];
      data[i] = result / magnitude;
      result %= magnitude;
    }

    return data;
  }


  // returns a vector of all values
  std::vector<unsigned short> vectorize() const {
    return vectorize(0, powers.greatest_exponent());
  }


  // Returns vector of values at indicies specified by vector of indices
  std::vector<unsigned short> vectorize(const std::vector<unsigned short>& indices) const {
    std::vector<unsigned short> data;
    data.reserve(indices.size());
    for (unsigned short i : indices) {
      data.push_back(get(i));
    }
    return data;
  }


  void print() const {
    std::vector<unsigned short> data = vectorize();
    for (unsigned short i : data) {
      std::cout << i << ' ';
    }
    std::cout << std::endl;
  }


  /*
    The IdxReference class allows overloading the assignment (=) operator.
     It can't be overloaded normally because we can't return a reference 
     to part of an integer
  */
  class IdxReference {
  private:
    IntVector& int_vector;
    unsigned short idx;

  public:
    IdxReference(IntVector& int_vector_in, unsigned short idx_in)
      : int_vector(int_vector_in), idx(idx_in)
    {}

    operator unsigned short() const { return int_vector.get(idx); }

    unsigned short operator=(unsigned short new_value) {
      int_vector.assign(idx, new_value);
      return *this;
    }
  };


  IdxReference operator[](unsigned short idx) {
    return IdxReference(*this, idx);
  }


  unsigned short operator[](unsigned short idx) const {
    return get(idx);
  }


};