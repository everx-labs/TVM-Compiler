#pragma once

#include <tvm/reflection.hpp>
#include <tvm/contract.hpp>
#include <tvm/smart_contract_info.hpp>

namespace tvm {

template<class Contract, class IContract, class DContract, unsigned Index, unsigned RestMethods>
struct smart_switcher_impl {
  static const unsigned my_method_id = get_interface_method_func_id<IContract, Index>::value;
  static int execute(unsigned func_id, cell msg, slice msg_body) {
    if (func_id == my_method_id) {
      // We need to parse persistent data for contract, parse arguments for method,
      // call method, build return value and send message, build updated persistent data and store
      Contract c;
      using ISmart = typename Contract::base;
      constexpr auto func = get_interface_method_ptr<Contract, ISmart, Index>::value;

      check_signature(msg_body, error_code::bad_signature);

      // Load persistent data (not for constructor)
      if constexpr (my_method_id != id_v<&IContract::constructor>) {
        auto cl = persistent_data::get();
        auto &base = static_cast<DContract&>(c);
        base = schema::parse<DContract>(parser(cl), error_code::persistent_data_parse_error, true);
      } else {
        // TODO: verify that we have uninitialized state in persistent data
      }
      
      parser body_p(msg_body);
      auto [in_hdr, new_parser] =
        schema::parse_continue<schema::abiv1::external_inbound_msg_header>(body_p);
      tvm_assert(!!in_hdr, error_code::bad_arguments);
      using Args = get_interface_method_arg_struct<IContract, Index>;
      constexpr unsigned args_sz = calc_fields_count<Args>::value;
      
      using rv_t = get_interface_method_rv<ISmart, Index>;

      // Execute and send answer if non-void return value
      if constexpr (!std::is_void_v<rv_t>) {
        rv_t rv;
        if constexpr (args_sz != 0) {
          auto parsed_args = schema::parse<Args>(new_parser, error_code::bad_arguments, true);
          rv = std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                                 to_std_tuple(parsed_args)));
        } else {
          rv = (c.*func)();
        }

        schema::abiv1::external_outbound_msg_header
          hdr{ schema::abiv1::answer_id(in_hdr->function_id()) };
        auto hdr_plus_rv = std::make_tuple(hdr, rv);

        schema::ext_out_msg_info_relaxed out_info;
        out_info.src = schema::addr_none{};
        out_info.dest = get_incoming_ext_src(msg);
        out_info.created_lt = 0;
        out_info.created_at = 0;

        schema::message_relaxed<decltype(hdr_plus_rv)> out_msg;
        out_msg.info = out_info;
        out_msg.body = hdr_plus_rv;
        tvm_sendmsg(build(out_msg).endc(), 0);
      } else {
        if constexpr (args_sz != 0) {
          auto parsed_args = schema::parse<Args>(new_parser, error_code::bad_arguments, true);
          std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                            to_std_tuple(parsed_args)));
        } else {
          (c.*func)();
        }
      }
      
      // Store persistent data
      {
        auto &base = static_cast<DContract&>(c);
        persistent_data::set(schema::build(base).make_cell());
      }
      return my_method_id;
    } else {
      return smart_switcher_impl<Contract, IContract, DContract, Index + 1, RestMethods - 1>
          ::execute(func_id, msg, msg_body);
    }
  }
};
template<class Contract, class IContract, class DContract, unsigned Index>
struct smart_switcher_impl<Contract, IContract, DContract, Index, 0> {
  static int execute(unsigned func_id, cell msg, slice msg_body) {
    return 0;
  }
};

template<class Contract, class IContract, class DContract>
struct smart_switcher {
  static const unsigned methods_count = get_interface_methods_count<IContract>::value;
  static int execute(unsigned func_id, cell msg, slice msg_body) {
    set_msg(msg); // msg cell is set into global value and will be parsed at first request
    return smart_switcher_impl<Contract, IContract, DContract, 0, methods_count>
      ::execute(func_id, msg, msg_body);
  }
};
template<class Contract, class IContract, class DContract>
int smart_switch(unsigned func_id, cell msg, slice msg_body) {
  return smart_switcher<Contract, IContract, DContract>::execute(func_id, msg, msg_body);
}

} // namespace tvm

