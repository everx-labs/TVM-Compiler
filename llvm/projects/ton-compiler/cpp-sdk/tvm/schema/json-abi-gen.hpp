#pragma once

#include <tvm/to_std_tuple.hpp>
#include <tvm/reflection.hpp>
#include <tvm/resumable.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/sequence.hpp>
#include <tvm/dict_array.hpp>
#include <tvm/dict_map.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>

/*
{
    "ABI version" : 2,
    "functions" : [
      {
        "name" : "fn1",
        "signed" : "false",
        "inputs" : [
          {"name" : "arg1", "type" : "uint64"}
        ],
        "outputs" : [
          {"name" : "result", "type" : "uint64"}
        ]
      }
    ]
}
*/

namespace tvm { namespace schema { namespace json_abi_gen {

using namespace hana::literals;

template<unsigned radix>
constexpr size_t get_magnitude(size_t num) {
  unsigned i = 0;
  while (num > 0) {
    num /= radix;
    ++i;
  }
  return i;
}

constexpr char hex_sym(unsigned num) {
  constexpr char arr[] = "0123456789abcdef";
  return arr[std::min(num, 16u)];
}

template <unsigned radix, class X, size_t ...i>
constexpr auto to_string(X x, std::index_sequence<i...>) {
  constexpr size_t mag = get_magnitude<radix>(X::value);
  return hana::string<
    hana::size_c<hex_sym(
      hana::value(x / hana::power(hana::size_c<radix>, hana::size_c<mag - i - 1>)
                    % hana::size_c<radix>))
      >...>{};
}

template <unsigned radix, class X>
constexpr auto to_string(X) {
  if constexpr (X::value == 0)
    return "0"_s;
  else
    return to_string<radix>(hana::size_c<static_cast<size_t>(X::value)>,
                            std::make_index_sequence<get_magnitude<radix>(X::value)>());
}

template<class T> struct is_tuple_type : std::true_type {};
template<unsigned _bitlen> struct is_tuple_type<int_t<_bitlen>> : std::false_type {};
template<unsigned _bitlen> struct is_tuple_type<uint_t<_bitlen>> : std::false_type {};
template<> struct is_tuple_type<MsgAddress> : std::false_type {};
template<> struct is_tuple_type<MsgAddressInt> : std::false_type {};
template<> struct is_tuple_type<MsgAddressExt> : std::false_type {};
template<class Element> struct is_tuple_type<dict_array<Element, 32>> : std::false_type {};
template<class Key, class Value> struct is_tuple_type<dict_map<Key, Value>> : std::false_type {};
template<> struct is_tuple_type<sequence<uint_t<8>>> : std::false_type {};
template<> struct is_tuple_type<cell> : std::false_type {};
template<class T> struct is_tuple_type<lazy<T>> : std::false_type {};

template<class T> struct is_tuple_array : std::false_type {};
template<class Element> struct is_tuple_array<dict_array<Element, 32>> : is_tuple_type<Element> {};

template<class T, unsigned Offset, class ReturnName>
constexpr auto make_struct_components();

template<class T, unsigned Offset> struct make_array_components {};
template<class Element, unsigned Offset> struct make_array_components<dict_array<Element, 32>, Offset> {
  static constexpr auto value = make_struct_components<Element, Offset, decltype(""_s)>();
};

template<class T>
struct make_simple_type_impl {
  static constexpr auto value = "unknown"_s;
};
template<unsigned _bitlen>
struct make_simple_type_impl<int_t<_bitlen>> {
  static constexpr auto value = "int"_s + to_string<10>(hana::integral_c<unsigned, _bitlen>);
};
template<unsigned _bitlen>
struct make_simple_type_impl<uint_t<_bitlen>> {
  static constexpr auto value = "uint"_s + to_string<10>(hana::integral_c<unsigned, _bitlen>);
};
template<>
struct make_simple_type_impl<uint_t<1>> {
  static constexpr auto value = "bool"_s;
};
template<>
struct make_simple_type_impl<MsgAddress> {
  static constexpr auto value = "address"_s;
};
template<>
struct make_simple_type_impl<MsgAddressInt> {
  static constexpr auto value = "address"_s;
};
template<>
struct make_simple_type_impl<MsgAddressExt> {
  static constexpr auto value = "address"_s;
};
template<class Element, bool is_tuple>
struct make_array_type {
  static constexpr auto elem_value = make_simple_type_impl<Element>::value;
  static constexpr auto value = elem_value + "[]"_s;
};
template<class Element>
struct make_array_type<Element, true> {
  static constexpr auto value = "tuple[]"_s;
};

template<class Element>
struct make_simple_type_impl<dict_array<Element, 32>> {
  static constexpr auto value = make_array_type<Element, is_tuple_type<Element>::value>::value;
};

template<class Key, class Value>
struct make_simple_type_impl<dict_map<Key, Value>> {
  static constexpr auto key_value = make_simple_type_impl<Key>::value;
  static constexpr auto elem_value = make_simple_type_impl<Value>::value;
  static constexpr auto value = "map("_s + key_value + ", "_s + elem_value + ")"_s;
};
template<>
struct make_simple_type_impl< sequence<uint_t<8>> > {
  static constexpr auto value = "bytes"_s;
};
template<>
struct make_simple_type_impl<cell> {
  static constexpr auto value = "cell"_s;
};
template<class T>
struct make_simple_type_impl<lazy<T>> {
  static constexpr auto value = make_simple_type_impl<T>::value;
};
template<class T>
struct make_simple_type_impl<resumable<T>> {
  static constexpr auto value = make_simple_type_impl<T>::value;
};
template<class T>
constexpr auto make_simple_type() {
  return make_simple_type_impl<T>::value;
}
template<class T>
constexpr bool has_simple_type() {
  return make_simple_type_impl<T>::value != "unknown"_s;
}

template<class T>
constexpr auto make_function_header(T func_name) {
  return "\"name\": \""_s + func_name + "\""_s;
}

template<class ParentT, size_t FieldIndex>
constexpr auto make_field_name() {
  return __reflect_field<hana::string, ParentT, FieldIndex>{};
};

template<bool IsLast>
constexpr auto make_field_impl_tail() {
  if constexpr (IsLast)
    return "}\n"_s;
  else
    return "},\n"_s;
}

template<unsigned Offset>
struct make_offset {
  static constexpr auto value = "  "_s + make_offset<Offset - 1>::value;
};
template<>
struct make_offset<0> {
  static constexpr auto value = ""_s;
};

template<class T, bool IsLast, unsigned Offset, class FieldName>
constexpr auto make_field_impl(FieldName field_name) {
  if constexpr (is_tuple_array<T>::value) {
    return make_offset<Offset>::value + "{ \"components\":"_s + make_array_components<T, Offset>::value
      + ", \"name\":\""_s + field_name + "\", \"type\":\"tuple[]\" "_s
      + make_field_impl_tail<IsLast>();
  } else if constexpr (is_tuple_type<T>::value) {
    return make_offset<Offset>::value + "{ \"components\":"_s + make_struct_components<T, Offset, decltype(""_s)>()
      + ", \"name\":\""_s + field_name + "\", \"type\":\"tuple\" "_s
      + make_field_impl_tail<IsLast>();
  } else {
    return make_offset<Offset>::value + "{ \"name\":\""_s + field_name + "\", \"type\":\""_s +
        make_simple_type<T>() + "\" "_s + make_field_impl_tail<IsLast>();
  }
};

template<class ParentT, size_t Size, size_t FieldIndex, class T, unsigned Offset>
constexpr auto make_field_json() {
  constexpr auto field_name = make_field_name<ParentT, FieldIndex>();
  return make_field_impl<T, FieldIndex + 1 == Size, Offset>(field_name);
};

template<unsigned Offset, class ParentT, size_t Size, size_t FieldIndex, class T, class... Types>
struct make_tuple_json_impl {
  static constexpr auto value = make_field_json<ParentT, Size, FieldIndex, T, Offset>() +
      make_tuple_json_impl<Offset, ParentT, Size, FieldIndex + 1, Types...>::value;
};
template<unsigned Offset, class ParentT, size_t Size, size_t FieldIndex, class T>
struct make_tuple_json_impl<Offset, ParentT, Size, FieldIndex, T> {
  static constexpr auto value = make_field_json<ParentT, Size, FieldIndex, T, Offset>();
};
template<unsigned Offset, class ParentT, class Tuple>
struct make_tuple_json {};
template<unsigned Offset, class ParentT, class... Types>
struct make_tuple_json<Offset, ParentT, std::tuple<Types...>> {
  static constexpr auto value = make_tuple_json_impl<Offset, ParentT, sizeof...(Types), 0, Types...>::value;
};
template<unsigned Offset, class ParentT>
struct make_tuple_json<Offset, ParentT, std::tuple<>> {
  static constexpr auto value = ""_s;
};

template<class T, unsigned Offset, class ReturnName>
struct make_struct_json {
  static constexpr auto value = make_tuple_json<Offset, T, tvm::to_std_tuple_t<T>>::value;
};
template<unsigned Offset, class ReturnName>
struct make_struct_json<void, Offset, ReturnName> {
  static constexpr auto value = ""_s;
};

template<class ReturnName>
__always_inline constexpr auto make_return_name() {
  if constexpr (hana::is_empty(ReturnName{}))
    return "value0"_s;
  else
    return ReturnName{};
}

// For simple-type return value ("MsgAddress func()") we don't have name for field, so using `ReturnName`
template<unsigned Offset, class ReturnName, unsigned _bitlen>
struct make_struct_json<int_t<_bitlen>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<int_t<_bitlen>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName, unsigned _bitlen>
struct make_struct_json<uint_t<_bitlen>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<uint_t<_bitlen>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName>
struct make_struct_json<uint_t<1>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<uint_t<1>, 1, Offset>(make_return_name<ReturnName>());
};

template<unsigned Offset, class ReturnName>
struct make_struct_json<MsgAddress, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<MsgAddress, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName>
struct make_struct_json<MsgAddressInt, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<MsgAddressInt, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName>
struct make_struct_json<MsgAddressExt, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<MsgAddressExt, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName, class Element>
struct make_struct_json<dict_array<Element, 32>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<dict_array<Element, 32>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName, class Key, class Value>
struct make_struct_json<dict_map<Key, Value>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<dict_map<Key, Value>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName>
struct make_struct_json< sequence<uint_t<8>>, Offset, ReturnName > {
  static constexpr auto value = make_field_impl<sequence<uint_t<8>>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName>
struct make_struct_json<cell, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<cell, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName, class T>
struct make_struct_json<lazy<T>, Offset, ReturnName> {
  static constexpr auto value = make_field_impl<lazy<T>, 1, Offset>(make_return_name<ReturnName>());
};
template<unsigned Offset, class ReturnName, class T>
struct make_struct_json<resumable<T>, Offset, ReturnName> {
  static constexpr auto value = make_struct_json<T, Offset, ReturnName>::value;
};

template<class T, unsigned Offset, class ReturnName>
constexpr auto make_struct_components() {
  return "[\n"_s + make_struct_json<T, Offset + 1, ReturnName>::value + make_offset<Offset>::value + "]"_s;
};

constexpr auto make_abi_version() {
  return "\"ABI version\": 2"_s;
}

constexpr auto make_getters() {
  return "\"getters\": ["_s +
    "\n  ]"_s;
}

template<class Interface>
constexpr auto make_header() {
  if constexpr (get_interface_has_pubkey<Interface>::value) {
    return "\"header\": [\n    \"pubkey\",\n    \"time\",\n    \"expire\"\n  ]"_s;
  } else {
    return "\"header\": [\n    \"time\",\n    \"expire\"\n  ]"_s;
  }
}

constexpr auto make_functions_begin() {
  return "\"functions\": ["_s;
}
constexpr auto make_functions_end() {
  return "\n  ]"_s;
}

constexpr auto make_events_begin() {
  return "\"events\": ["_s;
}
constexpr auto make_events_end() {
  return "\n  ]"_s;
}

template<bool HasAnswerId, unsigned ArgsSize>
constexpr auto make_inputs_prefix() {
  if constexpr (HasAnswerId) {
    constexpr auto prefix = "\"inputs\": [\n    { \"name\":\"_answer_id\", \"type\":\"uint32\" }"_s;
    if constexpr (ArgsSize == 0)
      return prefix + "\n"_s;
    else
      return prefix + ",\n"_s;
  } else {
    return "\"inputs\": [\n"_s;
  }
}

template<class ArgStruct, class ReturnName, bool HasAnswerId>
constexpr auto make_function_inputs() {
  // TODO: remove answer_id field generation in abi when abiv3 will provide answer_id in header
  constexpr auto prefix =
    make_inputs_prefix< HasAnswerId, std::tuple_size_v< to_std_tuple_t<ArgStruct> > >();
  return prefix + make_struct_json<ArgStruct, 2, ReturnName>::value + "    ]"_s;
}

template<class RvStruct, class ReturnName>
constexpr auto make_function_outputs() {
  return "\"outputs\": [\n"_s + make_struct_json<RvStruct, 2, ReturnName>::value + "    ]"_s;
}

template<unsigned FuncID, bool ImplicitFuncId>
constexpr auto make_function_id() {
  if constexpr (FuncID != 0 && !ImplicitFuncId)
    return ",\n    \"id\": \"0x"_s + to_string<16>(hana::integral_c<unsigned, FuncID>) + "\""_s;
  else
    return ""_s;
}

// ====== Signature generation =========
template<class T>
struct is_resumable : std::false_type {};
template<class T>
struct is_resumable<resumable<T>> : std::true_type {};
template<class T>
constexpr bool is_resumable_v = is_resumable<T>::value;

template<class T>
__always_inline constexpr auto make_type_signature();

// arg type lists for signature
template<class ArgsTuple>
struct make_arg_types_list {
  static constexpr auto value = ""_s;
};
template<class Arg0, class... Args>
struct make_arg_types_list<std::tuple<Arg0, Args...>> {
  static constexpr auto value =
    make_type_signature<Arg0>() + (""_s + ... + (","_s + make_type_signature<Args>()));
};

template<class T>
struct make_type_signature_impl {
  static constexpr auto value = []() constexpr {
    if constexpr (has_simple_type<T>()) {
      return make_simple_type<T>();
    } else {
      return "("_s + make_arg_types_list<to_std_tuple_t<T>>::value + ")"_s;
    }
  }();
};

template<class Element>
struct make_type_signature_impl<dict_array<Element, 32>> {
  static constexpr auto value = make_type_signature<Element>() + "[]"_s;
};

template<class Key, class Value>
struct make_type_signature_impl<dict_map<Key, Value>> {
  static constexpr auto value =
    "map("_s + make_type_signature<Key>() + ","_s + make_type_signature<Value>() + ")"_s;
};

template<class T>
constexpr auto make_type_signature() {
  return make_type_signature_impl<T>::value;
};

// return value type lists for signature
template<class Rv>
constexpr auto make_rv_types_list() {
  if constexpr (std::is_void_v<Rv>) {
    return ""_s;
  } else if constexpr (is_resumable_v<Rv>) {
    return make_rv_types_list<typename resumable_subtype<Rv>::type>();
  } else if constexpr (has_simple_type<Rv>()) {
    return make_type_signature<Rv>();
  } else {
    return make_arg_types_list<to_std_tuple_t<Rv>>::value;
  }
}

template<bool HasAnswerId, unsigned ArgsSize>
constexpr auto make_signature_prefix() {
  if constexpr (HasAnswerId) {
    if constexpr (ArgsSize == 0)
      return "(uint32"_s;
    else
      return "(uint32,"_s;
  } else {
    return "("_s;
  }
}

// generate function signature like: `getVersion()(bytes,uint24)v2`
template<class Interface, unsigned CurMethod>
constexpr auto make_func_signature() {
  using FuncName = get_interface_method_name<Interface, CurMethod>;
  using Arg = get_interface_method_arg_struct<Interface, CurMethod>;
  using ArgsTuple = to_std_tuple_t<Arg>;
  using Rv = get_interface_method_rv<Interface, CurMethod>;
  constexpr bool HasAnswerId =
    get_interface_method_internal<Interface, CurMethod>::value && !std::is_void_v<Rv> &&
    get_interface_method_answer_id<Interface, CurMethod>::value;
  // TODO: remove answer_id field generation in abi when abiv3 will provide answer_id in header
  auto prefix = make_signature_prefix<HasAnswerId, std::tuple_size_v<ArgsTuple>>();

  return FuncName{} +
    prefix + make_arg_types_list<ArgsTuple>::value + ")("_s + make_rv_types_list<Rv>() + ")v2"_s;
}

template<auto MethodPtr>
constexpr auto make_func_signature() {
  using FuncName = get_interface_method_ptr_name<MethodPtr>;
  using Arg = args_struct_t<MethodPtr>;
  using ArgsTuple = to_std_tuple_t<Arg>;
  using Rv = get_interface_method_ptr_rv<MethodPtr>;
  constexpr bool HasAnswerId =
    get_interface_method_ptr_internal<MethodPtr>::value && !std::is_void_v<Rv> &&
    get_interface_method_ptr_answer_id<MethodPtr>::value;
  // TODO: remove answer_id field generation in abi when abiv3 will provide answer_id in header
  auto prefix = make_signature_prefix<HasAnswerId, std::tuple_size_v<ArgsTuple>>();
  return FuncName{} +
    prefix + make_arg_types_list<ArgsTuple>::value + ")("_s + make_rv_types_list<Rv>() + ")v2"_s;
}
// ====== ^^^Signature generation^^^ =========

template<unsigned FuncID, bool ImplicitFuncId, bool HasAnswerId, class RvStruct,
         class ArgStruct, class ReturnName, class FuncName>
constexpr auto make_function_json(FuncName func_name) {
  constexpr auto hdr = make_function_header(func_name);
  constexpr auto inputs = make_function_inputs<ArgStruct, ReturnName, HasAnswerId>();
  constexpr auto outputs = make_function_outputs<RvStruct, ReturnName>();
  constexpr auto id = make_function_id<FuncID, ImplicitFuncId>();
  return "  {\n    "_s + hdr + ",\n    "_s + inputs + ",\n    "_s + outputs + id + "\n  }"_s;
}

template<class Interface, unsigned CurMethod, unsigned RestMethods>
struct make_json_abi_impl {
  static const unsigned FuncId = get_interface_method_func_id<Interface, CurMethod>::value;
  using Rv = get_interface_method_rv<Interface, CurMethod>;
  using Arg = get_interface_method_arg_struct<Interface, CurMethod>;
  using FuncName = get_interface_method_name<Interface, CurMethod>;
  using ReturnName = get_interface_return_name<Interface, CurMethod>;
  // If func id is not specified or was specified attribute [[implicit_func_id]]
  static constexpr bool ImplicitFuncId =
    get_interface_method_implicit_func_id<Interface, CurMethod>::value || !FuncId;
  static constexpr bool HasAnswerId =
    get_interface_method_internal<Interface, CurMethod>::value && !std::is_void_v<Rv> &&
    get_interface_method_answer_id<Interface, CurMethod>::value;

