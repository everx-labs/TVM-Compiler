#pragma once

#include <tvm/reflection.hpp>
#include <tvm/func_id.hpp>
#include <tvm/contract.hpp>
#include <tvm/smart_contract_info.hpp>
#include <tvm/schema/chain_builder.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/schema/chain_tuple_printer.hpp>
#include <tvm/schema/chain_fold.hpp>
#include <tvm/message_flags.hpp>
#include <tvm/awaiting_responses_map.hpp>
#include <tvm/resumable.hpp>
#include <tvm/globals.hpp>

#include <experimental/type_traits>
#include <tvm/schema/abiv2.hpp>

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
using pubkey_t = decltype(&T::set_msg_pubkey);
template<typename T>
constexpr bool supports_msg_pubkey_v = std::experimental::is_detected_v<pubkey_t, T>;
template<typename T>
using persistent_data_hdr_t = decltype(&T::set_persistent_data_header);
template<typename T>
constexpr bool supports_set_persistent_data_header_v = std::experimental::is_detected_v<persistent_data_hdr_t, T>;

// Check that a contract has field to store incoming msg slice
template<typename T>
using msg_slice_t = decltype(T{}.set_msg_slice(slice{}));
template<typename T>
constexpr bool supports_msg_slice_v = std::experimental::is_detected_v<msg_slice_t, T>;
// Check that contract have field to store parsed internal sender from incoming message
template<typename T>
using int_sender_t = decltype(&T::int_sender);
template<typename T>
constexpr bool supports_int_sender_v = std::experimental::is_detected_v<int_sender_t, T>;
// Check that contract have field to specify return message flag
template<typename T>
using int_return_flag_t = decltype(&T::int_return_flag);
template<typename T>
constexpr bool supports_int_return_flag_v = std::experimental::is_detected_v<int_return_flag_t, T>;
// Check that contract have field to specify return message value
template<typename T>
using int_return_value_t = decltype(&T::int_return_value);
template<typename T>
constexpr bool supports_int_return_value_v = std::experimental::is_detected_v<int_return_value_t, T>;
template<typename T>
using return_func_id_t = decltype(&T::return_func_id);
template<typename T>
constexpr bool supports_return_func_id_v = std::experimental::is_detected_v<return_func_id_t, T>;
template<typename T>
using suicide_addr_t = decltype(&T::suicide_addr);
template<typename T>
constexpr bool supports_suicide_addr_v = std::experimental::is_detected_v<suicide_addr_t, T>;
template<typename T>
using constructor_t = decltype(&T::constructor);
template<typename T>
constexpr bool has_constructor_v = std::experimental::is_detected_v<constructor_t, T>;

template<class Interface, unsigned my_method_id>
constexpr bool is_method_constructor() {
  if constexpr (has_constructor_v<Interface>)
    return my_method_id == id_v<&Interface::constructor>;
  else
    return false;
}

// Check that contract have field to store parsed external sender from incoming message
template<typename T>
using ext_sender_t = decltype(&T::ext_sender);
template<typename T>
constexpr bool supports_ext_sender_v = std::experimental::is_detected_v<ext_sender_t, T>;

template<typename T>
struct is_resumable : std::false_type {};
template<typename T>
struct is_resumable<schema::resumable<T>> : std::true_type {};
template<typename T>
constexpr bool is_resumable_v = is_resumable<T>::value;

template<typename T>
struct mem_fn_ret {};
template<typename Rv, typename T, typename... Args>
struct mem_fn_ret<Rv (T::*)(Args...)> {
  using type = Rv;
};

template<auto Func>
constexpr bool is_coroutine_v = is_resumable_v<typename mem_fn_ret<decltype(Func)>::type>;

template<class Interface, unsigned Index, unsigned RestMethods>
struct interface_has_coroutines_impl {
  using RetType = get_interface_method_rv<Interface, Index>;
  static constexpr bool value = is_resumable_v<RetType> ||
    interface_has_coroutines_impl<Interface, Index + 1, RestMethods - 1>::value;
};
template<class Interface, unsigned Index>
struct interface_has_coroutines_impl<Interface, Index, 0> : std::false_type {};
template<class Interface>
constexpr bool interface_has_coroutines_v =
  interface_has_coroutines_impl<Interface, 0,
    get_interface_methods_count<Interface>::value>::value;

