#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <tvm/chain_tuple.hpp>

#include <experimental/type_traits>
#include <experimental/coroutine>

#include <tvm/message_flags.hpp>

namespace tvm {

template<class Interface, class Func, class... Args>
using good_call_args_t = decltype( (((Interface*)nullptr)->*(Func::value))(Args{}...) );
template<class Interface, auto Func, class... Args>
constexpr bool good_call_args_v = std::experimental::is_detected_v<good_call_args_t, Interface, proxy_method<Func>, Args...>;

template<auto Func, class... Args>
__always_inline
void contract_call_impl(schema::lazy<schema::MsgAddressInt> addr, schema::Grams amount,
                        unsigned flags, Args... args) {
  using namespace schema;

  abiv1::internal_msg_header hdr{ uint32(id_v<Func>) };
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
    tvm_sendmsg(build(out_msg).endc(), flags);
  } else {
    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = out_info;
    out_msg.body = hdr_plus_args;
    tvm_sendmsg(build(out_msg).endc(), flags);
  }
}

template<class Interface, auto Func, class... Args>
__always_inline
void contract_deploy_impl(schema::lazy<schema::MsgAddressInt> addr, schema::StateInit init,
                          schema::Grams amount, unsigned flags, Args... args) {
  static_assert(good_call_args_v<Interface, Func, Args...>, "Wrong contract handle call arguments");

  using namespace schema;

  abiv1::internal_msg_header hdr{ uint32(id_v<Func>) };
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
    return schema::parse<RetT>(__builtin_tvm_cast_to_slice(temporary_data::getglob(3)));
  }
};

template<auto Func>
struct get_func_rv {};
template<typename Cl, typename RetT, typename... Args, RetT (Cl::*Func)(Args...)>
struct get_func_rv<Func> {
  using type = RetT;
};
template<auto Func>
using get_func_rv_t = typename get_func_rv<Func>::type;

template<typename RetT>
using awaitable_ret_t = std::conditional_t<std::is_void_v<RetT>, void, wait_call_result<RetT>>;

class contract_call_configured {
public:
  contract_call_configured(schema::lazy<schema::MsgAddressInt> addr, schema::Grams amount, unsigned flags)
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
  schema::lazy<schema::MsgAddressInt> addr_;
  schema::Grams amount_;
  unsigned flags_;
};

template<class Interface>
class contract_deploy_configured {
public:
  contract_deploy_configured(schema::lazy<schema::MsgAddressInt> addr, schema::StateInit init,
                             schema::Grams amount, unsigned flags)
    : addr_(addr), init_(init), amount_(amount), flags_(flags) {}
  template<auto Func, class... Args>
  __always_inline void call(Args... args) const {
    contract_deploy_impl<Interface, Func>(addr_, init_, amount_, flags_, args...);
  }
private:
  schema::lazy<schema::MsgAddressInt> addr_;
  schema::StateInit init_;
  schema::Grams amount_;
  unsigned flags_;
};

template<class Interface>
class contract_handle {
public:
  using proxy = __reflect_proxy<Interface, contract_call_configured>;

  explicit contract_handle(schema::lazy<schema::MsgAddressInt> addr) : addr_(addr) {}

  template<auto Func, class... Args>
  __always_inline
  void call(schema::Grams amount, Args... args) {
    contract_call_impl<Func>(addr_, amount, DEFAULT_MSG_FLAGS, args...);
  }

  template<auto Func, class... Args>
  __always_inline
  void deploy_call(schema::StateInit init, schema::Grams amount, Args... args) {
    contract_deploy_impl<Interface, Func>(addr_, init, amount, DEFAULT_MSG_FLAGS, args...);
  }

  __always_inline
  contract_call_configured cfg(
      schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return contract_call_configured(addr_, amount, flags);
  }
  __always_inline
  contract_deploy_configured<Interface> deploy(
      schema::StateInit init, schema::Grams amount, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return contract_deploy_configured<Interface>(addr_, init, amount, flags);
  }

  proxy operator()(schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return proxy(addr_, amount, flags);
  }
private:
  schema::lazy<schema::MsgAddressInt> addr_;
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

