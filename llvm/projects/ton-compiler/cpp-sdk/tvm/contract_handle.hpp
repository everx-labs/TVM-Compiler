#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/schema/chain_fold.hpp>

#include <experimental/type_traits>
#include <experimental/coroutine>

#include <tvm/message_flags.hpp>
#include <tvm/globals.hpp>

namespace tvm {

template<auto Func>
struct get_func_rv {};
template<typename Cl, typename RetT, typename... Args, RetT (Cl::*Func)(Args...)>
struct get_func_rv<Func> {
  using type = typename schema::resumable_subtype<RetT>::type;
};
template<auto Func>
using get_func_rv_t = typename get_func_rv<Func>::type;

template<class Interface, class Func, class... Args>
using good_call_args_t = decltype( (((Interface*)nullptr)->*(Func::value))(Args{}...) );
template<class Interface, auto Func, class... Args>
constexpr bool good_call_args_v = std::experimental::is_detected_v<good_call_args_t, Interface, proxy_method<Func>, Args...>;

template<auto Func>
constexpr auto prepare_internal_header() {
  using namespace schema;
  if constexpr (!std::is_void_v<get_func_rv_t<Func>> && get_interface_method_ptr_internal<Func>::value &&
                get_interface_method_ptr_answer_id<Func>::value) {
    unsigned answer_id = temporary_data::getglob(global_id::answer_id);
    return abiv2::internal_msg_header_with_answer_id{ uint32(id_v<Func>), uint32(abiv2::answer_id(answer_id)) };
  } else {
    return abiv2::internal_msg_header{ uint32(id_v<Func>) };
  }
}

// Prepare message cell for contract call (internal message)
template<auto Func, class... Args>
__always_inline
cell contract_call_prepare(address addr, schema::Grams amount, bool ihr_disabled,
    schema::lazy<schema::MsgAddress> src, Args... args) {
  using namespace schema;

  auto hdr = prepare_internal_header<Func>();
  auto hdr_plus_args = std::make_tuple(hdr, args...);
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = ihr_disabled;
  out_info.bounce = true;
  out_info.bounced = false;
  out_info.src = src;
  out_info.dest = addr;
  out_info.created_lt = 0;
  out_info.created_at = 0;
  out_info.value.grams = amount;

  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = out_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    return build(out_msg).endc();
  } else {
    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = out_info;
    out_msg.body = hdr_plus_args;
    return build(out_msg).endc();
  }
}

template<auto Func, class... Args>
__always_inline
cell internal_body_prepare(address addr, Args... args) {
  using namespace schema;

  auto hdr = prepare_internal_header<Func>();
  auto hdr_plus_args = std::make_tuple(hdr, args...);
  using est_t = estimate_element<decltype(hdr_plus_args)>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    return build(chain_tup).endc();
  } else {
    return build(hdr_plus_args).endc();
  }
}

// Prepare message body for external call
template<auto Func, class... Args>
__always_inline
cell external_body_prepare(address addr, Args... args) {
  using namespace schema;

  // placeholder for signature
  abiv2::external_signature signature_place {
    .signature = bitfield<512>{builder().stu(0, 256).stu(0, 256).make_slice()}
  };
  abiv2::external_inbound_msg_header_no_pubkey hdr{
    .timestamp = uint64{0},
    .expire = uint32{0},
    .function_id = uint32(id_v<Func>)
  };
  auto hdr_plus_args = std::make_tuple(signature_place, hdr, args...);
  ext_in_msg_info msg_info {
    .src = MsgAddressExt{addr_none{}},
    .dest = addr,
    .import_fee = Grams{0}
  };
  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    return build(chain_tup).endc();
  } else {
    return build(hdr_plus_args).endc();
  }
}

