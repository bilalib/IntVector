#pragma once

#include <math.h>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>

struct MaxEltTooBigError {};

template<typename IntType>
class IntVector
{
private:

  IntType container;

  /*
    The data is stored in base max_elt, where max_elt is given to the constructor.
    powers stores all the powers of max_elt that fit in IntType, and powers[i] = max_elt^(i+1).
  */
  std::shared_ptr<const std::vector<IntType>> powers;

public:

  IntVector(unsigned short max_elt_in) 
  {
    unsigned short capacity = calc_capacity(max_elt_in);
    if (capacity < 2) {
      throw MaxEltTooBigError();
    }
    std::vector<IntType> powers_vec(capacity);
    powers_vec[0] = max_elt_in;
    for (unsigned short i = 1; i < max_elt_in; ++i) {
      powers_vec[i] = max_elt_in * powers_vec[i - 1];
    }
    powers = std::make_shared<const std::vector<IntType>>(powers_vec);
  }

  IntVector(std::shared_ptr<std::vector<IntType>> powers_in)
    : powers(powers_in)
  {}

  IntVector(const IntVector<IntType>& other) {
    *this = other;
  }

  IntVector(const std::vector<unsigned short>& input) 
    : IntVector(*std::max_element(input.begin(), input.end()))
  {
    for (unsigned short i = 0; i < input.size(); ++i) {
      assign_to_empty(i, input[i]);
    }
  }

  const std::shared_ptr<std::vector<IntType>> get_powers() const { return powers; }

  unsigned short get(unsigned short i) const {
    return (container / powers->at(i)) % powers->at(0);
  }

  void assign_to_empty(unsigned short i, unsigned short number) {
    container += number * (i == 0 ? 1 : powers->at(i - 1));
  }

  void assign(unsigned short i, unsigned short number) {
    container -= get(i);
    assign_to_empty(i, number);
  }

  std::vector<unsigned short> vectorize() const {
    std::vector<unsigned short> data(powers->size());
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

