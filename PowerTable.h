#pragma once
#include <unordered_map>
#include <vector>


struct BaseTooBigError {};

template<typename IntType>
class PowerTable
{

public:

  struct Row {
    std::vector<IntType> powers;
    unsigned count;
    Row(IntType size)
      : powers(size), count(0)
    {}
  };

private:

  using TableType = std::unordered_map<unsigned short, Row>;
  TableType table;
  bool erase_unused_rows;

public:

  /*
    A base is the number whose powers are going to be stored. 
    i.e. if base = 2 then the table will be {1, 2, 4, 8, 16, 32, ...}

    erase_unused_rows should be set to true if more than O(1) bases  be stored 
    in the table, or if any particular base will be used in at most O(1) IntVectors
  */
  PowerTable(bool erase_unused_rows_in)
    : erase_unused_rows(erase_unused_rows_in)
  {}


  PowerTable()
    : erase_unused_rows(false)
  {}


  typename TableType::iterator find_or_build(unsigned short base) {

    unsigned short capacity = calc_capacity(base);
    if (capacity < 2) {
      throw BaseTooBigError();
    }

    auto insertion = table.insert({ base, Row(capacity - 1) });

    // if the insertion was sucessful, base did not already exist in table,
    // so we fill the vector at table[base] with the powers of base.
    if (insertion.second) {
      auto& vec = insertion.first->second.powers;
      vec[0] = (IntType)base * base;

      // powers[i] = base^(i + 2). The last entry is powers[capacity - 2] = base^(capacity)
      // This saves memory because we already know i^0=1 and i^1 = i
      for (unsigned short i = 1; i < capacity - 1; ++i) {
        vec[i] = vec[i - 1] * base;
      }
    }

    return insertion.first;
  }


  static unsigned short log_base(unsigned short base, IntType x) {
    return (unsigned short)(log2(x) / log2(base));
  }


  static unsigned short calc_capacity(unsigned short max_elt) {
    return log_base(max_elt, std::numeric_limits<IntType>::max());
  }


  /*
    Represents a row of PowerTable. Upon destruction, if this row is not referenced
    by any other IntVectors, and erase_unused_rows = true,
    the row is erased from the map in the main PowerTable (false by default).
  */
  class PowerList {

  private:

    static PowerTable<IntType>* power_table;
    typename TableType::iterator it;

  public:


    PowerList(unsigned short base_in) 
      : PowerList(power_table->find_or_build(base_in))
    {}


    PowerList(const PowerList& other)
      : PowerList(other.it)
    {}


    // Erases unreferenced rows as described above
    ~PowerList() {
      if (power_table->erase_unused_rows) {
        if (--it->second.count == 0) {
          power_table->table.erase(it);
        }
      }
    }


    PowerList& operator=(const PowerList& other) {
      it = other.it;
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


    unsigned short capacity() const {
      return (unsigned short)it->second.powers.size() + 1;
    }


    void swap(PowerList other) {
      std::swap(it, other.it);
    }


  private:

    PowerList(typename TableType::iterator it_in)
      : it(it_in)
    {
      ++it->second.count;
    }

  };
};