template<class Contract>
__always_inline schema::lazy<schema::MsgAddressExt> ext_return_address(Contract& c) {
  if constexpr (supports_ext_sender_v<Contract>)
    return c.ext_sender();
  else
    return ext_msg().unpack().ext_sender();
}

template<bool ImplicitFuncId, class Contract, class ReturnValue>
__always_inline void send_external_answer(Contract& c, unsigned func_id, ReturnValue rv) {
  // TODO: re-think in design, answer_id should have (1 << 31) bit set for implicit func_id,
  // and should not be changed for explicit func_id
  unsigned answer_id = ImplicitFuncId ? schema::abiv2::answer_id(func_id) : func_id;
  using namespace schema;
  abiv2::external_outbound_msg_header hdr{ uint32(answer_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  ext_out_msg_info_relaxed out_info;
  out_info.src = addr_none{};
  out_info.dest = ext_return_address(c);
  out_info.created_lt = 0;
  out_info.created_at = 0;

  using est_t = estimate_element<message<decltype(hdr_plus_rv)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_rv);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = out_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    tvm_sendmsg(build(out_msg).endc(), 0);
  } else {
    message_relaxed<decltype(hdr_plus_rv)> out_msg;
    out_msg.info = out_info;
    out_msg.body = hdr_plus_rv;
    tvm_sendmsg(build(out_msg).endc(), 0);
  }
}

template<class Contract>
__always_inline schema::lazy<schema::MsgAddressInt> int_return_address(Contract& c) {
  if constexpr (supports_int_sender_v<Contract>)
    return c.int_sender();
  else
    return int_msg().unpack().int_sender();
}

template<class Contract>
__always_inline void set_int_return_address(Contract& c, schema::lazy<schema::MsgAddressInt> addr) {
  static_assert(supports_int_sender_v<Contract>);
  c.set_int_sender(addr);
}

template<class Contract>
__always_inline void set_ext_return_address(Contract& c, schema::lazy<schema::MsgAddressExt> addr) {
  static_assert(supports_ext_sender_v<Contract>);
  c.set_ext_sender(addr);
}

template<class Contract>
__always_inline unsigned int_return_flag(Contract& c) {
  if constexpr (supports_int_return_flag_v<Contract>)
    return c.int_return_flag();
  else
    return DEFAULT_MSG_FLAGS;
}

template<class Contract>
__always_inline unsigned int_return_value(Contract& c) {
  if constexpr (supports_int_return_value_v<Contract>)
    return c.int_return_value();
  else
    return 0;
}

template<class Contract>
__always_inline schema::uint32 return_func_id(Contract& c, unsigned func_id) {
  using namespace schema;
  if constexpr (supports_return_func_id_v<Contract>)
    if (auto v = c.return_func_id())
      return *v;
  return uint32(abiv2::answer_id(func_id));
}

template<class Contract, class ReturnValue>
__always_inline void send_internal_answer(Contract& c, unsigned func_id, ReturnValue rv) {
  using namespace schema;
  abiv2::internal_msg_header hdr{ return_func_id(c, func_id) };
  auto hdr_plus_rv = std::make_tuple(hdr, rv);
  int_msg_info_relaxed out_info;
  out_info.ihr_disabled = true;
  out_info.bounce = true;
  out_info.src = addr_none{};
  out_info.dest = int_return_address(c);
  out_info.created_lt = 0;
  out_info.created_at = 0;
  out_info.value.grams = int_return_value(c);

  using est_t = estimate_element<message<decltype(hdr_plus_rv)>>;
  if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
    auto chain_tup = make_chain_tuple(hdr_plus_rv);
    message_relaxed<decltype(chain_tup)> out_msg;
    out_msg.info = out_info;
    out_msg.body = ref<decltype(chain_tup)>{chain_tup};
    tvm_sendmsg(build(out_msg).endc(), int_return_flag(c));
  } else {
    message_relaxed<decltype(hdr_plus_rv)> out_msg;
    out_msg.info = out_info;
    out_msg.body = hdr_plus_rv;
    tvm_sendmsg(build(out_msg).endc(), int_return_flag(c));
  }
}

template<bool Internal, bool ImplicitFuncId, class Contract, class ReturnValue>
__always_inline void send_answer(Contract& c, unsigned func_id, ReturnValue rv) {
  if constexpr (Internal)
    send_internal_answer(c, func_id, rv);
  else
    send_external_answer<ImplicitFuncId>(c, func_id, rv);
}

