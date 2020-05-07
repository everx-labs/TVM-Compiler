// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/parser.hpp>
#include <tvm/builder.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/schema/abiv1.hpp>
#include <tvm/schema/json-abi-gen.hpp>
#include <tvm/smart_contract_info.hpp>
#include <tvm/signature_checker.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/smart_contract_info.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/contract.hpp>
#include <tvm/to_dictionary.hpp>
#include <tvm/from_dictionary.hpp>
#include <tvm/call_wrapper.hpp>
#include <tvm/msg_address.inline.hpp>

#include "Wallet.hpp"

using namespace tvm::schema;
using namespace tvm;

class Wallet : public tvm::contract {
public:
  struct error_code : tvm::error_code {
    static constexpr int wrong_owner                = 101;
    static constexpr int invalid_transfer_value     = 102;
  };
  using replay_protection = replay_attack_protection::timestamp<100>;

  struct persistent_values {
    uint256 owner;
    MsgAddress subscription;
    replay_protection::persistent_t timestamp;
  } p;
  using persistent_strategy = persistent_strategy_in_slice<Wallet>;
  using wrapper = call_wrapper<Wallet, persistent_strategy>;

  inline int constructor(cell msg, slice msg_body);
  inline int set_subscription_account(cell msg, slice msg_body);
  inline int get_subscription_account(cell msg, slice msg_body);
  inline int send_transaction(cell msg, slice msg_body);

  static int constructor_external(cell msg, slice msg_body);
  static int set_subscription_account_external(cell msg, slice msg_body);
  static int get_subscription_account_external(cell msg, slice msg_body);
  static int send_transaction_external(cell msg, slice msg_body);
private:
  void replay_protection_check(unsigned msg_timestamp) {
    auto v = replay_protection::check(msg_timestamp, p.timestamp);
    tvm_assert(!!v, error_code::replay_attack_check);
    p.timestamp = *v;
  }
};
DEFINE_JSON_ABI(IWallet, DWallet, EWallet);

// Public call wrappers
int Wallet::constructor_external(cell msg, slice msg_body) {
  return wrapper::process_constructor(&Wallet::constructor, msg, msg_body);
}

int Wallet::set_subscription_account_external(cell msg, slice msg_body) {
  return wrapper::process_external_call(&Wallet::set_subscription_account, msg, msg_body);
}

int Wallet::get_subscription_account_external(cell msg, slice msg_body) {
  return wrapper::process_external_call(&Wallet::get_subscription_account, msg, msg_body);
}

int Wallet::send_transaction_external(cell msg, slice msg_body) {
  return wrapper::process_external_call(&Wallet::send_transaction, msg, msg_body);
}

// Public calls
int Wallet::constructor(cell msg, slice msg_body) {
  auto [sender_key, =msg_body] = signature_checker<error_code::invalid_signature>::check(msg_body);
  tvm_accept();

  p.owner = sender_key;
  p.subscription = MsgAddress{MsgAddressExt{addr_none{}}};
  p.timestamp = replay_protection::init();
  return 111;
}

// For subscription contract
int Wallet::set_subscription_account(cell msg, slice msg_body) {
  struct args_t {
    abiv1::external_inbound_msg_header hdr;
    MsgAddress address;
  };
  auto [sender_key, =msg_body] = signature_checker<error_code::invalid_signature>::check(msg_body);

  unsigned owner_v = p.owner.get();
  tvm_assert(owner_v == sender_key, error_code::wrong_owner);

  auto args = parse_args<args_t>(msg_body, error_code::bad_arguments);
  replay_protection_check(args.hdr.timestamp.get());
  tvm_accept();

  p.subscription = args.address;
  return 222;
}

int Wallet::get_subscription_account(cell msg, slice msg_body) {
  using args_t = abiv1::external_inbound_msg_header;
  struct return_t {
    abiv1::external_outbound_msg_header hdr;
    MsgAddress subscription;
  };

  auto [sender_key, =msg_body] = signature_checker<error_code::invalid_signature>::check(msg_body);

  MsgAddress subscription_v = p.subscription;
  auto args = parse_args<args_t>(msg_body, error_code::bad_arguments);
  replay_protection_check(args.timestamp.get());
  tvm_accept();

  return_t result;
  result.hdr.function_id = abiv1::answer_id(args.function_id());
  result.subscription = subscription_v;

  MsgAddressExt answer_dst = get_incoming_ext_src(msg);

  ext_out_msg_info_relaxed out_info;
  out_info.src = addr_none{};
  out_info.dest = answer_dst;
  out_info.created_lt = 0;
  out_info.created_at = 0;

  message_relaxed<return_t> out_msg;
  out_msg.info = out_info;
  out_msg.body = result;

  tvm_sendmsg(build(out_msg).endc(), 0);
  return 333;
}

// Allows to transfer grams to destination account.
int Wallet::send_transaction(cell msg, slice msg_body) {
  struct args_t {
    abiv1::external_inbound_msg_header hdr;
    MsgAddressInt dest;  // Transfer target address.
    uint_t<128> value; // Nanograms value to transfer.
    bool_t bounce;
  };

  args_t args = parse_args<args_t>(msg_body, error_code::bad_arguments);
  replay_protection_check(args.hdr.timestamp.get());

  auto [sender_key, =msg_body] = signature_checker<error_code::invalid_signature>::check(msg_body);
  unsigned owner_v = p.owner.get();
  MsgAddress subscription_v = p.subscription;

  MsgAddressExt sender = get_incoming_ext_src(msg);

  bool has_subscription = subscription_v != MsgAddress{MsgAddressExt{addr_none{}}};
  tvm_assert(sender_key == owner_v ||
          (has_subscription && MsgAddress(sender) == subscription_v),
          error_code::wrong_owner);

  tvm_accept();

  int balance = smart_contract_info::balance_remaining();
  tvm_assert(args.value() > 0 && args.value() < balance, error_code::invalid_transfer_value);

  tvm_transfer(args.dest, args.value.get(), args.bounce.get());
  return 444;
}

// ----------------- main entry functions ---------------------- //

__attribute__((tvm_raw_func)) int main_external(__tvm_cell msg, __tvm_slice msg_body) {
  cell msg_v(msg);
  slice msg_body_v(msg_body);

  parser msg_parser(msg_body_v);

  auto func_id = msg_parser.ldu(32);
  
  switch (func_id) {
  case id_v<&IWallet::constructor>:
    return Wallet::constructor_external(msg_v, msg_body_v);
  case id_v<&IWallet::set_subscription_account>:
    return Wallet::set_subscription_account_external(msg_v, msg_body_v);
  case id_v<&IWallet::get_subscription_account>:
    return Wallet::get_subscription_account_external(msg_v, msg_body_v);
  case id_v<&IWallet::send_transaction>:
    return Wallet::send_transaction_external(msg_v, msg_body_v);
  }
  tvm_throw(error_code::wrong_public_call);
  return 0;
}

__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {
  tvm_throw(error_code::unsupported_call_method);
  return 0;
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