template<auto Func, class... Args>
__always_inline
cell external_body_prepare_with_pubkey(address addr, schema::uint256 pubkey, Args... args) {
  using namespace schema;

  // placeholder for signature
  abiv2::external_signature signature_place {
    .signature = bitfield<512>{builder().stu(0, 256).stu(0, 256).make_slice()}
  };
  abiv2::external_inbound_msg_header_with_pubkey hdr{
    .pubkey = pubkey,
    .timestamp = uint64{0},
    .expire = uint32{0},
    .function_id = uint32(id_v<Func>)
  };
  auto hdr_plus_args = std::make_tuple(signature_place, hdr, args...);
  ext_in_msg_info msg_info {
    .src = MsgAddressExt{addr_none{}},
    .dest = addr,
    .import_fee = Grams{0}
  };
  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    return build(chain_tup).endc();
  } else {
    return build(hdr_plus_args).endc();
  }
}

// Prepare message cell for external call (should later be signed by debot engine)
template<auto Func, class... Args>
__always_inline
cell external_call_prepare(address addr, Args... args) {
  using namespace schema;

  // placeholder for signature
  abiv2::external_signature signature_place {
    .signature = bitfield<512>{builder().stu(0, 256).stu(0, 256).make_slice()}
  };
  abiv2::external_inbound_msg_header_no_pubkey hdr{
    .timestamp = uint64{0},
    .expire = uint32{0},
    .function_id = uint32(id_v<Func>)
  };
  auto hdr_plus_args = std::make_tuple(signature_place, hdr, args...);
  ext_in_msg_info msg_info {
    .src = MsgAddressExt{addr_none{}},
    .dest = addr,
    .import_fee = Grams{0}
  };

  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    return build(out_msg).endc();
  } else {
    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = hdr_plus_args;
    return build(out_msg).endc();
  }
}

// Prepare message cell for getter call (no signature/pubkey)
template<auto Func, class... Args>
__always_inline
cell getter_call_prepare(address addr, Args... args) {
  using namespace schema;

  uint32 answer_id(abiv2::answer_id(temporary_data::getglob(global_id::answer_id)));

  // placeholder for signature
  abiv2::external_signature signature_place {};
  abiv2::external_inbound_msg_header_no_pubkey hdr{
    .timestamp = uint64{0},
    .expire = answer_id, // answer_id is packed in `expire` place for debot getter runs
    .function_id = uint32(id_v<Func>)
  };
  auto hdr_plus_args = std::make_tuple(signature_place, hdr, args...);
  ext_in_msg_info msg_info {
    .src = MsgAddressExt{addr_none{}},
    .dest = addr,
    .import_fee = Grams{0}
  };

  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    return build(out_msg).endc();
  } else {
    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = hdr_plus_args;
    return build(out_msg).endc();
  }
}

// expire_val is used to place answer_id for debot external-in calls
template<auto Func, class... Args>
__always_inline
cell external_call_prepare_with_pubkey(address addr, schema::uint32 expire_val,
                                       schema::uint256 pubkey, Args... args) {
  using namespace schema;

  // placeholder for signature
  abiv2::external_signature signature_place {
    .signature = bitfield<512>{builder().stu(0, 256).stu(0, 256).make_slice()}
  };
  abiv2::external_inbound_msg_header_with_pubkey hdr{
    .pubkey = pubkey,
    .timestamp = uint64{0},
    .expire = expire_val,
    .function_id = uint32(id_v<Func>)
  };
  auto hdr_plus_args = std::make_tuple(signature_place, hdr, args...);
  ext_in_msg_info msg_info {
    .src = MsgAddressExt{addr_none{}},
    .dest = addr,
    .import_fee = Grams{0}
  };

  using est_t = estimate_element<message<decltype(hdr_plus_args)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_args);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    return build(out_msg).endc();
  } else {
    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = msg_info;
    out_msg.body = hdr_plus_args;
    return build(out_msg).endc();
  }
}

template<auto Func, class... Args>
__always_inline
void debot_call_ext_in_impl(address addr, unsigned flags, Args... args) {
  using namespace schema;
  uint32 answer_id(abiv2::answer_id(temporary_data::getglob(global_id::answer_id)));
  cell out_msg = external_call_prepare_with_pubkey<Func>(addr, answer_id, uint256(0), args...);
  tvm_sendmsg(out_msg, flags);
}