struct no_replay_protection {
  using persistent_t = schema::empty;
};
template<class Interface, class ReplayAttackProtection>
struct persistent_header_info {
  using replay_hdr = typename std::conditional_t<
    std::is_void_v<ReplayAttackProtection>,
      no_replay_protection,
      ReplayAttackProtection>::persistent_t;

  using common_hdr =
    std::conditional_t<interface_has_coroutines_v<Interface>,
        std::tuple<replay_hdr, awaiting_responses_map>,
        replay_hdr>;

   static constexpr bool non_empty = !std::is_void_v<ReplayAttackProtection> ||
                                     interface_has_coroutines_v<Interface>;
};
template<class Interface, class ReplayAttackProtection>
using persistent_data_header_t = typename persistent_header_info<Interface, ReplayAttackProtection>::common_hdr;

template<class Interface, class ReplayAttackProtection, class DContract>
inline std::tuple<persistent_data_header_t<Interface, ReplayAttackProtection>, DContract> load_persistent_data() {
  using namespace schema;

  parser persist(persistent_data::get());
  bool uninitialized = persist.ldu(1);
  tvm_assert(!uninitialized, error_code::method_called_without_init);

  using data_tup_t = to_std_tuple_t<DContract>;

  if constexpr (persistent_header_info<Interface, ReplayAttackProtection>::non_empty) {
    using HeaderT = persistent_data_header_t<Interface, ReplayAttackProtection>;
    auto [data_hdr, =persist] = parse_continue<HeaderT>(persist);
    tvm_assert(!!data_hdr, error_code::persistent_data_parse_error);

    using Est = estimate_element<HeaderT>;
    static_assert(Est::max_bits == Est::min_bits, "Persistent data header can't be dynamic-size");

    // Only 1 + bitsize(Header) bits to skip
    using LinearTup = decltype(make_chain_tuple<1 + Est::max_bits, Est::max_refs>(data_tup_t{}));
    // uncomment to print in remark:
    //__reflect_echo<print_chain_tuple<LinearTup>().c_str()>{};
    auto linear_tup = parse<LinearTup>(persist);
    DContract base = to_struct<DContract>(chain_fold_tup<data_tup_t>(linear_tup));
    // DContract base = parse_chain<DContract, 1 + Est::max_bits, Est::max_refs>(persist);
    return { *data_hdr, base };
  } else {
    // Only 1 bit to skip
    using LinearTup = decltype(make_chain_tuple<1, 0>(data_tup_t{}));
    // uncomment to print in remark:
    //__reflect_echo<print_chain_tuple<LinearTup>().c_str()>{};
    auto linear_tup = parse<LinearTup>(persist);
    DContract base = to_struct<DContract>(chain_fold_tup<data_tup_t>(linear_tup));
    return { {}, base };
  }
}

template<class MsgHeader, class Data, bool dyn_chain>
__always_inline Data parse_smart(parser p, parser from_header) {
  using namespace schema;

  if constexpr (dyn_chain) {
    return parse_chain_dynamic<Data>(p);
  } else {
    using est_header_t = estimate_element<MsgHeader>;
    using header_with_args = std::tuple<MsgHeader, Data>;
    using est_t = estimate_element<std::tuple<MsgHeader, Data>>;
    if constexpr (est_t::max_bits > cell::max_bits || est_t::max_refs > cell::max_refs) {
      using LinearTup = decltype(make_chain_tuple<0, 0>(header_with_args{}));
      // uncomment to print in remark:
      // __reflect_echo<print_chain_tuple<LinearTup>().c_str()>{};
      auto linear_tup = parse<LinearTup>(from_header);
      return std::get<1>(chain_fold_tup<header_with_args>(linear_tup));
    } else {
      return parse<Data>(p);
    }
  }
}

