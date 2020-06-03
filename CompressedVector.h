#pragma once
#include "IntVector.h"
#include <vector>
#include <limits>

class CompressedVector
{

private:

  unsigned short max_elt;

  // just using unsigned long long for every vector
  using VectorType = std::vector<IntVector<unsigned long long>>;
  VectorType data;

  // 1 + index of last assigned element of last subvector
  unsigned short last_subvector_end;

  // size of every subvector in this vector
  const unsigned short subvector_capacity;


private:

  std::pair<size_t, unsigned short> convert_idx(unsigned long long idx) const;
  unsigned long long convert_idx(unsigned long long subvector, unsigned short const idx_in_subvector) const;
  unsigned long long convert_idx(std::pair<size_t, unsigned short> idx) const;

  template <typename T>
  unsigned long long calc_size(T size_in) const;

public:

  // below: constructors
  CompressedVector(unsigned short max_elt_in);

  template<typename It>
  CompressedVector(unsigned short max_elt_in, It input_begin, It input_end);

  template<typename T>
  CompressedVector(unsigned short max_elt_in, const std::vector<T>& input);

  template<typename T>
  CompressedVector(unsigned short max_elt_in, std::initializer_list<T> input);
  // above: constructors


  size_t size() const;

  void push_back(unsigned short input);

  template<typename T>
  IntVector<unsigned long long>::IdxReference operator[](T idx);

  template<typename T>
  IntVector<unsigned long long>::IdxReference at(T idx);

  IntVector<unsigned long long>::IdxReference back();
  void pop_back();

  template<typename T>
  void reserve(T new_size);

  template<typename T>
  void resize(T new_size);



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

    IntVector<unsigned long long>::IdxReference operator*();
    Iterator& operator++();
    Iterator& operator--();

    CompressedVector::Iterator operator+(size_t i);
    CompressedVector::Iterator operator-(size_t i);

    Iterator& operator+=(size_t i);
    Iterator& operator-=(size_t i);

    IntVector<unsigned long long>::IdxReference operator[](size_t i);

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