template<auto Func, class... Args>
__always_inline
void contract_call_impl(address addr, schema::Grams amount,
                        unsigned flags, bool ihr_disabled, Args... args) {
  using namespace schema;
  tvm_sendmsg(contract_call_prepare<Func>(addr, amount, ihr_disabled, MsgAddress{MsgAddressExt{addr_none{}}}, args...), flags);
}

// Deploy message with function call (immediately after deploy)
template<auto Func, class... Args>
__always_inline
void contract_deploy_impl(address addr, schema::StateInit init,
                          schema::Grams amount, unsigned flags, bool ihr_disabled, Args... args) {
  using namespace schema;

  auto hdr = prepare_internal_header<Func>();
  auto hdr_plus_args = std::make_tuple(hdr, args...);
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = ihr_disabled;
  out_info.bounce = true;
  out_info.bounced = false;
  out_info.src = addr_none{};
  out_info.dest = addr;
  out_info.created_lt = 0;
  out_info.created_at = 0;
  out_info.value.grams = amount;

  auto chain_tup = make_chain_tuple(hdr_plus_args);
  message_relaxed<decltype(chain_tup)> out_msg;
  out_msg.info = out_info;
  Either<StateInit, ref<StateInit>> init_ref = ref<StateInit>{init};
  out_msg.init = init_ref;
  out_msg.body = ref<decltype(chain_tup)>{chain_tup};
  tvm_sendmsg(build(out_msg).endc(), flags);
}

// Deploy without any function called (with func_id = 0)
// Deployed contract should support `fallback`
__always_inline
void contract_deploy_noop_impl(address addr, schema::StateInit init,
                               schema::Grams amount, unsigned flags, bool ihr_disabled) {
  using namespace schema;

  auto hdr = abiv2::internal_msg_header{ uint32(0) };
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = ihr_disabled;
  out_info.bounce = true;
  out_info.bounced = false;
  out_info.src = addr_none{};
  out_info.dest = addr;
  out_info.created_lt = 0;
  out_info.created_at = 0;
  out_info.value.grams = amount;

  message_relaxed<decltype(hdr)> out_msg;
  out_msg.info = out_info;
  Either<StateInit, ref<StateInit>> init_ref = ref<StateInit>{init};
  out_msg.init = init_ref;
  out_msg.body = hdr;
  tvm_sendmsg(build(out_msg).endc(), flags);
}

template<class RetT>
struct wait_call_result {
  __always_inline
  bool await_ready() const { return false; }
  __always_inline
  void await_suspend(std::experimental::coroutine_handle<>) const {}
  __always_inline
  RetT await_resume() const {
    using namespace schema;
    using est_t = estimate_element<RetT>;
    if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
      parser p(__builtin_tvm_cast_to_slice(
        temporary_data::getglob(global_id::coroutine_answer_slice)));
      using LinearTup = decltype(make_chain_tuple<32, 0>(to_std_tuple(RetT{})));
      // uncomment to print in remark:
      //__reflect_echo<print_chain_tuple<LinearTup>().c_str()>{};
      auto linear_tup = parse<LinearTup>(p);
      return chain_fold<RetT>(linear_tup);
    } else {
      return parse<RetT>(__builtin_tvm_cast_to_slice(
        temporary_data::getglob(global_id::coroutine_answer_slice)));
    }
  }
};

template<typename RetT>
using awaitable_ret_t = std::conditional_t<std::is_void_v<RetT>, void, wait_call_result<RetT>>;

