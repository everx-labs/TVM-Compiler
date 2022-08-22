#pragma once

#include <tuple>

namespace tvm {

template<size_t N>
struct to_std_tuple_impl {
  static_assert("Unsupported to_std_tuple size");
};

template<> struct to_std_tuple_impl<0> {
  template<typename _Tp> auto operator()(_Tp&&) const {
    return std::tuple();
  }
};

#ifdef TO_STD_TUPLE_CASE
#error re-definition of TO_STD_TUPLE_CASE
#endif

#define TO_STD_TUPLE_CASE(Sz, ...)                          \
template<> struct to_std_tuple_impl<Sz> {                   \
  template<typename _Tp> auto operator()(_Tp&& s) const {   \
    auto [__VA_ARGS__] = std::forward<_Tp>(s);              \
    return std::tuple(__VA_ARGS__);                         \
  }                                                         \
}

#include "to_std_tuple.def"

#undef TO_STD_TUPLE_CASE

template<typename T>
struct calc_fields_count {
  static constexpr unsigned value = __reflect_fields_count<std::integral_constant, unsigned, T>::value;
};

template<class _Tp>
auto to_std_tuple(_Tp s) {
  return to_std_tuple_impl<calc_fields_count<_Tp>::value>{}(s);
}

template<class _Tp>
using to_std_tuple_t = decltype(to_std_tuple(*(const _Tp*)nullptr));

} // namespace tvm

