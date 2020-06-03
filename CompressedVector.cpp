#include "CompressedVector.h"
using namespace std;
using CvIt = CompressedVector::Iterator;

// below: index conversion functions
pair<size_t, unsigned short> CompressedVector::convert_idx(unsigned long long idx) const {
  return { idx / subvector_capacity, idx % subvector_capacity };
}

unsigned long long CompressedVector::convert_idx(unsigned long long subvector, 
  unsigned short const idx_in_subvector) const {
  return subvector_capacity * subvector + idx_in_subvector;
}

unsigned long long CompressedVector::convert_idx(pair<size_t, unsigned short> idx) const {
  return convert_idx(idx.first, idx.second);
}
// above: index conversion functions


// below: iterator
CompressedVector::Iterator::Iterator(CompressedVector* vec_in, 
  VectorType::iterator it_in, unsigned short subvector_idx_in)
  : vec(vec_in), it(it_in), subvector_idx(subvector_idx_in)
{}

CompressedVector::Iterator::Iterator(const CompressedVector::Iterator& other)
  : vec(other.vec), it(other.it), subvector_idx(other.subvector_idx)
{}

IntVector<unsigned long long>::IdxReference CompressedVector::Iterator::operator*() {
  return (*it)[subvector_idx];
}

CompressedVector::Iterator& CompressedVector::Iterator::operator++() {
  ++subvector_idx;
  if (subvector_idx == vec->subvector_capacity) {
    subvector_idx = 0;
    ++it;
  }
  return *this;
}

CompressedVector::Iterator& CompressedVector::Iterator::operator--() {
  if (subvector_idx == 0) {
    subvector_idx = vec->subvector_capacity - 1;
    --it;
  }
  else {
    --subvector_idx;
  }
  return *this;
}

CompressedVector::Iterator CompressedVector::Iterator::operator+(size_t i) {
  size_t increased = subvector_idx + i;
  auto new_it = it + increased / vec->subvector_capacity;
  unsigned short new_subvector_idx = increased % vec->subvector_capacity;
  return Iterator(vec, new_it, new_subvector_idx);
}

CompressedVector::Iterator CompressedVector::Iterator::operator-(size_t i) {
  size_t decreased = subvector_idx - i;
  auto new_it = it + decreased / vec->subvector_capacity;
  unsigned short new_subvector_idx = vec->subvector_capacity + decreased % vec->subvector_capacity;
  return Iterator(vec, new_it, new_subvector_idx);
}

CompressedVector::Iterator& CompressedVector::Iterator::operator+=(size_t i) {
  return *this = *this + i;
}

CompressedVector::Iterator& CompressedVector::Iterator::operator-=(size_t i) {
  return *this = *this - i;
}

IntVector<unsigned long long>::IdxReference CompressedVector::Iterator::operator[](size_t i) {
  auto idx = vec->convert_idx(i);
  return IntVector<unsigned long long>::IdxReference(vec->data[idx.first], idx.second);
}

void CompressedVector::Iterator::swap(Iterator& other) {
  std::swap(vec, other.vec);
  std::swap(it, other.it);
  std::swap(subvector_idx, other.subvector_idx);
}

CompressedVector::Iterator& CompressedVector::Iterator::operator=(CompressedVector::Iterator other) {
  swap(other);
  return *this;
}

bool CompressedVector::Iterator::operator==(const Iterator& other) const {
  return it == other.it && subvector_idx == other.subvector_idx;
}

bool CompressedVector::Iterator::operator!=(const Iterator& other) const {
  return !(*this == other);
}

bool CompressedVector::Iterator::operator<(const Iterator& other) const {
  return it < other.it && subvector_idx < other.subvector_idx;
}

bool CompressedVector::Iterator::operator>(const Iterator& other) const {
  return it > other.it && subvector_idx > other.subvector_idx;
}

bool CompressedVector::Iterator::operator<=(const Iterator& other) const {
  return !(*this > other);
}

bool CompressedVector::Iterator::operator>=(const Iterator& other) const {
  return !(*this < other);
}
// above: iterator



// below: constructors
CompressedVector::CompressedVector(unsigned short max_elt_in)
  : max_elt(max_elt_in), last_subvector_end(0), subvector_capacity(IntVector<unsigned long long>::size(max_elt))
{}

template<typename It>
CompressedVector::CompressedVector(unsigned short max_elt_in, It input_begin, It input_end)
  : CompressedVector(max_elt_in)
{
  reserve(input_end - input_begin);
  for (; input_begin != input_end; ++input_begin) {
    push_back(*input_begin);
  }
}

template<typename T>
CompressedVector::CompressedVector(unsigned short max_elt_in, const std::vector<T>& input)
  : CompressedVector(max_elt_in, input.begin(), input.end())
{}

template<typename T>
CompressedVector::CompressedVector(unsigned short max_elt_in, initializer_list<T> input)
  : CompressedVector(max_elt_in, input.begin(), input.end())
{}
// above: constructors

CompressedVector::Iterator CompressedVector::begin() {
  return Iterator(this, this->begin, 0);
}

CompressedVector::Iterator CompressedVector::end() {
  return Iterator(this, this->end, 0);
}

size_t CompressedVector::size() const {
  return subvector_capacity * data.size() + last_subvector_end;
}

template<typename T>
IntVector<unsigned long long>::IdxReference CompressedVector::operator[](T idx) {
  return begin()[idx];
}

void CompressedVector::push_back(unsigned short input) {
  if (last_subvector_end == subvector_capacity) {
    data.emplace_back(input);
    last_subvector_end = 1;
  }
  else {
    data.back().assign(last_subvector_end, input);
    ++last_subvector_end;
  }
}

void CompressedVector::pop_back() {
  if (last_subvector_end == 1) {
    data.pop_back();
    last_subvector_end = (data.size() == 0 ? 0 : subvector_capacity);
  }
  else {
    data.back().assign(last_subvector_end - 1, 0);
    --last_subvector_end;
  }
}

template<typename T>
unsigned long long CompressedVector::calc_size(T size_in) const {
  auto breakdown = convert_idx(new_size);
  if (breakdown.second != 0) {
    ++breakdown.first;
  }
  return breakdown.first;
}

template<typename T>
void CompressedVector::reserve(T new_size) {
  data.reserve(calc_size(new_size);
}

template<typename T>
void CompressedVector::resize(T new_size) {
  data.resize(calc_size(new_size));
}