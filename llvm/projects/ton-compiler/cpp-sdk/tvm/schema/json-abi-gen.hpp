#pragma once

#include <tvm/to_std_tuple.hpp>
#include <tvm/reflection.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/sequence.hpp>
#include <tvm/dict_array.hpp>
#include <tvm/dict_map.hpp>

/*
{
    "ABI version" : 0,
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
template<class Element>
struct make_simple_type_impl<dict_array<Element, 32>> {
  static constexpr auto elem_value = make_simple_type_impl<Element>::value;
  static constexpr auto value = elem_value + "[]"_s;
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
constexpr auto make_simple_type() {
  return make_simple_type_impl<T>::value;
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

template<class T, bool IsLast, class FieldName>
constexpr auto make_field_impl(FieldName field_name) {
  return "      { \"name\":\""_s + field_name + "\", \"type\":\""_s +
      make_simple_type<T>() + "\" "_s + make_field_impl_tail<IsLast>();
};

template<class ParentT, size_t Size, size_t FieldIndex, class T>
constexpr auto make_field_json() {
  constexpr auto field_name = make_field_name<ParentT, FieldIndex>();
  return make_field_impl<T, FieldIndex + 1 == Size>(field_name);
};

template<class ParentT, size_t Size, size_t FieldIndex, class T, class... Types>
struct make_tuple_json_impl {
  static constexpr auto value = make_field_json<ParentT, Size, FieldIndex, T>() +
      make_tuple_json_impl<ParentT, Size, FieldIndex + 1, Types...>::value;
};
template<class ParentT, size_t Size, size_t FieldIndex, class T>
struct make_tuple_json_impl<ParentT, Size, FieldIndex, T> {
  static constexpr auto value = make_field_json<ParentT, Size, FieldIndex, T>();
};
template<class ParentT, class Tuple>
struct make_tuple_json {};
template<class ParentT, class... Types>
struct make_tuple_json<ParentT, std::tuple<Types...>> {
  static constexpr auto value = make_tuple_json_impl<ParentT, sizeof...(Types), 0, Types...>::value;
};
template<class ParentT>
struct make_tuple_json<ParentT, std::tuple<>> {
  static constexpr auto value = ""_s;
};

template<class T>
struct make_struct_json {
  static constexpr auto value = make_tuple_json<T, tvm::to_std_tuple_t<T>>::value;
};
template<>
struct make_struct_json<void> {
  static constexpr auto value = ""_s;
};

// For simple-type return value ("MsgAddress func()") we don't have name for field, so just "value0"
template<unsigned _bitlen>
struct make_struct_json<int_t<_bitlen>> {
  static constexpr auto value = make_field_impl<int_t<_bitlen>, 1>("value0"_s);
};
template<unsigned _bitlen>
struct make_struct_json<uint_t<_bitlen>> {
  static constexpr auto value = make_field_impl<uint_t<_bitlen>, 1>("value0"_s);
};
template<>
struct make_struct_json<uint_t<1>> {
  static constexpr auto value = make_field_impl<uint_t<1>, 1>("value0"_s);
};

template<>
struct make_struct_json<MsgAddress> {
  static constexpr auto value = make_field_impl<MsgAddress, 1>("value0"_s);
};
template<>
struct make_struct_json<MsgAddressInt> {
  static constexpr auto value = make_field_impl<MsgAddressInt, 1>("value0"_s);
};
template<>
struct make_struct_json<MsgAddressExt> {
  static constexpr auto value = make_field_impl<MsgAddressExt, 1>("value0"_s);
};
template<class Element>
struct make_struct_json<dict_array<Element, 32>> {
  static constexpr auto value = make_field_impl<dict_array<Element, 32>, 1>("value0"_s);
};
template<class Key, class Value>
struct make_struct_json<dict_map<Key, Value>> {
  static constexpr auto value = make_field_impl<dict_map<Key, Value>, 1>("value0"_s);
};
template<>
struct make_struct_json< sequence<uint_t<8>> > {
  static constexpr auto value = make_field_impl<sequence<uint_t<8>>, 1>("value0"_s);
};
template<>
struct make_struct_json<cell> {
  static constexpr auto value = make_field_impl<cell, 1>("value0"_s);
};
template<class T>
struct make_struct_json<lazy<T>> {
  static constexpr auto value = make_field_impl<lazy<T>, 1>("value0"_s);
};
template<class T>
struct make_struct_json<resumable<T>> {
  static constexpr auto value = make_struct_json<T>::value;
};

constexpr auto make_abi_version() {
  return "\"ABI version\": 1"_s;
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

template<class ArgStruct>
constexpr auto make_function_inputs() {
  return "\"inputs\": [\n"_s + make_struct_json<ArgStruct>::value + "    ]"_s;
}

template<class RvStruct>
constexpr auto make_function_outputs() {
  return "\"outputs\": [\n"_s + make_struct_json<RvStruct>::value + "    ]"_s;
}


template<unsigned FuncID>
constexpr auto make_function_id() {
  if constexpr (FuncID != 0)
    return ",\n    \"id\": \"0x"_s + to_string<16>(hana::integral_c<unsigned, FuncID>) + "\""_s;
  else
    return ""_s;
}

template<unsigned FuncID, class RvStruct, class ArgStruct, class FuncName>
constexpr auto make_function_json(FuncName func_name) {
  constexpr auto hdr = make_function_header(func_name);
  constexpr auto inputs = make_function_inputs<ArgStruct>();
  constexpr auto outputs = make_function_outputs<RvStruct>();
  constexpr auto id = make_function_id<FuncID>();
  return "  {\n    "_s + hdr + ",\n    "_s + inputs + ",\n    "_s + outputs + id + "\n  }"_s;
}

template<class Interface, unsigned CurMethod, unsigned RestMethods>
struct make_json_abi_impl {
  static const unsigned FuncId = get_interface_method_func_id<Interface, CurMethod>::value;
  using Rv = get_interface_method_rv<Interface, CurMethod>;
  using Arg = get_interface_method_arg_struct<Interface, CurMethod>;
  using FuncName = get_interface_method_name<Interface, CurMethod>;

  static constexpr auto value = make_function_json<FuncId, Rv, Arg>(FuncName{}) + ",\n"_s +
    make_json_abi_impl<Interface, CurMethod + 1, RestMethods - 1>::value;
};
template<class Interface, unsigned CurMethod>
struct make_json_abi_impl<Interface, CurMethod, 1> {
  static const unsigned FuncId = get_interface_method_func_id<Interface, CurMethod>::value;
  using Rv = get_interface_method_rv<Interface, CurMethod>;
  using Arg = get_interface_method_arg_struct<Interface, CurMethod>;
  using FuncName = get_interface_method_name<Interface, CurMethod>;

  static constexpr auto value = make_function_json<FuncId, Rv, Arg>(FuncName{});
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
    "{\n  "_s + make_abi_version() + ",\n  "_s +
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
