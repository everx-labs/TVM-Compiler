#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

namespace abiv1 {

struct external_inbound_msg_header {
  uint32_t function_id;
  uint64_t timestamp;
};

struct external_outbound_msg_header {
  uint32_t function_id;
};

struct internal_msg_header {
  uint32_t function_id;
};

static inline unsigned answer_id(unsigned func_id) {
  return func_id | (1ul << 31);
}

} // namespace abiv1

}} // namespace tvm::schema
