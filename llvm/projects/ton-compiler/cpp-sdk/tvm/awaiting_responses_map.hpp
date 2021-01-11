#pragma once

#include <tvm/dict_map.hpp>
#include <tvm/error_code.hpp>
#include <tvm/schema/message.hpp>

namespace tvm {

struct awaiting_record {
  schema::uint32 func_id;
  schema::int8 waiting_workchain;
  schema::uint256 waiting_addr;
  schema::ref<schema::lazy<schema::MsgAddress>> return_addr;
  // ... frame values
};

class awaiting_responses_map {
public:
  static constexpr unsigned key_size = 32;

  __always_inline
  schema::uint32 find_next_answer_id() {
    do {
      auto [cl, succ] = dict_.dictugetref(next_answer_id_.get(), key_size);
      if (succ) {
        ++next_answer_id_;
        next_answer_id_ &= ((1u << 31) - 1);
        continue;
      }
    } while (false);
    return next_answer_id_;
  }
  __always_inline
  void add_awaiting(unsigned exec_id, cell state) {
    dict_.dictusetref(state, exec_id, key_size);
  }
  __always_inline
  cell find_awaiting(unsigned exec_id) {
    auto [cl, succ] = dict_.dictugetref(exec_id, key_size);
    require(succ, error_code::unexpected_answer);
    return cl;
  }
  __always_inline
  void delete_awaiting(unsigned exec_id) {
    dict_.dictudel(exec_id, key_size);
  }

  schema::uint32 next_answer_id_;
  dictionary dict_;
};

} // namespace tvm

