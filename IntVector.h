#pragma once

#include <math.h>
#include <limits>
#include <memory>
#include <vector>
#include <algorithm>

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
  std::shared_ptr<std::vector<IntType>> powers;

public:


  IntVector(unsigned short max_elt_in) 
  {
    unsigned short capacity = calc_capacity(max_elt_in);
    if (capacity < 2) {
      throw MaxEltTooBigError();
    }
    powers = std::shared_ptr<std::vector<IntType>>(new std::vector<IntType>(capacity));
    powers->at(0) = max_elt_in;
    for (unsigned short i = 1; i < max_elt_in; ++i) {
      powers->at(i) = max_elt_in * powers->at(i - 1);
    }
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
      assign(i, input[i]);
    }
  }

  std::shared_ptr<std::vector<IntType>> get_powers() const { return powers; }

  unsigned short get(unsigned short i) const {
    return container % powers->at(i);
  }

  void assign(unsigned short i, unsigned short number) {
    IntType addition = number * (i == 0 ? 1 : powers->at(i - 1));
    container = container - get(i) + addition;
  }

  std::vector<unsigned short> vectorize() {
    std::vector<unsigned short> data(powers->size());
    for (unsigned short i = 0; i < data.size(); ++i) {
      data[i] = get(i);
    }
  }


private:

  static unsigned short log_with_base(double base, double x) {
    return (unsigned short)(log(x) / log(base));
  }

  static unsigned short calc_capacity(unsigned short max_elt) {
    return log_with_base(max_elt, std::numeric_limits<IntType>::max());
  }
};

