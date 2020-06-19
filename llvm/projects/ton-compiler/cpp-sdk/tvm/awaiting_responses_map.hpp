#pragma once

#include <tvm/dict_map.hpp>
#include <tvm/error_code.hpp>

namespace tvm {

class awaiting_responses_map {
public:
  // works only for addr_std { kind:0b10, optional<Anycast>:0b0, workchain_id:uint8, address:uint256 }
  static constexpr unsigned addr_size = 2 + 1 + 8 + 256;
  static constexpr unsigned exec_id_size = 32;
  static constexpr unsigned key_size = addr_size + exec_id_size;

  __always_inline
  void add_awaiting(slice addr, unsigned exec_id, cell state) {
    slice key = make_key(addr, exec_id);
    dict_.dictsetref(state, key, key_size);
  }
  __always_inline
  cell find_awaiting(slice addr, unsigned exec_id) {
    slice key = make_key(addr, exec_id);
    auto [cl, succ] = dict_.dictdelgetref(key, key_size);
    require(succ, error_code::unexpected_answer);
    return cl;
  }
  __always_inline
  slice make_key(slice addr, unsigned exec_id) const {
    slice sl = builder().stslice(addr).stu(exec_id, 32).make_slice();
    require(sl.sbits() == key_size, error_code::wrong_awaiting_addr_size);
    return sl;
  }

  dictionary dict_;
};

} // namespace tvm

