#pragma once

#include <math.h>
#include <limits>
#include <memory>

class SizeTooBigError {};

template<typename IntType>
class IntVector
{
private:

  IntType container;
  std::shared_ptr<IntType[]> powers;

public:

  static unsigned capacity(unsigned short size) {
    return log_with_base(size, std::numeric_limits<IntType>::max());
  }

  IntVector(unsigned short size_in) 
  {
    unsigned curr_capacity = capacity(size_in);
    if (curr_capacity < 2) {
      throw SizeTooBigError();
    }
    powers = std::unique_ptr<IntType[]>(new IntType[curr_capacity]);
    powers[0] = size_in;
    for (unsigned short i = 1; i < size_in; ++i) {
      powers[i] = size_in * powers[i - 1];
    }
  }

  IntVector(const std::shared_ptr<IntType[]> const powers_in)
    : powers(powers_in)
  {}

  IntVector(const IntVector<IntType>& other)
    : IntVector(other.powers)
  {
    container = other.container;
  }

  const std::shared_ptr<IntType[]> const get_powers() {
    return powers;
  }

private:

  static int log_with_base(double base, double x) {
    return (int)(log(x) / log(base));
  }


};

