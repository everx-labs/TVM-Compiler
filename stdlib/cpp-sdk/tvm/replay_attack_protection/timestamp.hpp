#pragma once

#include <tvm/smart_contract_info.hpp>
#include <tvm/schema/basics.hpp>

namespace tvm { namespace replay_attack_protection {

template<unsigned Interval>
class timestamp {
public:
  using persistent_t = schema::uint64_t;
  static inline persistent_t init() {
    return persistent_t(0);
  }
  static inline std::optional<persistent_t> check(unsigned msg_time, persistent_t last) {
    if (last && msg_time <= last)
      return {};
    if (msg_time >= smart_contract_info::now() + Interval)
      return {};
    return persistent_t{msg_time};
  }
};

}} // namespace tvm::replay_attack_protection
