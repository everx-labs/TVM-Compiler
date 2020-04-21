#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>

#include <experimental/type_traits>

namespace tvm {

template<class Interface, class Func, class... Args>
using good_call_args_t = decltype( (((Interface*)nullptr)->*(Func::value))(Args{}...) );
template<class Interface, auto Func, class... Args>
constexpr bool good_call_args_v = std::experimental::is_detected_v<good_call_args_t, Interface, proxy_method<Func>, Args...>;

constexpr unsigned SEND_ALL_GAS = 128;
constexpr unsigned SEND_REST_GAS_FROM_INCOMING = 64;
constexpr unsigned DELETE_ME_IF_I_AM_EMPTY = 32;
constexpr unsigned IGNORE_ACTION = 2;
constexpr unsigned SENDER_WANTS_TO_PAY_FEES_SEPARATELY = 1;

constexpr unsigned DEFAULT_MSG_FLAGS = SENDER_WANTS_TO_PAY_FEES_SEPARATELY | IGNORE_ACTION;

template<class Interface, auto Func, class... Args>
inline void contract_call_impl(schema::lazy<schema::MsgAddressInt> addr, schema::Grams amount,
                               unsigned flags, Args... args) {
  static_assert(good_call_args_v<Interface, Func, Args...>, "Wrong contract handle call arguments");

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

  message_relaxed<decltype(hdr_plus_args)> out_msg;
  out_msg.info = out_info;
  out_msg.body = hdr_plus_args;
  tvm_sendmsg(build(out_msg).endc(), flags);
}

template<class Interface, auto Func, class... Args>
inline void contract_deploy_impl(schema::lazy<schema::MsgAddressInt> addr, schema::StateInit init,
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

  message_relaxed<decltype(hdr_plus_args)> out_msg;
  out_msg.info = out_info;
  Either<StateInit, ref<StateInit>> init_ref = ref<StateInit>{init};
  out_msg.init = init_ref;
  out_msg.body = hdr_plus_args;
  tvm_sendmsg(build(out_msg).endc(), flags);
}

template<class Interface>
class contract_call_configured {
public:
  contract_call_configured(schema::lazy<schema::MsgAddressInt> addr, schema::Grams amount, unsigned flags)
    : addr_(addr), amount_(amount), flags_(flags) {}
  template<auto Func, class... Args>
  inline void call(Args... args) const {
    contract_call_impl<Interface, Func>(addr_, amount_, flags_, args...);
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
  inline void call(Args... args) const { contract_deploy_impl(addr_, init_, amount_, flags_, args...); }
private:
  schema::lazy<schema::MsgAddressInt> addr_;
  schema::StateInit init_;
  schema::Grams amount_;
  unsigned flags_;
};

template<class Interface>
class contract_handle {
public:
  explicit contract_handle(schema::lazy<schema::MsgAddressInt> addr) : addr_(addr) {}

  template<auto Func, class... Args>
  void call(schema::Grams amount, Args... args) {
    contract_call_impl<Interface, Func>(addr_, amount, DEFAULT_MSG_FLAGS, args...);
  }

  template<auto Func, class... Args>
  void deploy_call(schema::StateInit init, schema::Grams amount, Args... args) {
    contract_deploy_impl<Interface, Func>(addr_, init, amount, DEFAULT_MSG_FLAGS, args...);
  }

  contract_call_configured<Interface> operator()(
      schema::Grams amount = 10000000, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return contract_call_configured<Interface>(addr_, amount, flags);
  }
  contract_deploy_configured<Interface> deploy(
      schema::StateInit init, schema::Grams amount, unsigned flags = DEFAULT_MSG_FLAGS) const {
    return contract_deploy_configured<Interface>(addr_, init, amount, flags);
  }
private:
  schema::lazy<schema::MsgAddressInt> addr_;
};

/*
void example_usage(contract_handle<Interface> handle) {
  handle(10, SENDER_WANTS_TO_PAY_FEES_SEPARATELY).call<&Interface::method>(arg0, arg1, ...);
  handle.deploy(init, 10, SENDER_WANTS_TO_PAY_FEES_SEPARATELY).call<&Interface::method>(arg0, arg1, ...);
}
*/

} // namespace tvm