template<class IContract, class ReplayAttackProtection, class DContract>
inline cell prepare_persistent_data(persistent_data_header_t<IContract, ReplayAttackProtection> persistent_data_header,
                                    DContract base) {
  using namespace schema;
  auto data_tup = to_std_tuple(base);
  // First add uninitialized bit = false in tuple
  // Also add replay attack protection header if needed
  // Then store data_tup_combined into chain of cells
  if constexpr (persistent_header_info<IContract, ReplayAttackProtection>::non_empty) {
    auto data_tup_combined = std::tuple_cat(std::make_tuple(bool_t(false), persistent_data_header), data_tup);
    auto chain_tup = make_chain_tuple(data_tup_combined);
    // uncomment to print in remark:
    //__reflect_echo<print_chain_tuple<decltype(chain_tup)>().c_str()>{};
    return build(chain_tup).make_cell();
  } else {
    auto data_tup_combined = std::tuple_cat(std::make_tuple(bool_t(false)), data_tup);
    auto chain_tup = make_chain_tuple(data_tup_combined);
    // uncomment to print in remark:
    //__reflect_echo<print_chain_tuple<decltype(chain_tup)>().c_str()>{};
    return build(chain_tup).make_cell();
  }
}

template<class IContract, class ReplayAttackProtection, class DContract>
inline void save_persistent_data(persistent_data_header_t<IContract, ReplayAttackProtection> persistent_data_header,
                                 DContract base) {
  persistent_data::set(prepare_persistent_data<IContract, ReplayAttackProtection, DContract>(persistent_data_header, base));
}

template<bool Internal, class Contract, class MsgHeader, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index, unsigned RestMethods>
struct smart_switcher_impl {
  static const unsigned my_method_id = get_func_id<IContract, Index>();

