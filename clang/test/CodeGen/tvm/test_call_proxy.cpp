// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/default_support_functions.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/contract_handle.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>

using namespace tvm;
using namespace schema;

__interface IProposalRoot {
  __attribute__((internal, external))
  void constructor() = 1;

  __attribute__((internal, noaccept))
  void setFinished(bool_t val) = 2;

  __attribute__((internal, noaccept))
  bool_t checkFinished(uint32 answer_id) = 7;

  // getters
  __attribute__((getter))
  bool_t isFinished() = 5;
};

/*
  __reflect_proxy<IProposalRoot, Impl> generates class like this:
class Proxy : public Impl {
public:
  using Impl::Impl;

  auto checkFinished(uint32 answer_id) const {
    return Impl::_call_impl<&IProposalRoot::checkFinished>(answer_id);
  }
  auto setFinished() const {
    return Impl::_call_impl<&IProposalRoot::setFinished>();
  }
};
*/

// ================ Simulated contract code =================

using IProposalRootPtr = handle<IProposalRoot>;

__interface ITest {
  __attribute__((internal, noaccept))
  resumable<bool_t> checkAll() = 2;
};

struct DTest {
  dict_array<uint256> proposals_;
};

struct ETest {
};

static constexpr unsigned TIMESTAMP_DELAY = 1800;

class Test final : public smart_interface<ITest>, public DTest {
public:
  using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

  __always_inline
  resumable<bool_t> checkAll() {
    for (auto proposal : proposals_) {
      IProposalRootPtr handle(address::make_std(int8{0}, proposal));
      uint32 answer_id{0};
      bool_t finished = co_await handle(Grams(0)).checkFinished(answer_id);
      if (!finished)
        co_return bool_t{false};
    }
    co_return bool_t{true};
  }
  DEFAULT_SUPPORT_FUNCTIONS(ITest, replay_protection_t);
};

DEFINE_JSON_ABI(ITest, DTest, ETest);

// ----------------------------- Main entry functions ---------------------- //
DEFAULT_MAIN_ENTRY_FUNCTIONS(Test, ITest, DTest, TIMESTAMP_DELAY)

