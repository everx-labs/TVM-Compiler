// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o - | FileCheck %s
// REQUIRES: tvm-registered-target

// CHECK-NOT: @.str

#include <tvm/contract.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/contract_handle.hpp>
#include <tvm/string.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/default_support_functions.hpp>

using namespace tvm;
using namespace schema;

__interface IBigStringTest {
  [[external]]
  void constructor() = 1;
  [[getter]]
  string str() = 2;
};

struct DBigStringTest {
};

__interface EBigStringTest {
};

static constexpr unsigned TIMESTAMP_DELAY = 1800;
using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

class BigStringTest final : public smart_interface<IBigStringTest>, public DBigStringTest {
public:
  __always_inline
  void constructor() override {
  }
  __always_inline
  string str() override {
    return "0123456789012345678901234567890123456789";
  }
  DEFAULT_SUPPORT_FUNCTIONS(IBigStringTest, replay_protection_t)
};

DEFINE_JSON_ABI(IBigStringTest, DBigStringTest, EBigStringTest);

// ----------------------------- Main entry functions ---------------------- //
DEFAULT_MAIN_ENTRY_FUNCTIONS(BigStringTest, IBigStringTest, DBigStringTest, TIMESTAMP_DELAY)

