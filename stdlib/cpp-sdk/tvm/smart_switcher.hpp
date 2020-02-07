#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/smart_contract_info.hpp>

namespace tvm {

template<class ReturnValue>
inline void send_external_answer(unsigned func_id, ReturnValue rv) {
  schema::abiv1::external_outbound_msg_header hdr{ schema::abiv1::answer_id(func_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  schema::ext_out_msg_info_relaxed out_info;
  out_info.src = schema::addr_none{};
  out_info.dest = ext_msg().unpack().ext_sender();
  out_info.created_lt = 0;
  out_info.created_at = 0;

  schema::message_relaxed<decltype(hdr_plus_rv)> out_msg;
  out_msg.info = out_info;
  out_msg.body = hdr_plus_rv;
  tvm_sendmsg(build(out_msg).endc(), 0);
}

template<class ReturnValue>
inline void send_internal_answer(unsigned func_id, ReturnValue rv) {
  schema::abiv1::internal_msg_header hdr{ schema::abiv1::answer_id(func_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  schema::int_msg_info_relaxed out_info;
  out_info.src = schema::addr_none{};
  out_info.dest = int_msg().unpack().int_sender();
  out_info.created_lt = 0;
  out_info.created_at = 0;

  schema::message_relaxed<decltype(hdr_plus_rv)> out_msg;
  out_msg.info = out_info;
  out_msg.body = hdr_plus_rv;
  tvm_sendmsg(build(out_msg).endc(), 0);
}

template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index, unsigned RestMethods>
struct smart_switcher_impl {
  static const unsigned my_method_id = get_interface_method_func_id<IContract, Index>::value;
  __always_inline static int execute(unsigned func_id, cell msg, slice msg_body) {
    constexpr bool valid = (Internal && get_interface_method_internal<IContract, Index>::value) ||
                           (!Internal && get_interface_method_external<IContract, Index>::value);
    using namespace schema;
    if (valid && (func_id == my_method_id)) {
      // We need to parse persistent data for contract, parse arguments for method,
      // call method, build return value and send message, build updated persistent data and store
      Contract c;
      using ISmart = typename Contract::base;
      constexpr auto func = get_interface_method_ptr<Contract, ISmart, Index>::value;

      check_signature(msg_body, error_code::bad_signature);
      struct no_replay_protection {
        using persistent_t = int;
      };

      using persistent_data_header_t = typename std::conditional_t<
        std::is_void_v<ReplayAttackProtection>,
          no_replay_protection,
          ReplayAttackProtection>::persistent_t;

      persistent_data_header_t persistent_data_header;

      {
        parser persist(persistent_data::get());
        bool uninitialized = persist.ldu(1);
        // Load persistent data (not for constructor)
        if constexpr (my_method_id != id_v<&IContract::constructor>) {
          tvm_assert(!uninitialized, error_code::method_called_without_init);
          // Load replay attack protection header
          if constexpr (!std::is_void_v<ReplayAttackProtection>) {
            using HeaderT = typename ReplayAttackProtection::persistent_t;
            auto [data_hdr, new_parser] = parse_continue<HeaderT>(persist);
            tvm_assert(!!data_hdr, error_code::persistent_data_parse_error);
            persist = new_parser;
            persistent_data_header = *data_hdr;
          }

          auto &base = static_cast<DContract&>(c);
          base = parse<DContract>(persist, error_code::persistent_data_parse_error, true);
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
      if constexpr (!std::is_void_v<ReplayAttackProtection>) {
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
      tvm_accept();

      using Args = get_interface_method_arg_struct<IContract, Index>;
      constexpr unsigned args_sz = calc_fields_count<Args>::value;
      using rv_t = get_interface_method_rv<ISmart, Index>;

      // Execute and send answer if non-void return value
      if constexpr (!std::is_void_v<rv_t>) {
        rv_t rv;
        if constexpr (args_sz != 0) {
          auto parsed_args = schema::parse<Args>(body_p, error_code::bad_arguments, true);
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
          auto parsed_args = parse<Args>(body_p, error_code::bad_arguments, true);
          std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                            to_std_tuple(parsed_args)));
        } else {
          (c.*func)();
        }
      }

      // Store persistent data
      {
        builder b;
        // first store uninitialized bit = false
        b.stu(0, 1);
        // Store replay attack protection header
        if constexpr (!std::is_void_v<ReplayAttackProtection>)
          b = build(b, persistent_data_header);
        // then storing contract data
        auto &base = static_cast<DContract&>(c);
        persistent_data::set(schema::build(b, base).make_cell());
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
template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection = void>
int smart_switch(cell msg, slice msg_body) {
  parser msg_parser(msg_body);
  auto func_id = msg_parser.ldu(32);
  return smart_switcher<Internal, Contract, IContract, DContract, ReplayAttackProtection>
    ::execute(func_id, msg, msg_body);
}

} // namespace tvm

