#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/smart_contract_info.hpp>
#include <tvm/chain_builder.hpp>

#include <experimental/type_traits>

namespace tvm {

template<typename T>
using fallback_t = decltype(T::_fallback(cell{}, slice{}));
template<typename T>
constexpr bool supports_fallback_v = std::experimental::is_detected_v<fallback_t, T>;
template<typename T>
using on_bounced_t = decltype(T::_on_bounced(cell{}, slice{}));
template<typename T>
constexpr bool supports_on_bounced_v = std::experimental::is_detected_v<on_bounced_t, T>;
template<typename T>
using pubkey_t = decltype(&T::set_tvm_pubkey);
template<typename T>
constexpr bool supports_tvm_pubkey_v = std::experimental::is_detected_v<pubkey_t, T>;
template<typename T>
using persistent_data_hdr_t = decltype(&T::set_persistent_data_header);
template<typename T>
constexpr bool supports_set_persistent_data_header_v = std::experimental::is_detected_v<persistent_data_hdr_t, T>;

template<class ReturnValue>
inline void send_external_answer(unsigned func_id, ReturnValue rv) {
  using namespace schema;
  abiv1::external_outbound_msg_header hdr{ uint32(func_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  ext_out_msg_info_relaxed out_info;
  out_info.src = addr_none{};
  out_info.dest = ext_msg().unpack().ext_sender();
  out_info.created_lt = 0;
  out_info.created_at = 0;

  message_relaxed<decltype(hdr_plus_rv)> out_msg;
  out_msg.info = out_info;
  out_msg.body = hdr_plus_rv;
  tvm_sendmsg(build(out_msg).endc(), 0);
}

template<class ReturnValue>
inline void send_internal_answer(unsigned func_id, ReturnValue rv) {
  using namespace schema;
  abiv1::internal_msg_header hdr{ uint32(func_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  int_msg_info_relaxed out_info;
  out_info.src = addr_none{};
  out_info.dest = int_msg().unpack().int_sender();
  out_info.created_lt = 0;
  out_info.created_at = 0;

  message_relaxed<decltype(hdr_plus_rv)> out_msg;
  out_msg.info = out_info;
  out_msg.body = hdr_plus_rv;
  tvm_sendmsg(build(out_msg).endc(), 0);
}

struct no_replay_protection {
  using persistent_t = int;
};

template<class ReplayAttackProtection>
using persistent_data_header_t = typename std::conditional_t<
  std::is_void_v<ReplayAttackProtection>,
    no_replay_protection,
    ReplayAttackProtection>::persistent_t;

template<class ReplayAttackProtection, class DContract>
inline std::tuple<persistent_data_header_t<ReplayAttackProtection>, DContract> load_persistent_data() {
  using namespace schema;

  parser persist(persistent_data::get());
  bool uninitialized = persist.ldu(1);
  tvm_assert(!uninitialized, error_code::method_called_without_init);

  if constexpr (!std::is_void_v<ReplayAttackProtection>) {
    using HeaderT = typename ReplayAttackProtection::persistent_t;
    auto [data_hdr, =persist] = parse_continue<HeaderT>(persist);
    tvm_assert(!!data_hdr, error_code::persistent_data_parse_error);

    static_assert(get_bitsize_v<HeaderT> != 0,
      "Replay attack protection header can't be dynamic-size");
    // Only 1 + bitsize(Header) bits to skip
    DContract base = parse_chain<DContract, 1 + get_bitsize_v<HeaderT>>(persist);
    return { *data_hdr, base };
  } else {
    // Only 1 bit to skip
    DContract base = parse_chain<DContract, 1>(persist);
    return { 0, base };
  }
}

template<class Data, bool dyn_chain>
__always_inline Data parse_smart(parser p) {
  using namespace schema;

  if constexpr (dyn_chain) {
    return parse_chain_dynamic<Data>(p);
  } else {
    return parse<Data>(p);
  }
}

template<class ReplayAttackProtection, class DContract>
inline cell prepare_persistent_data(persistent_data_header_t<ReplayAttackProtection> persistent_data_header,
                                    DContract base) {
  using namespace schema;
  auto data_tup = to_std_tuple(base);
  // First add uninitialized bit = false in tuple
  // Also add replay attack protection header if needed
  // Then store data_tup_combined into chain of cells
  if constexpr (!std::is_void_v<ReplayAttackProtection>) {
    auto data_tup_combined = std::tuple_cat(std::make_tuple(bool_t(false), persistent_data_header), data_tup);
    return build_chain(data_tup_combined).make_cell();
  } else {
    auto data_tup_combined = std::tuple_cat(std::make_tuple(bool_t(false)), data_tup);
    return build_chain(data_tup_combined).make_cell();
  }
}

template<class ReplayAttackProtection, class DContract>
inline void save_persistent_data(persistent_data_header_t<ReplayAttackProtection> persistent_data_header,
                                 DContract base) {
  persistent_data::set(prepare_persistent_data<ReplayAttackProtection, DContract>(persistent_data_header, base));
}

template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index, unsigned RestMethods>
struct smart_switcher_impl {
  static const unsigned my_method_id = get_interface_method_func_id<IContract, Index>::value;
  __always_inline static int execute(unsigned func_id, cell msg, slice msg_body) {
    constexpr bool is_getter = get_interface_method_getter<IContract, Index>::value;
    constexpr bool is_noaccept = get_interface_method_noaccept<IContract, Index>::value;
    constexpr bool is_internal = get_interface_method_internal<IContract, Index>::value;
    constexpr bool is_external = get_interface_method_external<IContract, Index>::value;
    constexpr bool is_dyn_chain_parse = get_interface_method_dyn_chain_parse<IContract, Index>::value;
    // getters should be inserted at main_external switch
    constexpr bool valid = (Internal && is_internal) ||
                           (!Internal && (is_external || is_getter));
    using namespace schema;
    if (valid && (func_id == my_method_id)) {
      // We need to parse persistent data for contract, parse arguments for method,
      // call method, build return value and send message, build updated persistent data and store
      Contract c;
      using ISmart = typename Contract::base;
      constexpr auto func = get_interface_method_ptr<Contract, ISmart, Index>::value;

      // Internal methods and getters are not signed
      if constexpr (!Internal && !is_getter) {
        auto [pubkey, =msg_body] = signature_checker<error_code::invalid_signature>::check(msg_body);
        if constexpr (supports_tvm_pubkey_v<Contract>)
          c.set_tvm_pubkey(pubkey);
      }

      persistent_data_header_t<ReplayAttackProtection> persistent_data_header;

      if constexpr (!get_interface_method_no_read_persistent<IContract, Index>::value) {
        parser persist(persistent_data::get());
        bool uninitialized = persist.ldu(1);
        // Load persistent data (not for constructor)
        if constexpr (my_method_id != id_v<&IContract::constructor>) {
          tvm_assert(!uninitialized, error_code::method_called_without_init);
          auto &base = static_cast<DContract&>(c);
          // Load replay attack protection header
          if constexpr (!std::is_void_v<ReplayAttackProtection>) {
            using HeaderT = typename ReplayAttackProtection::persistent_t;
            auto [data_hdr, new_parser] = parse_continue<HeaderT>(persist);
            tvm_assert(!!data_hdr, error_code::persistent_data_parse_error);
            persist = new_parser;
            persistent_data_header = *data_hdr;

            static_assert(get_bitsize_v<HeaderT> != 0,
              "Replay attack protection header can't be dynamic-size");
            // Only 1 + bitsize(Header) bits to skip
            base = parse_chain<DContract, 1 + get_bitsize_v<HeaderT>>(persist);
          } else {
            // Only 1 bit to skip
            base = parse_chain<DContract, 1>(persist);
          }
        } else {
          tvm_assert(uninitialized, error_code::constructor_double_call);
        }
      }

      using MsgHeaderT = std::conditional_t<Internal,
                                            abiv1::internal_msg_header,
                                            abiv1::external_inbound_msg_header>;
      parser body_p(msg_body);
      auto [in_hdr, new_parser] = parse_continue<MsgHeaderT>(body_p);
      body_p = new_parser;
      tvm_assert(!!in_hdr, error_code::bad_arguments);

      // If ReplayAttackProtection is specified, we need to perform check
      //   for external & non-constructor methods.
      // And perform ReplayAttackProtection initialization for constructor calls
      if constexpr (!std::is_void_v<ReplayAttackProtection> || !is_getter) {
        if constexpr (my_method_id != id_v<&IContract::constructor>) {
          if constexpr (!Internal) {
            auto verified = ReplayAttackProtection::check(*in_hdr, persistent_data_header);
            tvm_assert(!!verified, error_code::replay_attack_check);
            persistent_data_header = *verified;
          }
        } else {
          persistent_data_header = ReplayAttackProtection::init();
        }
      }
      // We don't need accept for getter methods
      if constexpr (!is_getter && !is_noaccept)
        tvm_accept();

      if constexpr (!get_interface_method_no_write_persistent<IContract, Index>::value && !is_getter) {
        if constexpr (supports_set_persistent_data_header_v<Contract>)
          c.set_persistent_data_header(persistent_data_header);
      }

      using Args = get_interface_method_arg_struct<IContract, Index>;
      constexpr unsigned args_sz = calc_fields_count<Args>::value;
      using rv_t = get_interface_method_rv<ISmart, Index>;

      // Execute and send answer if non-void return value
      if constexpr (!std::is_void_v<rv_t>) {
        rv_t rv;
        if constexpr (args_sz != 0) {
          auto parsed_args = parse_smart<Args, is_dyn_chain_parse>(body_p);
          rv = std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                                 to_std_tuple(parsed_args)));
        } else {
          rv = (c.*func)();
        }
        if constexpr (Internal)
          send_internal_answer(in_hdr->function_id(), rv);
        else
          send_external_answer(in_hdr->function_id(), rv);
      } else {
        // void return (no need to send an answer message)
        if constexpr (args_sz != 0) {
          auto parsed_args = parse_smart<Args, is_dyn_chain_parse>(body_p);
          std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                            to_std_tuple(parsed_args)));
        } else {
          (c.*func)();
        }
      }

      // Store persistent data
      if constexpr (!get_interface_method_no_write_persistent<IContract, Index>::value && !is_getter) {
        auto &base = static_cast<DContract&>(c);
        save_persistent_data<ReplayAttackProtection>(persistent_data_header, base);
      }
      return my_method_id;
    } else {
      return smart_switcher_impl<Internal, Contract, IContract, DContract, ReplayAttackProtection,
                                 Index + 1, RestMethods - 1>::execute(func_id, msg, msg_body);
    }
  }
};
template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index>
struct smart_switcher_impl<Internal, Contract, IContract, DContract, ReplayAttackProtection, Index, 0> {
  __always_inline static int execute(unsigned func_id, cell msg, slice msg_body) {
    if constexpr (Internal && supports_fallback_v<Contract>)
      return Contract::_fallback(msg, msg_body);
    else
      tvm_throw(error_code::wrong_public_call);
    return 0;
  }
};

template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection>
struct smart_switcher {
  static const unsigned methods_count = get_interface_methods_count<IContract>::value;
  static int execute(unsigned func_id, cell msg, slice msg_body) {
    set_msg(msg); // msg cell is set into global value and will be parsed at first request
    return smart_switcher_impl<Internal, Contract, IContract, DContract, ReplayAttackProtection,
                               0, methods_count>::execute(func_id, msg, msg_body);
  }
};

__always_inline bool is_bounced(cell msg) {
  parser p(msg.ctos());
  require(p.ldu(1) == schema::int_msg_info{}.kind.code, error_code::bad_incoming_msg);
  p.skip(2); // ihr_disabled + bounce
  return p.ldu(1) != 0; // bounced != 0
}

template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection = void>
int smart_switch(cell msg, slice msg_body) {
  if constexpr (Internal) {
    if constexpr (supports_on_bounced_v<Contract>) {
      if (is_bounced(msg))
        return Contract::_on_bounced(msg, msg_body);
    } else {
      require(!is_bounced(msg), error_code::unsupported_bounced_msg);
    }
  }

  parser msg_parser(msg_body);
  unsigned func_id;
  if constexpr (Internal && supports_fallback_v<Contract>) {
    if (auto opt_val = msg_parser.lduq(32))
      func_id = *opt_val;
    else
      return Contract::_fallback(msg, msg_body);
    if (func_id == 0)
      return Contract::_fallback(msg, msg_body);
  } else {
    func_id = msg_parser.ldu(32);
  }
  return smart_switcher<Internal, Contract, IContract, DContract, ReplayAttackProtection>
    ::execute(func_id, msg, msg_body);
}

} // namespace tvm