  __always_inline
  static int execute(Contract& c, std::optional<schema::bitfield<512>> signature,
      MsgHeader hdr, cell msg, slice msg_body, slice body_from_header) {
    constexpr bool is_getter = get_interface_method_getter<IContract, Index>::value;
    constexpr bool is_noaccept = get_interface_method_noaccept<IContract, Index>::value;
    constexpr bool is_implicit_id = is_implicit_func_id<IContract, Index>();
    constexpr bool is_internal = get_interface_method_internal<IContract, Index>::value;
    constexpr bool is_external = get_interface_method_external<IContract, Index>::value;
    constexpr bool is_dyn_chain_parse = get_interface_method_dyn_chain_parse<IContract, Index>::value;
    // getters should be inserted at main_external switch
    constexpr bool valid = (Internal && is_internal) ||
                           (!Internal && (is_external || is_getter));
    using namespace schema;
    if constexpr (valid)
    if (hdr.function_id() == my_method_id) {
      // We need to parse persistent data for contract, parse arguments for method,
      // call method, build return value and send message, build updated persistent data and store
      using ISmart = typename Contract::base;
      constexpr auto func = get_interface_method_ptr<Contract, ISmart, Index>::value;

      // Internal methods and getters are not signed
      // TODO: support signature check with offchain-constructor predefined pubkey
      if constexpr (!Internal && !is_getter && get_interface_has_pubkey<IContract>::value) {
        require(!!signature, error_code::invalid_signature);
        require(!!hdr.pubkey, error_code::no_pubkey);
        bool sign_ok = signature_checker_v2::check(body_from_header, (*signature)(), *hdr.pubkey);
        require(sign_ok, error_code::invalid_signature);
        if constexpr (supports_msg_pubkey_v<Contract>)
          c.set_msg_pubkey(hdr.pubkey->get());
      }

      persistent_data_header_t<IContract, ReplayAttackProtection> persistent_data_header;

      if constexpr (!get_interface_method_no_read_persistent<IContract, Index>::value) {
        // Load persistent data (not for constructor)
        if constexpr (!is_method_constructor<IContract, my_method_id>()) {
          auto [parsed_hdr, parsed_data] = load_persistent_data<IContract, ReplayAttackProtection, DContract>();
          auto &base = static_cast<DContract&>(c);
          persistent_data_header = parsed_hdr;
          base = parsed_data;
        } else {
          parser persist(persistent_data::get());
          bool uninitialized = persist.ldu(1);
          tvm_assert(uninitialized, error_code::constructor_double_call);
        }
      }

      // If ReplayAttackProtection is specified, we need to perform check
      //   for external & non-constructor methods.
      // And perform ReplayAttackProtection initialization for constructor calls
      if constexpr (!std::is_void_v<ReplayAttackProtection> && !is_getter) {
        if constexpr (!is_method_constructor<IContract, my_method_id>()) {
          if constexpr (!Internal) {
            if constexpr (interface_has_coroutines_v<IContract>) {
              auto verified = ReplayAttackProtection::check(hdr, std::get<0>(persistent_data_header));
              tvm_assert(!!verified, error_code::replay_attack_check);
              std::get<0>(persistent_data_header) = *verified;
            } else {
              auto verified = ReplayAttackProtection::check(hdr, persistent_data_header);
              tvm_assert(!!verified, error_code::replay_attack_check);
              persistent_data_header = *verified;
            }
          }
        } else {
          // Persistent data initialization in constructor
          if constexpr (interface_has_coroutines_v<IContract>) {
            persistent_data_header = { ReplayAttackProtection::init(), {} };
          } else {
            persistent_data_header = { ReplayAttackProtection::init() };
          }
        }
      }
      // We don't need accept for getter methods
      if constexpr (!is_getter && !is_noaccept)
        tvm_accept();

      if constexpr (supports_set_persistent_data_header_v<Contract>)
        c.set_persistent_data_header(persistent_data_header);

      using Args = get_interface_method_arg_struct<ISmart, Index>;
      constexpr unsigned args_sz = calc_fields_count<Args>::value;
      using rv_t = get_interface_method_rv<ISmart, Index>;

      parser body_p(msg_body);

      constexpr bool HasAnswerId =
        get_interface_method_internal<IContract, Index>::value && !std::is_void_v<rv_t> &&
        get_interface_method_answer_id<IContract, Index>::value;
      using FixedHeader = std::conditional_t<HasAnswerId, std::tuple<MsgHeader, uint32>, MsgHeader>;
      if constexpr (HasAnswerId) {
        c.set_return_func_id(uint32{body_p.ldu(32)});
      }

      unsigned next_answer_id = 0;
      // Execute and send answer if non-void return value
      if constexpr (!std::is_void_v<rv_t>) {
        if constexpr (is_coroutine_v<func>) {
          next_answer_id = std::get<1>(persistent_data_header).find_next_answer_id().get();
          temporary_data::setglob(global_id::answer_id, next_answer_id);
        } else {
          temporary_data::setglob(global_id::answer_id, 0);
        }
        if constexpr (args_sz != 0) {
          auto parsed_args = parse_smart<FixedHeader, Args, is_dyn_chain_parse>(body_p, parser(body_from_header));
          auto rv = std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                                      to_std_tuple(parsed_args)));
          if constexpr (is_coroutine_v<func>) {
            // this is resumable coroutine, we need to save its state if it is waiting for an answer
            if (!rv.done()) {
              slice wait_addr = __builtin_tvm_cast_to_slice(
                temporary_data::getglob(global_id::coroutine_wait_addr));
              auto std_addr = std::get<addr_std>(address(wait_addr)());

              slice ret_addr_sl;
              if constexpr (Internal)
                ret_addr_sl = int_return_address(c).sl();
              else
                ret_addr_sl = ext_return_address(c).sl();

              awaiting_record await_rec = {
                uint32{my_method_id}, std_addr.workchain_id, std_addr.address, ret_addr_sl };
              builder b = build(await_rec);
              __tvm_cell cl = serialize_resumable<func>(b.get(), rv);
              std::get<1>(persistent_data_header).add_awaiting(next_answer_id, cl);
            } else {
              if constexpr (!std::is_same_v<rv_t, resumable<void>>)
                send_answer<Internal, is_implicit_id>(c, my_method_id, rv.return_val());
            }
          } else {
            send_answer<Internal, is_implicit_id>(c, my_method_id, rv);
          }
        } else {
          auto rv = (c.*func)();
          if constexpr (is_coroutine_v<func>) {
            // this is resumable coroutine, we need to save its state if it is waiting for an answer
            if (!rv.done()) {
              slice wait_addr = __builtin_tvm_cast_to_slice(
                temporary_data::getglob(global_id::coroutine_wait_addr));
              auto std_addr = std::get<addr_std>(address(wait_addr)());

              slice ret_addr_sl;
              if constexpr (Internal)
                ret_addr_sl = int_return_address(c).sl();
              else
                ret_addr_sl = ext_return_address(c).sl();

              awaiting_record await_rec = {
                uint32{my_method_id}, std_addr.workchain_id, std_addr.address, ret_addr_sl };
              builder b = build(await_rec);
              __tvm_cell cl = serialize_resumable<func>(b.get(), rv);
              std::get<1>(persistent_data_header).add_awaiting(next_answer_id, cl);
            } else {
              if constexpr (!std::is_same_v<rv_t, resumable<void>>)
                send_answer<Internal, is_implicit_id>(c, my_method_id, rv.return_val());
            }
          } else {
            send_answer<Internal, is_implicit_id>(c, my_method_id, rv);
          }
        }
      } else {
        // void return (no need to send an answer message)

        temporary_data::setglob(global_id::answer_id, 0);
        if constexpr (args_sz != 0) {
          auto parsed_args = parse_smart<FixedHeader, Args, is_dyn_chain_parse>(body_p, parser(body_from_header));
          std::apply_f<func>(std::tuple_cat(std::make_tuple(std::ref(c)),
                                            to_std_tuple(parsed_args)));
        } else {
          (c.*func)();
        }
      }

