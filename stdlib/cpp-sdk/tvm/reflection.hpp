#pragma once

#define BOOST_HANA_CONFIG_ENABLE_STRING_UDL

#include <boost/hana/string.hpp>
#include <boost/hana/power.hpp>
#include <boost/hana/value.hpp>
#include <boost/hana/div.hpp>
#include <boost/hana/plus.hpp>

namespace tvm {

namespace hana = boost::hana;

template<class Interface>
using get_interface_methods_count =
  __reflect_methods_count<std::integral_constant, unsigned, Interface>;
template<class Interface, unsigned Index>
using get_interface_method_name = __reflect_method_name<hana::string, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_func_id =
  __reflect_method_func_id<std::integral_constant, unsigned, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_internal =
  __reflect_method_internal<std::integral_constant, bool, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_external =
  __reflect_method_external<std::integral_constant, bool, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_getter =
  __reflect_method_getter<std::integral_constant, bool, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_no_read_persistent =
  __reflect_method_no_read_persistent<std::integral_constant, bool, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_no_write_persistent =
  __reflect_method_no_write_persistent<std::integral_constant, bool, Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_rv = __reflect_method_rv<Interface, Index>;
template<class Interface, unsigned Index>
using get_interface_method_arg_struct = __reflect_method_arg_struct<Interface, Index>;

template<class Fmt>
using parsed_value = Fmt;
template<class Fmt>
using built_value = Fmt;

// For each interface method "Rv Meth(ArgN...)" generates method
//  virtual built_value<Rv> Meth(parsed_value<ArgN>...)
template<class Interface>
using smart_interface = __reflect_smart_interface<parsed_value, built_value, Interface>;

template<auto MethodPtr>
using id = __reflect_method_ptr_func_id<std::integral_constant, unsigned, MethodPtr>;
template<auto MethodPtr>
static constexpr unsigned id_v =
  __reflect_method_ptr_func_id<std::integral_constant, unsigned, MethodPtr>::value;

template<auto MethodPtr>
struct proxy_method {
  static constexpr auto value = MethodPtr;
};

template<class Contract, class Interface, unsigned Index>
using get_interface_method_ptr = __reflect_method_ptr<proxy_method, Contract, Interface, Index>;

} // namespace tvm

