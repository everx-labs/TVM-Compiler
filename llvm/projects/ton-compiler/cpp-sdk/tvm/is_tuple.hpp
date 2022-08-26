#pragma once

namespace tvm {

template<class T>
struct is_tuple_impl : std::false_type {};

template<class... U>
struct is_tuple_impl<std::tuple<U...>> : std::true_type {};

template<class T>
constexpr bool is_tuple() {
  return is_tuple_impl<T>::value;
}

template<class T>
constexpr bool is_tuple(T) {
  return is_tuple_impl<T>::value;
}

} // namespace tvm

