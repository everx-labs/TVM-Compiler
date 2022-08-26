#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

namespace abiv2 {

struct external_signature {
  std::optional<bitfield<512>> signature;
};

struct external_inbound_msg_header_no_pubkey {
  uint64 timestamp;
  uint32 expire;
  uint32 function_id;
};
struct external_inbound_msg_header_with_pubkey {
  std::optional<uint256> pubkey;
  uint64 timestamp;
  uint32 expire;
  uint32 function_id;
};

template<bool has_pubkey>
using external_inbound_msg_header =
  std::conditional_t<has_pubkey,
    external_inbound_msg_header_with_pubkey,
    external_inbound_msg_header_no_pubkey>;

struct external_outbound_msg_header {
  uint32 function_id;
};

struct internal_msg_header {
  uint32 function_id;
};

// internal contract calls for non-void methods require answer_id
struct internal_msg_header_with_answer_id {
  uint32 function_id;
  uint32 answer_id;
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