      // Store persistent data
      if constexpr (!get_interface_method_no_write_persistent<IContract, Index>::value && !is_getter) {
        auto &base = static_cast<DContract&>(c);
        save_persistent_data<IContract, ReplayAttackProtection>(persistent_data_header, base);
      }
      if constexpr (supports_suicide_addr_v<Contract>) {
        if (auto opt_addr = c.suicide_addr()) {
          tvm_transfer(*opt_addr, 0, false,
            SEND_ALL_GAS | DELETE_ME_IF_I_AM_EMPTY | IGNORE_ACTION_ERRORS | SENDER_WANTS_TO_PAY_FEES_SEPARATELY);
        }
      }
      return my_method_id;
    }
    return smart_switcher_impl<Internal, Contract, MsgHeader, IContract, DContract, ReplayAttackProtection,
                               Index + 1, RestMethods - 1>::execute(c, signature, hdr, msg, msg_body, body_from_header);
  }
};
template<bool Internal, class Contract, class MsgHeader, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index>
struct smart_switcher_impl<Internal, Contract, MsgHeader, IContract, DContract, ReplayAttackProtection, Index, 0> {
  __always_inline
  static int execute(Contract& c, std::optional<schema::bitfield<512>> signature,
      MsgHeader hdr, cell msg, slice msg_body, slice body_from_header) {
    if constexpr (Internal && supports_fallback_v<Contract>)
      return Contract::_fallback(msg, msg_body);
    else
      tvm_throw(error_code::wrong_public_call);
    return 0;
  }
};

template<bool Internal, class Contract, class MsgHeader, class IContract, class DContract, class ReplayAttackProtection>
struct smart_switcher {
  static const unsigned methods_count = get_interface_methods_count<IContract>::value;
  __always_inline
  static int execute(Contract& c, std::optional<schema::bitfield<512>> signature,
      MsgHeader hdr, cell msg, slice msg_body, slice body_from_header) {
    return smart_switcher_impl<Internal, Contract, MsgHeader, IContract, DContract, ReplayAttackProtection,
                               0, methods_count>::execute(c, signature, hdr, msg, msg_body, body_from_header);
  }
};

__always_inline bool is_bounced(slice msg_sl) {
  parser p(msg_sl);
  require(p.ldu(1) == schema::int_msg_info{}.kind.code, error_code::bad_incoming_msg);
  p.skip(2); // ihr_disabled + bounce
  return p.ldu(1) != 0; // bounced != 0
}

template<class Contract, class IContract, class DContract, class ReplayAttackProtection,
         unsigned Index, unsigned RestMethods>
struct smart_process_answer_impl {
  static const unsigned my_method_id = get_func_id<IContract, Index>();

