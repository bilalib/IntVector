#pragma once
#include "IntVector.h"
#include <vector>
#include <limits>

class CompressedVector
{

private:

  unsigned short max_elt;

  // just using unsigned long long for every vector
  using IntType = unsigned long long;
  using Iv = IntVector<IntType>;
  using VectorType = std::vector<Iv>;

  VectorType data;

  // 1 + index of last assigned element of last subvector
  unsigned short last_subvector_end;

  // size of every subvector in this vector
  const unsigned short subvector_capacity;


private:

  size_t convert_idx(size_t subvector, unsigned short idx_in_subvector) const;
  size_t convert_idx(std::pair<size_t, unsigned short> idx) const;


  template<typename T>
  size_t calc_size(T size_in) const {
    auto breakdown = div(size_in, subvector_capacity);
    if (breakdown.rem != 0) {
      ++breakdown.quot;
    }
    return breakdown.quot;
  }

public:

  // below: constructors
  CompressedVector(unsigned short max_elt_in)
    : max_elt(max_elt_in), last_subvector_end(0), subvector_capacity(Iv::size(max_elt))
  {}

  template<typename It>
  CompressedVector(unsigned short max_elt_in, It input_begin, It input_end)
    : CompressedVector(max_elt_in)
  {
    reserve(input_end - input_begin);
    for (; input_begin != input_end; ++input_begin) {
      push_back(*input_begin);
    }
  }

  template<typename T>
  CompressedVector(unsigned short max_elt_in, const std::vector<T>& input)
    : CompressedVector(max_elt_in, input.begin(), input.end())
  {}


  template<typename T>
  CompressedVector(unsigned short max_elt_in, std::initializer_list<T> input)
    : CompressedVector(max_elt_in, input.begin(), input.end())
  {}

  // above: constructors


  size_t size() const;
  void push_back(unsigned short input);
  Iv::IdxReference back();
  void pop_back();
  bool operator==(const CompressedVector& other) const;
  Iv::IdxReference at(size_t subvector, unsigned short idx_in_subvector);


  template<typename T>
  Iv::IdxReference operator[](T idx) {
    return begin()[idx];
  }

  template<typename T>
  Iv::IdxReference at(T idx) {
    return data[idx];
  }

  template<typename T>
  Iv::IdxReference at(T idx) const {
    return data[idx];
  }

  template<typename T>
  void reserve(T new_size) {
    data.reserve(calc_size(new_size));
  }

  template<typename T>
  void resize(T new_size) {
    data.resize(calc_size(new_size));
  }



  class Iterator {

  private:

    CompressedVector* vec;
    VectorType::iterator it;
    unsigned short subvector_idx;

  public:
    
    Iterator(CompressedVector* vec_in, VectorType::iterator it_in, unsigned short subvector_idx_in);
    Iterator(const Iterator& other);

    void swap(Iterator& other);
    Iterator& operator=(Iterator other);

    Iv::IdxReference operator*();
    Iterator& operator++();
    Iterator& operator--();

    CompressedVector::Iterator operator+(size_t i);
    CompressedVector::Iterator operator-(size_t i);

    Iterator& operator+=(size_t i);
    Iterator& operator-=(size_t i);

    Iv::IdxReference operator[](size_t i);

    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
    bool operator<(const Iterator& other) const;
    bool operator>(const Iterator& other) const;
    bool operator<=(const Iterator& other) const;
    bool operator>=(const Iterator& other) const;

  };

  Iterator begin();
  Iterator end();

};

