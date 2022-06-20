#pragma once

#include <tvm/reflection.hpp>
#include <tvm/schema/json-abi-gen.hpp>

namespace tvm {

// If func_id is specified for method, returns it.
// Otherwise, calculates by method signature.

template<auto MethodPtr>
constexpr unsigned get_func_id() {
  using namespace schema;
  constexpr unsigned explicit_id =
    __reflect_method_ptr_func_id<std::integral_constant, unsigned, MethodPtr>::value;
  if constexpr (explicit_id != 0) {
    return explicit_id;
  } else {
    return get_signature_func_id<json_abi_gen::make_func_signature<MethodPtr>().c_str()>::value;
  }
}

template<class Interface, unsigned Index>
constexpr unsigned get_func_id() {
  using namespace schema;
  constexpr unsigned explicit_id =
    __reflect_method_func_id<std::integral_constant, unsigned, Interface, Index>::value;
  if constexpr (explicit_id != 0) {
    return explicit_id;
  } else {
    return get_signature_func_id<json_abi_gen::make_func_signature<Interface, Index>().c_str()>::value;
  }
}

// If func id is not specified or was specified attribute [[implicit_func_id]]
template<class Interface, unsigned Index>
constexpr unsigned is_implicit_func_id() {
  constexpr unsigned explicit_id =
    __reflect_method_func_id<std::integral_constant, unsigned, Interface, Index>::value;
  return !explicit_id || get_interface_method_implicit_func_id<Interface, Index>::value;
}

template<auto MethodPtr>
using id = std::integral_constant<unsigned, get_func_id<MethodPtr>()>;
template<auto MethodPtr>
static constexpr unsigned id_v = get_func_id<MethodPtr>();

} // namespace tvm