class contract_call_configured {
public:
  contract_call_configured(address addr, schema::Grams amount, unsigned flags, bool ihr_disabled)
    : addr_(addr), amount_(amount), flags_(flags), ihr_disabled_(ihr_disabled) {}
  template<auto Func, class... Args>
  __always_inline void call(Args... args) const {
    contract_call_impl<Func>(addr_, amount_, flags_, ihr_disabled_, args...);
  }
  template<auto Func, class... Args>
  __always_inline
  awaitable_ret_t<get_func_rv_t<Func>> _call_impl(Args... args) const {
    contract_call_impl<Func>(addr_, amount_, flags_, ihr_disabled_, args...);
    if constexpr (!std::is_void_v<get_func_rv_t<Func>>) {
      temporary_data::setglob(global_id::coroutine_wait_addr, __builtin_tvm_cast_from_slice(addr_.sl()));
      return {};
    }
  }
private:
  address addr_;
  schema::Grams amount_;
  unsigned flags_;
  bool ihr_disabled_;
};

// message is sent to one contract, but answer is expected from other contract
template<class ReturnVal>
class tail_call_configured {
public:
  tail_call_configured(address addr, address wait_addr, schema::Grams amount, unsigned flags, bool ihr_disabled)
    : addr_(addr), wait_addr_(wait_addr), amount_(amount), flags_(flags), ihr_disabled_(ihr_disabled) {}
  template<auto Func, class... Args>
  __always_inline
  awaitable_ret_t<ReturnVal> _call_impl(Args... args) const {
    static_assert(!std::is_void_v<ReturnVal>);

    tvm_sendmsg(contract_call_prepare<Func>(addr_, amount_, ihr_disabled_, {wait_addr_.sl()}, args...), flags_);
    temporary_data::setglob(global_id::coroutine_wait_addr, __builtin_tvm_cast_from_slice(wait_addr_.sl()));
    return {};
  }
private:
  address addr_;
  address wait_addr_;
  schema::Grams amount_;
  unsigned flags_;
  bool ihr_disabled_;
};

class run_getter {
public:
  explicit run_getter(address addr) : addr_(addr) {}
  template<auto Func, class... Args>
  __always_inline
  awaitable_ret_t<get_func_rv_t<Func>> _call_impl(Args... args) const {
    static_assert(!std::is_void_v<get_func_rv_t<Func>>, "Void return for getter");

    cell msg = getter_call_prepare<Func>(addr_, args...);
    tvm_sendmsg(msg, DEFAULT_MSG_FLAGS);
    // For getter call we are expecting the answer from debot engine
    auto awaiting_addr = address::make_std(schema::int8(0), schema::uint256(0));
    temporary_data::setglob(global_id::coroutine_wait_addr, __builtin_tvm_cast_from_slice(awaiting_addr.sl()));
    return {};
  }
private:
  address addr_;
};

class contract_deploy_configured {
public:
  contract_deploy_configured(address addr, schema::StateInit init,
                             schema::Grams amount, unsigned flags, bool ihr_disabled)
    : addr_(addr), init_(init), amount_(amount), flags_(flags), ihr_disabled_(ihr_disabled) {}
  template<auto Func, class... Args>
  __always_inline void call(Args... args) const {
    contract_deploy_impl<Func>(addr_, init_, amount_, flags_, ihr_disabled_, args...);
  }
  template<auto Func, class... Args>
  __always_inline
  awaitable_ret_t<get_func_rv_t<Func>> _call_impl(Args... args) const {
    contract_deploy_impl<Func>(addr_, init_, amount_, flags_, ihr_disabled_, args...);
    if constexpr (!std::is_void_v<get_func_rv_t<Func>>) {
      temporary_data::setglob(global_id::coroutine_wait_addr, __builtin_tvm_cast_from_slice(addr_.sl()));
      return {};
    }
  }
private:
  address addr_;
  schema::StateInit init_;
  schema::Grams amount_;
  unsigned flags_;
  bool ihr_disabled_;
};

// class for preparing (internal) call message cell without sending it
class contract_call_prepare_only {
public:
  contract_call_prepare_only(address addr, schema::Grams amount, bool ihr_disabled)
    : addr_(addr), amount_(amount), ihr_disabled_(ihr_disabled) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    using namespace schema;
    return contract_call_prepare<Func>(addr_, amount_, ihr_disabled_, MsgAddress{MsgAddressExt{addr_none{}}}, args...);
  }
private:
  address addr_;
  schema::Grams amount_;
  bool ihr_disabled_;
};

