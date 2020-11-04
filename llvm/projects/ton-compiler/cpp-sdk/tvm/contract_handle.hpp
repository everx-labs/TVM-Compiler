#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <tvm/chain_tuple.hpp>

#include <experimental/type_traits>
#include <experimental/coroutine>

#include <tvm/message_flags.hpp>
#include <tvm/globals.hpp>

namespace tvm {

template<auto Func>
struct get_func_rv {};
template<typename Cl, typename RetT, typename... Args, RetT (Cl::*Func)(Args...)>
struct get_func_rv<Func> {
  using type = RetT;
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
cell contract_call_prepare(address addr, schema::Grams amount, Args... args) {
  using namespace schema;

  auto hdr = prepare_internal_header<Func>();
  auto hdr_plus_args = std::make_tuple(hdr, args...);
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = true;
  out_info.bounce = true;
  out_info.bounced = false;
  out_info.src = addr_none{};
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

template<auto Func, class... Args>
__always_inline
cell external_call_prepare_with_pubkey(address addr, schema::uint256 pubkey, Args... args) {
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
void contract_call_impl(address addr, schema::Grams amount,
                        unsigned flags, Args... args) {
  tvm_sendmsg(contract_call_prepare<Func>(addr, amount, args...), flags);
}

template<auto Func, class... Args>
__always_inline
void contract_deploy_impl(address addr, schema::StateInit init,
                          schema::Grams amount, unsigned flags, Args... args) {
  using namespace schema;

  auto hdr = prepare_internal_header<Func>();
  auto hdr_plus_args = std::make_tuple(hdr, args...);
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = true;
  out_info.bounce = false;
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

template<class RetT>
struct wait_call_result {
  __always_inline
  bool await_ready() const { return false; }
  __always_inline
  void await_suspend(std::experimental::coroutine_handle<>) const {}
  __always_inline
  RetT await_resume() const {
    return schema::parse<RetT>(__builtin_tvm_cast_to_slice(
      temporary_data::getglob(global_id::coroutine_answer_slice)));
  }
};

template<typename RetT>
using awaitable_ret_t = std::conditional_t<std::is_void_v<RetT>, void, wait_call_result<RetT>>;

class contract_call_configured {
public:
  contract_call_configured(address addr, schema::Grams amount, unsigned flags)
    : addr_(addr), amount_(amount), flags_(flags) {}
  template<auto Func, class... Args>
  __always_inline void call(Args... args) const {
    contract_call_impl<Func>(addr_, amount_, flags_, args...);
  }
  template<auto Func, class... Args>
  __always_inline
  awaitable_ret_t<get_func_rv_t<Func>> _call_impl(Args... args) const {
    contract_call_impl<Func>(addr_, amount_, flags_, args...);
    if constexpr (!std::is_void_v<get_func_rv_t<Func>>) {
      temporary_data::setglob(2, __builtin_tvm_cast_from_slice(addr_.sl()));
      return {};
    }
  }
private:
  address addr_;
  schema::Grams amount_;
  unsigned flags_;
};

class contract_deploy_configured {
public:
  contract_deploy_configured(address addr, schema::StateInit init,
                             schema::Grams amount, unsigned flags)
    : addr_(addr), init_(init), amount_(amount), flags_(flags) {}
  template<auto Func, class... Args>
  __always_inline void call(Args... args) const {
    contract_deploy_impl<Func>(addr_, init_, amount_, flags_, args...);
  }
  template<auto Func, class... Args>
  __always_inline
  void _call_impl(Args... args) const {
    contract_deploy_impl<Func>(addr_, init_, amount_, flags_, args...);
  }
private:
  address addr_;
  schema::StateInit init_;
  schema::Grams amount_;
  unsigned flags_;
};

// class for preparing call message cell without sending it
class contract_call_prepare_only {
public:
  contract_call_prepare_only(address addr, schema::Grams amount)
    : addr_(addr), amount_(amount) {}
  template<auto Func, class... Args>
  __always_inline
  cell _call_impl(Args... args) const {
    return contract_call_prepare<Func>(addr_, amount_, args...);
  }
private:
  address addr_;
  schema::Grams amount_;
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
    return external_call_prepare_with_pubkey<Func>(addr_, pubkey_, args...);
  }
private:
  address addr_;
  schema::uint256 pubkey_;
};

template<class Interface>
class contract_handle {
public:
  using proxy = __reflect_proxy<Interface, contract_call_configured, true>;
  using proxy_deploy = __reflect_proxy<Interface, contract_deploy_configured, true>;
  using proxy_prepare = __reflect_proxy<Interface, contract_call_prepare_only, true>;
  using proxy_prepare_ext = __reflect_proxy<Interface, external_call_prepare_only, false>;
  using proxy_prepare_ext_with_pubkey = __reflect_proxy<Interface, external_call_prepare_only_with_pubkey, false>;

  contract_handle() {}
  contract_handle(address addr) : addr_(addr) {}

  template<auto Func, class... Args>
  __always_inline
  void call(schema::Grams amount, Args... args) {
    contract_call_impl<Func>(addr_, amount, DEFAULT_MSG_FLAGS, args...);
  }

  template<auto Func, class... Args>
  __always_inline
  void deploy_call(schema::StateInit init, schema::Grams amount, Args... args) {
    contract_deploy_impl<Func>(addr_, init, amount, DEFAULT_MSG_FLAGS, args...);
  }

  __always_inline
  contract_call_configured cfg(
      schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return contract_call_configured(addr_, amount, flags);
  }
  __always_inline
  proxy_deploy deploy(
      schema::StateInit init, schema::Grams amount, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return proxy_deploy(addr_, init, amount, flags);
  }
  __always_inline
  proxy operator()(schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return proxy(addr_, amount, flags);
  }
  __always_inline
  proxy_prepare prepare_internal(schema::Grams amount = 10000000) const {
    return proxy_prepare(addr_, amount);
  }
  __always_inline
  proxy_prepare_ext prepare_external() const {
    return proxy_prepare_ext(addr_);
  }
  __always_inline
  proxy_prepare_ext_with_pubkey prepare_external_with_pubkey(schema::uint256 pubkey) const {
    return proxy_prepare_ext(addr_, pubkey);
  }
  address get() const { return addr_; }

  address addr_;
};

template<class Interface>
using handle = contract_handle<Interface>;

/*
void example_usage(contract_handle<Interface> handle) {
  handle(Grams(10), SENDER_WANTS_TO_PAY_FEES_SEPARATELY).method(arg0, arg1, ...);
  handle.deploy(init, 10, SENDER_WANTS_TO_PAY_FEES_SEPARATELY).call<&Interface::method>(arg0, arg1, ...);
}
*/

} // namespace tvm