  template<class PersistentHeader>
  __always_inline
  static int execute(Contract& c, PersistentHeader persistent_data_header, awaiting_record await_rec,
                     cell msg, slice msg_body, slice await_sl) {
    using ISmart = typename Contract::base;
    static constexpr auto func = get_interface_method_ptr<Contract, ISmart, Index>::value;
    if constexpr (is_coroutine_v<func>) {
      if (my_method_id == await_rec.func_id) {
        constexpr bool is_internal = get_interface_method_internal<IContract, Index>::value;
        constexpr bool is_external = get_interface_method_external<IContract, Index>::value;
        constexpr bool is_implicit_id = is_implicit_func_id<IContract, Index>();
        static_assert(is_internal != is_external,
          "Coroutine method must be only internal or external, not both");

        using namespace schema;
        static_assert(!get_interface_method_no_read_persistent<IContract, Index>::value,
                      "Coroutine method can't be no-read-persistent");
        static_assert(!get_interface_method_no_write_persistent<IContract, Index>::value,
                      "Coroutine method can't be no-write-persistent");
        static_assert(persistent_header_info<Contract, ReplayAttackProtection>::non_empty,
                      "Coroutine method can't have empty persistent data header");

        parser body_p(msg_body);

        temporary_data::setglob(global_id::coroutine_answer_slice,
          __builtin_tvm_cast_from_slice(body_p.sl()));

        if constexpr (supports_set_persistent_data_header_v<Contract>)
          c.set_persistent_data_header(persistent_data_header);

        using rv_t = get_interface_method_rv<ISmart, Index>;

        unsigned next_answer_id = std::get<1>(persistent_data_header).find_next_answer_id().get();
        temporary_data::setglob(global_id::answer_id, next_answer_id);

        auto ret_addr = await_rec.return_addr();
        if constexpr (is_internal)
          set_int_return_address(c, address{ret_addr.sl()});
        else
          set_ext_return_address(c, address_ext{ret_addr.sl()});
        tvm_accept();
        auto res = deserialize_resumable<rv_t, func>(await_sl, &c);
        if (res.resume()) {
          // coroutine is finished by next awaitable request
          slice wait_addr = __builtin_tvm_cast_to_slice(
            temporary_data::getglob(global_id::coroutine_wait_addr));
          auto std_addr = std::get<addr_std>(address(wait_addr)());
          awaiting_record await_rec = {
            uint32{my_method_id}, std_addr.workchain_id, std_addr.address, ret_addr.sl() };
          builder b = build(await_rec);
          __tvm_cell cl = serialize_resumable<func>(b.get(), res);
          std::get<1>(persistent_data_header).add_awaiting(next_answer_id, cl);
          if constexpr (supports_set_persistent_data_header_v<Contract>)
            c.set_persistent_data_header(persistent_data_header);
        } else {
          if constexpr (!std::is_same_v<rv_t, resumable<void>>) {
            // coroutine is done, sending an answer
            send_answer<is_internal, is_implicit_id>(c, my_method_id, res.return_val());
          }
        }
        // Store persistent data
        auto &base = static_cast<DContract&>(c);
        save_persistent_data<IContract, ReplayAttackProtection>(persistent_data_header, base);
        return my_method_id;
      }
    }
    return smart_process_answer_impl<
      Contract, IContract, DContract, ReplayAttackProtection, Index + 1, RestMethods - 1>
        ::execute(c, persistent_data_header, await_rec, msg, msg_body, await_sl);
  }
};

template<class Contract, class IContract, class DContract, class ReplayAttackProtection, unsigned Index>
struct smart_process_answer_impl<Contract, IContract, DContract, ReplayAttackProtection, Index, 0> {
  template<class PersistentHeader>
  __always_inline
  static int execute(Contract& c, PersistentHeader persistent_data_header, awaiting_record await_rec,
                     cell msg, slice msg_body, slice await_sl) {
    tvm_throw(error_code::wrong_answer_id);
    return 0;
  }
};


template<class Contract, class MsgHeader, class IContract, class DContract, class ReplayAttackProtection>
struct smart_process_answer {
  static const unsigned methods_count = get_interface_methods_count<IContract>::value;
  __always_inline static int execute(Contract& c, MsgHeader hdr, cell msg, slice msg_body) {
    using namespace schema;
    unsigned answer_id = schema::abiv2::from_answer_id(hdr.function_id());
    persistent_data_header_t<IContract, ReplayAttackProtection> persistent_data_header;
    parser persist(persistent_data::get());
    bool uninitialized = persist.ldu(1);
    // Load persistent data
    tvm_assert(!uninitialized, error_code::method_called_without_init);
    // Load replay attack protection header
    using HeaderT = decltype(persistent_data_header);
    auto [data_hdr, =persist] = parse_continue<HeaderT>(persist);
    tvm_assert(!!data_hdr, error_code::persistent_data_parse_error);
    persistent_data_header = *data_hdr;
    if constexpr (supports_set_persistent_data_header_v<Contract>)
      c.set_persistent_data_header(persistent_data_header);

    using Est = estimate_element<HeaderT>;
    static_assert(Est::max_bits == Est::min_bits, "Persistent data header can't be dynamic-size");
    auto &base = static_cast<DContract&>(c);
    // Only 1 + bitsize(Header) bits to skip
    base = parse_chain<DContract, 1 + Est::max_bits, Est::max_refs>(persist);

    cell state_cl = std::get<1>(persistent_data_header).find_awaiting(answer_id);
    parser p(state_cl.ctos());
    auto [await_rec, =p] = parse_continue<awaiting_record>(p);
    require(!!await_rec, error_code::custom_data_parse_error);
    auto sender = std::get<addr_std>(int_return_address(c).val());
    require(sender.workchain_id == await_rec->waiting_workchain and sender.address == await_rec->waiting_addr, 
      error_code::unexpected_answer);
    std::get<1>(persistent_data_header).delete_awaiting(answer_id);
    return smart_process_answer_impl<Contract, IContract, DContract, ReplayAttackProtection, 0, methods_count>
      ::execute(c, persistent_data_header, *await_rec, msg, msg_body, p.sl());
  }
};