// class for preparing (internal) call message body cell without sending it
class body_prepare_only {
public:
  explicit body_prepare_only(address addr)
    : addr_(addr) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return internal_body_prepare<Func>(addr_, args...);
  }
private:
  address addr_;
};

// prepare only body for external call
class external_body_prepare_only {
public:
  explicit external_body_prepare_only(address addr)
    : addr_(addr) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return external_body_prepare<Func>(addr_, args...);
  }
private:
  address addr_;
};
// the same with pubkey in header
class external_body_prepare_only_with_pubkey {
public:
  external_body_prepare_only_with_pubkey(address addr, schema::uint256 pubkey)
    : addr_(addr), pubkey_(pubkey) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return external_body_prepare_with_pubkey<Func>(addr_, pubkey_, args...);
  }
private:
  address addr_;
  schema::uint256 pubkey_;
};

// For external-in messages from debot we converting void return value to bool (to be awaitable)
template<typename RetT>
using awaitable_ext_t = std::conditional_t<std::is_void_v<RetT>,
  wait_call_result<schema::bool_t>, wait_call_result<RetT>>;

class debot_call_ext_in {
public:
  debot_call_ext_in(address addr, unsigned flags)
    : addr_(addr), flags_(flags) {}
  template<auto Func, class... Args>
  __always_inline
  awaitable_ext_t<get_func_rv_t<Func>> _call_impl(Args... args) const {
    debot_call_ext_in_impl<Func>(addr_, flags_, args...);
    // For external-in call we are expecting the answer from debot engine
    auto awaiting_addr = address::make_std(schema::int8(0), schema::uint256(0));
    temporary_data::setglob(global_id::coroutine_wait_addr, __builtin_tvm_cast_from_slice(awaiting_addr.sl()));
    return {};
  }
private:
  address addr_;
  unsigned flags_;
};
// class for preparing external call message cell
class external_call_prepare_only {
public:
  explicit external_call_prepare_only(address addr)
    : addr_(addr) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return external_call_prepare<Func>(addr_, args...);
  }
private:
  address addr_;
};
// the same with pubkey in header
class external_call_prepare_only_with_pubkey {
public:
  external_call_prepare_only_with_pubkey(address addr, schema::uint256 pubkey)
    : addr_(addr), pubkey_(pubkey) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return external_call_prepare_with_pubkey<Func>(addr_, schema::uint32(0), pubkey_, args...);
  }
private:
  address addr_;
  schema::uint256 pubkey_;
};

template<class Interface>
class contract_handle {
  static const unsigned only_external = 0; // filter only external methods in proxy interface
  static const unsigned only_internal = 1; // filter only internal methods in proxy interface
  static const unsigned only_getter = 2;   // filter only getter methods in proxy interface
public:
  // for internal call
  using proxy = __reflect_proxy<Interface, contract_call_configured, only_internal>;
  // for internal deploy
  using proxy_deploy = __reflect_proxy<Interface, contract_deploy_configured, only_internal>;
  // prepare full internal call message (not send)
  using proxy_prepare = __reflect_proxy<Interface, contract_call_prepare_only, only_internal>;
  // prepare internal call message body
  using proxy_body = __reflect_proxy<Interface, body_prepare_only, only_internal>;
  // prepare external call message without pubkey
  using proxy_prepare_ext = __reflect_proxy<Interface, external_call_prepare_only, only_external>;
  // prepare external call message with pubkey
  using proxy_prepare_ext_with_pubkey = __reflect_proxy<Interface, external_call_prepare_only_with_pubkey, only_external>;
  // perform external call with zero pubkey and signature (used by debots to call inside network)
  using proxy_debot_ext_in = __reflect_proxy<Interface, debot_call_ext_in, only_external>;
  // prepare external call message body
  using proxy_body_ext = __reflect_proxy<Interface, external_body_prepare_only, only_external>;
  // prepare external call message body with pubkey
  using proxy_body_ext_with_pubkey = __reflect_proxy<Interface, external_body_prepare_only_with_pubkey, only_external>;

