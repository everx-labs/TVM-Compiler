#pragma once

#include <tvm/tuple_stack.hpp>

namespace tvm {

class tree_cell {
public:
  explicit tree_cell(cell cl) : root_(cl) {}

  std::tuple<unsigned, unsigned> get_bits_and_cells() const {
    unsigned bits = 0;
    unsigned cells = 0;
    
    tuple_stack<slice> slices;
    slices.push(root_.ctos());

    while (!slices.empty()) {
      slice sl = slices.top();
      auto [cur_bits, cur_refs] = sl.sbitrefs();
      if (!cur_refs) {
        bits += cur_bits;
        ++cells;
        slices.pop();
      } else {
        parser p(sl);
        slice child_sl = p.ldrefrtos();
        slices.top() = p.sl(); // updating top slice to slice without one parsed reference
        slices.push(child_sl);
      }
    }
    return { bits, cells };
  }
  cell root_;
};

} // namespace tvm

