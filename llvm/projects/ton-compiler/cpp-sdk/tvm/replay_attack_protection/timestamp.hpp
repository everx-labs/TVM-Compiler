#pragma once

#include <tvm/smart_contract_info.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/abiv2.hpp>

namespace tvm { namespace replay_attack_protection {

template<unsigned Interval>
class timestamp {
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
  static inline std::optional<persistent_t> check(unsigned msg_time, persistent_t last) {
    if (last && persistent_t(msg_time) <= last)
      return {};
    if (msg_time >= (smart_contract_info::now() + Interval) * 1000)
      return {};
    return persistent_t{msg_time};
  }
};

}} // namespace tvm::replay_attack_protection
