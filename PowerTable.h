#pragma once
#include <unordered_map>
#include <vector>


struct BaseTooBigError {};

template<typename IntType>
class PowerList;


template<typename IntType>
class PowerTable
{

  friend class PowerList<IntType>;

protected:

  struct Row {
    std::vector<IntType> powers;
    unsigned count;
    Row(IntType size)
      : powers(size), count(0)
    {}
  };

  using TableType = std::unordered_map<unsigned short, Row>;

  const bool erase_unused_rows;

  void erase(typename TableType::iterator it) {
    table.erase(it);
  }


private:

  TableType table;


public:

  /*
    A base is the number whose powers are going to be stored. 
    i.e. if base = 2 then the table will be {1, 2, 4, 8, 16, 32, ...}

    erase_unused_rows should be set to true if more than O(1) different bases
    or max elements in IntVectors are used throughout the program
  */
  PowerTable(bool erase_unused_rows_in)
    : erase_unused_rows(erase_unused_rows_in)
  {}


  PowerTable()
    : erase_unused_rows(false)
  {}


  typename TableType::iterator find_or_build(unsigned short base) {

    unsigned short greatest_exponent = calc_greatest_exponent(base);
    if (greatest_exponent < 2) {
      throw BaseTooBigError();
    }

    auto insertion = table.try_emplace(base, greatest_exponent - 1);

    // if the insertion was sucessful, base did not already exist in table,
    // so we fill the vector at table[base] with the powers of base.
    if (insertion.second) {
      auto& vec = insertion.first->second.powers;
      vec[0] = (IntType)base * base;
      
      // powers[i] = base^(i + 2). The last entry is powers[greatest_exponent - 2] = base^(greatest_exponent)
      // This saves memory because we already know i^0=1 and i^1 = i
      for (unsigned short i = 1; i < greatest_exponent - 1; ++i) {
        vec[i] = vec[i - 1] * base;
      }
    }

    return insertion.first;
  }


  static unsigned short log_base(unsigned short base, IntType x) {
    return (unsigned short)(log2(x) / log2(base));
  }


  static unsigned short calc_greatest_exponent(unsigned short max_elt) {
    return log_base(max_elt, std::numeric_limits<IntType>::max());
  }
};

