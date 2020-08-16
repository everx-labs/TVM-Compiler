// RUN: %clang -O3 -target tvm %s -S --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/
// REQUIRES: tvm-registered-target

#include <tvm/contract.hpp>
#include <tvm/contract_handle.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/default_support_functions.hpp>

using namespace tvm;
using namespace schema;

__interface ITest {
  __attribute__((internal, noaccept))
  uint128 transfer(uint128 tokens) = 1;
};

struct DTest {
  uint128 balance_;
};

__interface ETest {};

static constexpr unsigned TIMESTAMP_DELAY = 1800;

class Test final : public smart_interface<ITest>, public DTest {
public:
  using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

  __always_inline
  void constructor() {}

  __always_inline
  uint128 transfer(uint128 tokens) {
    return balance_ = std::min(tokens, balance_);
  }

  // getters
  __always_inline
  uint128 getBalance() {
    return balance_;
  }
public:
  // Support functions
  DEFAULT_SUPPORT_FUNCTIONS(ITest, replay_protection_t);

  // default processing of unknown messages
  __always_inline static int _fallback(cell msg, slice msg_body) {
    return 0;
  }
};

DEFINE_JSON_ABI(ITest, DTest, ETest);

// ----------------------------- Main entry functions ---------------------- //
DEFAULT_MAIN_ENTRY_FUNCTIONS(Test, ITest, DTest, TIMESTAMP_DELAY)

