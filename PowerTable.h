#pragma once
#include <unordered_map>
#include <vector>



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

  class PowerListHolder {
  public:

    ~PowerListHolder() {
      if (--it->second.count == 0) {
        table.erase(it);
      }
    }

    IntType operator[](unsigned short idx) const {
      return it->second.powers[idx];
    }

  private:
    std::unordered_map<unsigned short, PowerList>& table;
    std::unordered_map<unsigned short, PowerList>::iterator it;
  };

  std::unordered_map<unsigned short, PowerList> table;

public:

  void fill(unsigned short base) {

    unsigned short max = calc_capacity(base);

    auto insertion = table.insert({ base, PowerList(max) });
    if (!insertion.second) {
      return;
    }

    auto& vec = insertion.first->second.powers;
    vec[0] = base;

    for (unsigned short i = 1; i < max; ++i) {
      vec[i] = vec[i - 1] * base;
    }

  }

private:

  static unsigned short int_log2(unsigned long long x) {
    unsigned short result = 0;
    while (x >>= 1) {
      ++result;
    }
    return result;
  }

  static unsigned short log_base(unsigned short base, IntType x) {
    return int_log2(x) / int_log2(base);
  }

  static unsigned short calc_capacity(unsigned short max_elt) {
    return log_base(max_elt, std::numeric_limits<IntType>::max());
  }


};

