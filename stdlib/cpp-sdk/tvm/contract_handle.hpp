#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>

#include <experimental/type_traits>

namespace tvm {

template<class Interface, class Func, class... Args>
using good_call_args_t = decltype( (((Interface*)nullptr)->*(Func::value))(Args{}...) );
template<class Interface, auto Func, class... Args>
constexpr bool good_call_args_v = std::experimental::is_detected_v<good_call_args_t, Interface, proxy_method<Func>, Args...>;

template<class Interface>
class contract_handle {
public:
  explicit contract_handle(schema::lazy<schema::MsgAddressInt> addr) : addr_(addr) {}

  template<auto Func, class... Args>
  void call(schema::Grams amount, Args... args) {
    static_assert(good_call_args_v<Interface, Func, Args...>, "Wrong contract handle call arguments");

    using namespace schema;

    abiv1::internal_msg_header hdr{ uint32(id_v<Func>) };
    auto hdr_plus_args = std::make_tuple(hdr, args...);
    int_msg_info_relaxed out_info;
    out_info.src = addr_none{};
    out_info.dest = addr_;
    out_info.created_lt = 0;
    out_info.created_at = 0;
    out_info.value.grams = amount;

    message_relaxed<decltype(hdr_plus_args)> out_msg;
    out_msg.info = out_info;
    out_msg.body = hdr_plus_args;
    tvm_sendmsg(build(out_msg).endc(), 0);
  }
private:
  schema::lazy<schema::MsgAddressInt> addr_;
};

} // namespace tvm

