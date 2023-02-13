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
  static constexpr unsigned lookup_bitsize = 6;
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
  constexpr static unsigned lookup_table_val() {
    constexpr unsigned lookup[] = {
      32, 0, 1,
      26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11,
      0, 13, 4, 7, 17, 0, 25, 22, 31, 15, 29,
      10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19,
      18
    };
    unsigned val = 0;
    #pragma unroll
    for (unsigned i = 0; i < 37; ++i) {
      val |= lookup[i] << (i * lookup_bitsize);
    }
    return val;
  }
  static unsigned lookup(unsigned idx) {
    static constexpr unsigned lookup_table = lookup_table_val();
    return (lookup_table >> (idx * lookup_bitsize)) & ((1 << lookup_bitsize) - 1);
  }
  static unsigned count_trail_zeros(uint32 val) {
    int x = val.get();
    return lookup((-x & x) % 37);
  }
  static unsigned count_trail_ones(uint32 val) {
    return count_trail_zeros(val ^ 0xFFFFFFFF);
  }
  /// Returns empty optional in case of failed check.
  /// Otherwise, returns new persistent state of rolling ids buffer
  __attribute__((noinline))
  static std::optional<persistent_t> check(unsigned msg_id, persistent_t last) {
    int start_id = last.get() >> 32;
    int bitmask = last.get() & 0xFFFFFFFF;
    require(msg_id >= start_id && msg_id < start_id + 32, error_code::replay_attack_check);
    unsigned bitnum = msg_id - start_id;
    bool exists = (bitmask >> bitnum) & 1;
    require(!exists, error_code::replay_attack_check);

    bitmask = bitmask | (1 << bitnum);
    if (start_id == msg_id) {
      // when we receive msg_id == start_id, we need to roll the buffer to the next unregistered message
      unsigned n = count_trail_ones(uint32(bitmask));
      //slice sl = builder().stu(bitmask, 32).make_slice();
      //unsigned n = __builtin_tvm_sdcnttrail1(sl);
      bitmask = bitmask >> n;
      start_id += n;
      return persistent_t{(start_id << 32) | bitmask};
    }
    return persistent_t{(start_id << 32) | bitmask};
  }
};

}} // namespace tvm::replay_attack_protection
