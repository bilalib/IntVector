#pragma once

#include <math.h>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

#include "PowerTable.h"


template<typename IntType>
class IntVector
{
private:

  static PowerTable<IntType> table;

  IntType container;
  typename PowerTable<IntType>::PowerListHolder powers;

public:

  IntVector(unsigned short max_elt_in) 
    : powers(max_elt_in, table)
  {}

  IntVector(const IntVector<IntType>& other)
    : container(other.container), powers(other.powers)
  {}

  IntVector(const std::vector<unsigned short> & input, unsigned short max_elt)
    : IntVector(max_elt)
  {
    for (unsigned short i = 0; i < input.size(); ++i) {
      assign_to_empty(i, input[i]);
    }
  }

  IntVector(const std::vector<unsigned short>& input) 
    : IntVector(input, *std::max_element(input.begin(), input.end()))
  {}

  unsigned short get(unsigned short i) const {
    return (container % powers[i + 1]) / powers[i];
  }

  void assign_to_empty(unsigned short i, unsigned short number) {
    container += number * powers[i];
  }

  void assign(unsigned short i, unsigned short number) {
    container -= container % powers[i + 1] - container % powers[i];
    assign_to_empty(i, number);
  }

  std::vector<unsigned short> vectorize() const {
    std::vector<unsigned short> data(powers.capacity());
    for (unsigned short i = 0; i < data.size(); ++i) {
      data[i] = get(i);
    }
  }

  void print() const {
    std::vector<unsigned short> data = vectorize();
    for (auto i : data) {
      std::cout << i << ' ';
    }
    std::cout << std::endl;
  }


private:

  static unsigned short int_log2(unsigned long long x) {
    unsigned short result = 0;
    while (x >>= 1) {
      ++result;
    }
    return result;
  }

  static unsigned short log_base(unsigned short base, IntType x) {
    return int_log2(x) / int_log2(base);
  }

  static unsigned short calc_capacity(unsigned short max_elt) {
    return log_base(max_elt, std::numeric_limits<IntType>::max());
  }
};

// intializing the static member
template<typename IntType>
PowerTable<IntType> IntVector<IntType>::table = PowerTable<IntType>();