  static constexpr auto value =
    make_function_json<FuncId, ImplicitFuncId, HasAnswerId, Rv, Arg, ReturnName>(FuncName{}) + ",\n"_s +
      make_json_abi_impl<Interface, CurMethod + 1, RestMethods - 1>::value;
};
template<class Interface, unsigned CurMethod>
struct make_json_abi_impl<Interface, CurMethod, 1> {
  static const unsigned FuncId = get_interface_method_func_id<Interface, CurMethod>::value;
  using Rv = get_interface_method_rv<Interface, CurMethod>;
  using Arg = get_interface_method_arg_struct<Interface, CurMethod>;
  using FuncName = get_interface_method_name<Interface, CurMethod>;
  using ReturnName = get_interface_return_name<Interface, CurMethod>;
  static constexpr bool ImplicitFuncId =
    get_interface_method_implicit_func_id<Interface, CurMethod>::value || !FuncId;
  static constexpr bool HasAnswerId =
    get_interface_method_internal<Interface, CurMethod>::value && !std::is_void_v<Rv> &&
    get_interface_method_answer_id<Interface, CurMethod>::value;

  static constexpr auto value =
    make_function_json<FuncId, ImplicitFuncId, HasAnswerId, Rv, Arg, ReturnName>(FuncName{});
};
template<class Interface, unsigned CurMethod>
struct make_json_abi_impl<Interface, CurMethod, 0> {
  static constexpr auto value = ""_s;
};

template<class Interface, class Events>
constexpr auto make_json_abi() {
  using MethodsCount = get_interface_methods_count<Interface>;
  using EventsCount = get_interface_methods_count<Events>;
  return
    "{\n  "_s + make_abi_version() + ",\n  "_s + make_header<Interface>() + ",\n  "_s +
      make_functions_begin() + "\n"_s +
        make_json_abi_impl<Interface, 0, MethodsCount::value>::value +
      make_functions_end() + ",\n  "_s +
      make_events_begin() +
        make_json_abi_impl<Events, 0, EventsCount::value>::value +
      make_events_end() +
    "\n}\n"_s;
}

#define DEFINE_JSON_ABI(Interface, DInterface, EInterface) \
  const char* json_abi = tvm::schema::json_abi_gen::make_json_abi<Interface, EInterface>().c_str()

}}} // namespace tvm::schema::json_abi_gen
