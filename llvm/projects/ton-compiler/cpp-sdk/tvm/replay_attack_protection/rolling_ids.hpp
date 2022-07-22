#pragma once

#include <tvm/smart_contract_info.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/abiv2.hpp>

namespace tvm { namespace replay_attack_protection {

/// Rolling ids buffer
/// (u32 start_id, bitmask[32])
/// start_id - id of the next not-yet-received message
/// Allowed message should have id from [start_id, start_id + 32),
///  and not yet registered in the bitmask.
/// \warning start_id is 32-bit value, so a contract with this protection will be safe only
///  for 4 294 967 295 external messages. This replay protection suits well for personal contracts.
/// And allows to send 32 messages in parallel.
class rolling_ids {
public:
  using persistent_t = schema::uint64;
  static inline persistent_t init() {
    return persistent_t(0);
  }
  template<class Header>
  static inline std::optional<persistent_t> check(
      Header hdr, persistent_t last) {
    return check(hdr.timestamp.get(), last);
  }
  /// Returns array of next allowed message ids. For call from contract getter.
  static inline dict_array<uint32> get_next_ids(persistent_t last) {
    unsigned start_id = last.get() >> 32;
    unsigned bitmask = last.get() & 0xFFFFFFFF;
    dict_array<uint32> rv;
    for (unsigned i = 0; i < 32; ++i) {
      if (!((bitmask >> i) & 1))
        rv.push_back(uint32(start_id + i));
    }
    return rv;
  }
  /// Returns empty optional in case of failed check.
  /// Otherwise, returns new persistent state of rolling ids buffer
  static inline std::optional<persistent_t> check(unsigned msg_id, persistent_t last) {
    int start_id = last.get() >> 32;
    int bitmask = last.get() & 0xFFFFFFFF;
    if (msg_id < start_id || msg_id >= start_id + 32)
      return {};
    unsigned bitnum = msg_id - start_id;
    bool exists = (bitmask >> bitnum) & 1;
    if (exists)
      return {};

    bitmask = bitmask | (1 << bitnum);
    if (start_id == msg_id) {
      // when we receive msg_id == start_id, we need to roll the buffer to the next unregistered message
      slice sl = builder().stu(bitmask, 32).make_slice();
      unsigned n = __builtin_tvm_sdcnttrail1(sl);
      bitmask = bitmask >> n;
      start_id += n;
      return persistent_t{(start_id << 32) | bitmask};
    }
    return persistent_t{(start_id << 32) | bitmask};
  }
};

}} // namespace tvm::replay_attack_protection
