#include "CompressedVector.h"

using namespace std;
using IntType = unsigned long long;
using Iv = IntVector<IntType>;
using VectorType = std::vector<Iv>;

size_t CompressedVector::convert_idx(size_t subvector, 
  unsigned short idx_in_subvector) const {
  if (subvector == 0) {
    return idx_in_subvector;
  }
  return subvector_capacity * (subvector - 1) + idx_in_subvector;
}

size_t CompressedVector::convert_idx(pair<size_t, unsigned short> idx) const {
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

Iv::IdxReference CompressedVector::Iterator::operator*() {
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
  unsigned short new_subvector_idx = decreased % vec->subvector_capacity;
  if (new_subvector_idx < 0) {
    new_subvector_idx += vec->subvector_capacity;
    --new_it;
  }
  return Iterator(vec, new_it, new_subvector_idx);
}

CompressedVector::Iterator& CompressedVector::Iterator::operator+=(size_t i) {
  return *this = *this + i;
}

CompressedVector::Iterator& CompressedVector::Iterator::operator-=(size_t i) {
  return *this = *this - i;
}

Iv::IdxReference CompressedVector::Iterator::operator[](size_t i) {
  auto idx = div(i, vec->subvector_capacity);
  return Iv::IdxReference(vec->data[idx.quot], idx.rem);
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
  return it < other.it || (it == other.it && subvector_idx < other.subvector_idx);
}

bool CompressedVector::Iterator::operator>(const Iterator& other) const {
  return it > other.it || (it == other.it && subvector_idx > other.subvector_idx);
}

bool CompressedVector::Iterator::operator<=(const Iterator& other) const {
  return !(*this > other);
}

bool CompressedVector::Iterator::operator>=(const Iterator& other) const {
  return !(*this < other);
}
// above: iterator


CompressedVector::Iterator CompressedVector::begin() {
  return Iterator(this, data.begin(), 0);
}

CompressedVector::Iterator CompressedVector::end() {
  return Iterator(this, data.end(), 0);
}

size_t CompressedVector::size() const {
  return convert_idx(data.size(), last_subvector_end);
}

void CompressedVector::push_back(unsigned short input) {
  if (last_subvector_end == subvector_capacity || data.empty()) {
    data.emplace_back(max_elt, input);
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
    --last_subvector_end;
  }
}
Iv::IdxReference CompressedVector::back() {
  return data.back()[last_subvector_end - 1];
}

Iv::IdxReference CompressedVector::at(size_t subvector, unsigned short idx_in_subvector) {
  return *Iterator(this, data.begin() + subvector, idx_in_subvector);
}

bool CompressedVector::operator==(const CompressedVector& other) const {
  size_t vector_size = size();
  if (vector_size != other.size()) {
    return false;
  }
  for (size_t i = 0; i < vector_size; ++i) {
    if (at(i) != other.at(i)) {
      return false;
    }
  }
  return true;
}

