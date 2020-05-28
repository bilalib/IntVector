#pragma once
#include <unordered_map>
#include <vector>


struct BaseTooBigError {};

template<typename IntType>
class PowerTable
{
private:

  struct PowerList {
    std::vector<IntType> powers;
    unsigned short count;
    PowerList(IntType max)
      : powers(max), count(0)
    {}
  };

  using TableType = std::unordered_map<unsigned short, PowerList>;
  TableType table;

public:

  typename TableType::iterator find_or_build(unsigned short base) {

    unsigned short capacity = calc_capacity(base);
    if (capacity < 2) {
      throw BaseTooBigError();
    }

    auto insertion = table.insert({ base, PowerList(capacity - 1) });

    // if the insertion was sucessful, base did not already exist in table,
    // so we fill the vector at table[base] with the powers of base.
    if (insertion.second) {
      auto& vec = insertion.first->second.powers;
      vec[0] = base * base;

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


  class PowerListHolder {

  private:

    TableType& table;
    typename TableType::iterator it;

  public:

    PowerListHolder(unsigned short base_in, PowerTable& table_in) 
      : table(table_in.table), it(table_in.find_or_build(base_in))
    {}

    PowerListHolder(const PowerListHolder& other)
      : table(other.table), it(other.it)
    {}

    ~PowerListHolder() {
      if (--it->second.count == 0) {
        table.erase(it);
      }
    }

    IntType operator[](unsigned short idx) const {
      if (idx == 0) {
        return 1;
      }
      else if (idx == 1) {
        return it->first;
      }
      // powers[i] = base^(i + 2)
      return it->second.powers[idx - 2];
    }

    unsigned short capacity() const {
      return (unsigned short)it->second.powers.size();
    }
  };
};

