#include <iostream>
#include <cassert>
#include <math.h>
#include <limits>
#include <string>
#include <iostream>

#include "IntVector.h"

using namespace std;

template<typename IntType>
void print_list(IntType last) {
  cout << last << endl;
}

template<typename First, typename... Rest>
void print_list(const First& first, const Rest&... rest) {
  cout << first << ", ";
  print_list(rest...);
}

// pt = PowerTable

void pt_find_or_build() {

  PowerTable<unsigned> table;

  cout << "base, exp, calculated, actual" << endl;
  for (unsigned short base = 2; base < 20; ++base) {
    PowerTable<unsigned>::PowerListHolder list(base, table);
    for (unsigned short exp = 2; exp < list.capacity(); ++exp) {
      unsigned actual = (unsigned)pow(base, exp);
      if (actual != list[exp]) {
        print_list(base, exp, list[exp], actual);
      }
      assert(actual == list[exp]);
    }
  }
}


void test_power_table() {

  pt_find_or_build();

}


int main() {

  test_power_table();

  return 0;
}