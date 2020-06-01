#include <iostream>
#include <cassert>
#include <math.h>
#include <limits>
#include <string>
#include <iostream>
#include <sstream>

#include "IntVector.h"

using namespace std;

// utility functions for printing test info
template<typename IntType>
void print_list_helper(ostream& os, IntType last) {
  os << last << endl;
}

template<typename First, typename... Rest>
void print_list_helper(ostream& os, const First& first, const Rest&... rest) {
  os << first << ", ";
  print_list_helper(os, rest...);
}

template<typename... List>
const string print_list(const List&... list) {
  stringstream buffer;
  print_list_helper(buffer, list...);
  return buffer.str();
}

// pt = PowerTable

void pt_find_or_build() {

  PowerTable<unsigned> table(false);

  for (unsigned short base = 2; base < 20; ++base) {
    PowerList<unsigned> list(base);
    for (unsigned short exp = 2; exp < list.greatest_exponent(); ++exp) {
      unsigned actual = (unsigned)pow(base, exp);
      if (actual != list[exp]) {
        cout << "base, exp, calculated, actual" << print_list(base, exp, list[exp], actual);
      }
      assert(actual == list[exp]);
    }
  }
}

void test_power_table() {

  pt_find_or_build();

}


// iv = IntVector


void iv_constructors_compile() {

  IntVector<unsigned> a(10);
  vector<unsigned short> input({1, 2, 3, 4, 5});
  IntVector<unsigned> b(12, input);
  IntVector<unsigned> c(b);

  IntVector<unsigned> d(10);
  d = a;

}

void iv_slice() {

  // in base 10
  unsigned long long a = 9876543210;
  IntVector<unsigned long long> b(10, a);
  assert(b.slice(0, 11) == a);
  assert(b.slice(0, 1) == 0);
  assert(b.slice(3, 7) == 6543);

  // in base 15, {6, 13, 14, 0, 11, 6}
  //              0   1   2  3  4   5
  unsigned long long c = 6 + 13 * pow(15, 1) + 14 * pow(15, 2) + 0 * pow(15, 3) + 11 * pow(15, 4) + 6 * pow(15, 5);
  IntVector<unsigned long long> d(15, c);
  assert(d.slice(0, 1) == 6);
  assert(d.slice(1, 4) == 13 + 14 * pow(15, 1));
  assert(d.slice(2, 5) == (14 * pow(15, 2) + 0 * pow(15, 3) + 11 * pow(15, 4)) / pow(15, 2));

}

void iv_get() {

  vector<unsigned short> values = { 6, 13, 14, 0, 11, 6, 1 };
  IntVector<unsigned long long> b(15, values);

  for (unsigned i = 0; i < 7; ++i) {
    bool good = (b.get(i) == b[i] && b[i] == values[i]);
    assert(good);
  }


}

void iv_assign_to_empty() {

  IntVector<unsigned long long> a(10);
  for (unsigned i = 0; i < 10; ++i) {
    a.assign_to_empty(i, i);
  }
  assert(a == 9876543210);
}

void iv_assign() {

  vector<unsigned short> values = { 6, 13, 14, 0, 11, 6, 1 };
  IntVector<unsigned long long> b(15, values);
  IntVector<unsigned long long> c(b);

  for (unsigned i = 0; i < 7; ++i) {
    values[i] = (i * i * (values[i] + values[(i + 1) % 7])) % 15;
    unsigned short new_value = c[i] = values[i];
    b.assign(i, new_value);
  }

  IntVector<unsigned long long> d(15, values);

  assert(b == c && c == d);

}


void assert_equal_in_range(unsigned short start, unsigned short end, 
                    const vector<unsigned short>& v, const vector<unsigned short>& w) {
  for (; start < end; ++start) {
    assert(v[start] == w[start]);
  }
}

void iv_vectorize() {

  IntVector<unsigned long long> a(10, 9876543210);

  vector<unsigned short> v1({ 0,1,2,3,4,5,6,7,8,9 });
  assert_equal_in_range(0, v1.size(), a.vectorize(), v1);

  vector<unsigned short> v2({ 4, 5, 6, 7, 8, 9 });
  assert_equal_in_range(0, v2.size(), a.vectorize(4, 10), v2);

  vector<unsigned short> v3({ 4, 9, 3, 2, 2, 7 });
  assert_equal_in_range(0, v3.size(), a.vectorize(v3), v3);

}

void iv_subscript_operator() {

  IntVector<unsigned long long> a(25, { 0, 13, 19, 9, 22, 3, 4 });
  a[3] = 10;
  auto x = a[6] = a[4] = 9;
  assert(x == 9);
  assert(a == IntVector<unsigned long long>(25, { 0, 13, 19, 10, 9, 3, 9 }));

}


void test_int_vector() {

  iv_constructors_compile();
  iv_slice();
  iv_get();
  iv_assign_to_empty();
  iv_assign();
  iv_vectorize();
  iv_subscript_operator();

}




int main() {

  test_power_table();
  test_int_vector();

  return 0;
}