  // for getter run (from debot)
  using proxy_run_getter = __reflect_proxy<Interface, run_getter, only_getter>;
  // for tail call - when return is expected from another contract (a->b->c->a)
  template<class ReturnVal>
  using proxy_tail_call = __reflect_proxy<Interface, tail_call_configured<ReturnVal>, only_internal>;

  contract_handle() {}
  contract_handle(address addr) : addr_(addr) {}

  template<auto Func, class... Args>
  __always_inline
  void call(schema::Grams amount, Args... args) {
    contract_call_impl<Func>(addr_, amount, DEFAULT_MSG_FLAGS, true, args...);
  }

  template<auto Func, class... Args>
  __always_inline
  void deploy_call(schema::StateInit init, schema::Grams amount, Args... args) {
    contract_deploy_impl<Func>(addr_, init, amount, DEFAULT_MSG_FLAGS, true, args...);
  }

  __always_inline
  contract_call_configured cfg(
      schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS, bool ihr_disabled = true) const {
    return contract_call_configured(addr_, amount, flags, ihr_disabled);
  }
  // Deploy message with function call (immediately after deploy)
  __always_inline
  proxy_deploy deploy(
      schema::StateInit init, schema::Grams amount, unsigned flags = DEFAULT_MSG_FLAGS,
      bool ihr_disabled = true) const {
    return proxy_deploy(addr_, init, amount, flags, ihr_disabled);
  }
  // Deploy message with func_id = 0 (deploying contract must support fallback)
  __always_inline
  void deploy_noop(schema::StateInit init, schema::Grams amount,
                   unsigned flags = DEFAULT_MSG_FLAGS, bool ihr_disabled = true) {
    contract_deploy_noop_impl(addr_, init, amount, flags, ihr_disabled);
  }
  __always_inline
  proxy operator()(schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS, bool ihr_disabled = true) const {
    return proxy(addr_, amount, flags, ihr_disabled);
  }
  __always_inline
  proxy_prepare prepare_internal(schema::Grams amount = 10000000, bool ihr_disabled = true) const {
    return proxy_prepare(addr_, amount, ihr_disabled);
  }
  __always_inline
  proxy_body body_internal() const {
    return proxy_body(addr_);
  }
  __always_inline
  proxy_prepare_ext prepare_external() const {
    return proxy_prepare_ext(addr_);
  }
  __always_inline
  proxy_prepare_ext_with_pubkey prepare_external_with_pubkey(schema::uint256 pubkey) const {
    return proxy_prepare_ext_with_pubkey(addr_, pubkey);
  }
  __always_inline
  proxy_debot_ext_in debot_ext_in(unsigned flags = DEFAULT_MSG_FLAGS) const {
    return proxy_debot_ext_in(addr_, flags);
  }
  __always_inline
  proxy_body_ext body_external() const {
    return proxy_body_ext(addr_);
  }
  __always_inline
  proxy_body_ext_with_pubkey body_external_with_pubkey(schema::uint256 pubkey) const {
    return proxy_body_ext_with_pubkey(addr_, pubkey);
  }
  __always_inline
  proxy_run_getter run() const {
    return proxy_run_getter(addr_);
  }
  template<class ReturnVal>
  __always_inline
  auto tail_call(address wait_addr, schema::Grams amount = 10000000,
                 unsigned flags = DEFAULT_MSG_FLAGS, bool ihr_disabled = true) const {
    return proxy_tail_call<ReturnVal>(addr_, wait_addr, amount, flags, ihr_disabled);
  }
  address get() const { return addr_; }

  address addr_;
};

template<class Interface>
using handle = contract_handle<Interface>;

/*
void example_usage(contract_handle<Interface> handle) {
  handle(Grams(10), SENDER_WANTS_TO_PAY_FEES_SEPARATELY).method(arg0, arg1, ...);
  handle.deploy(init, 10, SENDER_WANTS_TO_PAY_FEES_SEPARATELY).method(arg0, arg1, ...);
}
*/

} // namespace tvm

