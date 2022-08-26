#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

namespace abiv1 {

struct external_inbound_msg_header {
  uint32 function_id;
  uint64 timestamp;
};

struct external_outbound_msg_header {
  uint32 function_id;
};

struct internal_msg_header {
  uint32 function_id;
};

static __always_inline unsigned answer_id(unsigned func_id) {
  return func_id | (1ul << 31);
}

static __always_inline unsigned is_answer_id(unsigned func_id) {
  return func_id & (1ul << 31);
}

static __always_inline unsigned from_answer_id(unsigned func_id) {
  return func_id ^ (1ul << 31);
}

} // namespace abiv1

}} // namespace tvm::schema