template<bool Internal, class Contract, class IContract, class DContract, class ReplayAttackProtection = void>
__always_inline
int smart_switch(cell msg, slice msg_body) {
  Contract c;
  if constexpr (Internal) {
    slice msg_slice = msg.ctos();
    if constexpr (supports_msg_slice_v<Contract>)
      c.set_msg_slice(msg_slice);
    else
      set_msg(msg);
    if constexpr (supports_on_bounced_v<Contract>) {
      if (is_bounced(msg_slice))
        return Contract::_on_bounced(msg, msg_body);
    } else {
      require(!is_bounced(msg_slice), error_code::unsupported_bounced_msg);
    }
  } else {
    // Store msg cell into contract if it supports the required field
    // Otherwise, store into global (set_msg)
    if constexpr (supports_msg_slice_v<Contract>)
      c.set_msg_slice(msg);
    else
      set_msg(msg);
  }

  parser msg_parser(msg_body);
  using namespace schema;
  if constexpr (Internal) {
    unsigned func_id;
    if constexpr (supports_fallback_v<Contract>) {
      if (auto opt_val = msg_parser.lduq(32))
        func_id = *opt_val;
      else
        return Contract::_fallback(msg, msg_body);
    } else {
      func_id = msg_parser.ldu(32);
    }
    if constexpr (supports_fallback_v<Contract>) {
      // TODO: the next line is required to prevent compiler from combining conditions into:
      //  `lduq.success != 0 or lduq.value != 0`.
      // Because lduq.value is null in case of lduq.success == 0 (null is generated by NULLROTRIFNOT).
      // We need ZEROROTRIFNOT / ZEROSWAPIFNOT instructions or disable such optimizations in compiler
      require(msg_body.sbits(), error_code::bad_incoming_msg);
      if (func_id == 0)
        return Contract::_fallback(msg, msg_body);
    }
    if constexpr (interface_has_coroutines_v<IContract>) {
      if (abiv2::is_answer_id(func_id)) {
        using MsgHeaderT = abiv2::internal_msg_header;
        parser body_p(msg_body);
        auto [in_hdr, =body_p] = parse_continue<MsgHeaderT>(body_p);
        tvm_assert(!!in_hdr, error_code::bad_arguments);
        return smart_process_answer<Contract, MsgHeaderT, IContract, DContract, ReplayAttackProtection>
          ::execute(c, *in_hdr, msg, body_p.sl());
      }
    }
    using MsgHeaderT = abiv2::internal_msg_header;
    parser body_p(msg_body);
    auto [in_hdr, =body_p] = parse_continue<MsgHeaderT>(body_p);
    tvm_assert(!!in_hdr, error_code::bad_arguments);
    return smart_switcher<Internal, Contract, MsgHeaderT, IContract, DContract, ReplayAttackProtection>
      ::execute(c, {}, *in_hdr, msg, body_p.sl(), msg_body);
  } else {
    static constexpr bool has_pubkey = get_interface_has_pubkey<IContract>::value;
    using MsgHeaderT = abiv2::external_inbound_msg_header<has_pubkey>;
    parser body_p(msg_body);
    // ABIv2
    //
    auto [sign, =body_p] = parse_continue<abiv2::external_signature>(body_p);
    slice body_from_header = body_p.sl();
    auto [in_hdr, =body_p] = parse_continue<MsgHeaderT>(body_p);
    tvm_assert(!!in_hdr, error_code::bad_arguments);
    return smart_switcher<Internal, Contract, MsgHeaderT, IContract, DContract, ReplayAttackProtection>
      ::execute(c, sign->signature, *in_hdr, msg, body_p.sl(), body_from_header);
  }
}

} // namespace tvm

