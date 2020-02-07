// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../../stdlib -o -
// REQUIRES: tvm-registered-target

#include <tvm/contract.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include "Piggybank.hpp"
#include <tvm/msg_address.inline.hpp>

using namespace tvm::schema;
using namespace tvm;

class Piggybank final : public smart_interface<IPiggybank>, public DPiggybank {
public:
  struct error_code : tvm::error_code {
    static constexpr int wrong_owner = 101;
    static constexpr int not_enough_balance = 102;
  };

  __always_inline void constructor(lazy<MsgAddress> pb_owner, uint_t<256> pb_limit) final;
  __always_inline void deposit() final;
  __always_inline void withdraw() final;
};
DEFINE_JSON_ABI(IPiggybank);

// -------------------------- Public calls --------------------------------- //
void Piggybank::constructor(lazy<MsgAddress> pb_owner, uint_t<256> pb_limit) {
  owner = pb_owner;
  limit = pb_limit;
  balance = 0;
}

void Piggybank::deposit() {
  balance += int_msg().unpack().value();
}

void Piggybank::withdraw() {
  auto sender = int_msg().unpack().int_sender();

  require(sender.raw_slice() == owner.raw_slice(), error_code::wrong_owner);
  require(balance >= limit, error_code::not_enough_balance);

  tvm_transfer(sender, balance, false);
  balance = 0;
}

// ----------------------------- Main entry functions ---------------------- //

__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {
  return smart_switch</*Internal=*/false, Piggybank, IPiggybank, DPiggybank,
                      replay_attack_protection::timestamp<100>>(msg, msg_body);
}

__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {
  return smart_switch</*Internal=*/true, Piggybank, IPiggybank, DPiggybank,
                      replay_attack_protection::timestamp<100>>(msg, msg_body);
}

__attribute__((tvm_raw_func)) int main_ticktock(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

__attribute__((tvm_raw_func)) int main_split(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

__attribute__((tvm_raw_func)) int main_merge(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
}

