#pragma once
#include "PowerTable.h"


/*
  Represents a row of PowerTable. Upon destruction, if this row is not referenced
  by any other IntVectors, and erase_unused_rows = true,
  the row is erased from the map in the main PowerTable (false by default).
*/
template<typename IntType>
class PowerList {

private:

  using TableType = std::unordered_map<unsigned short, typename PowerTable<IntType>::Row>;
  typename TableType::iterator it;
  static PowerTable<IntType> power_table;

public:


  PowerList(unsigned short base_in)
    : PowerList(power_table.find_or_build(base_in))
  {}


  PowerList(const PowerList& other)
    : PowerList(other.it)
  {}


  // Erases unreferenced rows as described above
  ~PowerList() {
    if (power_table.erase_unused_rows) {
      if (--it->second.count == 0) {
        power_table.erase(it);
      }
    }
  }


  void swap(PowerList& other) {
    std::swap(it, other.it);
  }


  PowerList& operator=(PowerList other) {
    std::swap(it, other.it);
    return *this;
  }


  /*
    To save space, base^0 and base^1 are not stored in the vector. This overloaded
    subscript operator adjusts the indices so that power_list[i] = base^i
  */
  IntType operator[](unsigned short exponent) const {
    if (exponent == 0) {
      return 1;
    }
    else if (exponent == 1) {
      return it->first;
    }
    // powers[i] = base^(i + 2)
    return it->second.powers[exponent - 2];
  }

  
  unsigned short greatest_exponent() const {
    return (unsigned short)(it->second.powers.size()) + 1;
  }


private:

  PowerList(typename TableType::iterator it_in)
    : it(it_in)
  {
    ++it->second.count;
  }

};


template<typename IntType>
PowerTable<IntType> PowerList<IntType>::power_table;