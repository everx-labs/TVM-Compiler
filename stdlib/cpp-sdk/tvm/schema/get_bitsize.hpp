#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

// Calculates static bitsize for format element or returns 0 if it can't be calculated compile-time
template<class _Tp>
struct get_bitsize {
  static_assert(std::is_class_v<_Tp>);
  static constexpr unsigned value = get_bitsize<to_std_tuple_t<_Tp>>::value;
};

// Implementations for different types

// Dynamic size types
template<auto _Field>
struct get_bitsize<dynamic_bitfield<_Field>> {
  static constexpr unsigned value = 0;
};
template<auto _Field>
struct get_bitsize<dynamic_uint<_Field>> {
  static constexpr unsigned value = 0;
};
template<auto _Field>
struct get_bitsize<dynamic_int<_Field>> {
  static constexpr unsigned value = 0;
};
template<unsigned _bitlen>
struct get_bitsize<varuint<_bitlen>> {
  static constexpr unsigned value = 0;
};
template<unsigned _bitlen>
struct get_bitsize<varint<_bitlen>> {
  static constexpr unsigned value = 0;
};
template<unsigned _keylen, class _element_type>
struct get_bitsize<HashmapE<_keylen, _element_type>> {
  static constexpr unsigned value = 0;
};
template<class T>
struct get_bitsize<std::optional<T>> {
  static constexpr unsigned value = 0;
};

// Counting refs as dynamic
template<class _Tp>
struct get_bitsize<ref<_Tp>> {
  static constexpr unsigned value = 0;
};
template<>
struct get_bitsize<cell> {
  static constexpr unsigned value = 0;
};
template<>
struct get_bitsize<anyval> {
  static constexpr unsigned value = 0;
};
template<>
struct get_bitsize<MsgAddressSlice> {
  static constexpr unsigned value = 0;
};

// Static size types
template<unsigned _bitlen>
struct get_bitsize<bitfield<_bitlen>> {
  static constexpr unsigned value = _bitlen;
};
template<unsigned _bitlen, unsigned _code>
struct get_bitsize<bitconst<_bitlen, _code>> {
  static constexpr unsigned value = _bitlen;
};
template<unsigned _bitlen>
struct get_bitsize<uint_t<_bitlen>> {
  static constexpr unsigned value = _bitlen;
};
template<unsigned _bitlen>
struct get_bitsize<int_t<_bitlen>> {
  static constexpr unsigned value = _bitlen;
};

// Container types
template<class... Types>
struct get_bitsize<std::tuple<Types...>> {
  static constexpr bool all_calculated = (get_bitsize<Types>::value && ...);
  static constexpr unsigned value = all_calculated ? (get_bitsize<Types>::value + ...) : 0;
};
template<class... Types>
struct get_bitsize<std::variant<Types...>> {
  static constexpr bool all_calculated = (get_bitsize<Types>::value && ...);
  static constexpr unsigned value = all_calculated ? std::max({get_bitsize<Types>::value ... }) : 0;
};
template<class X>
struct get_bitsize<EitherLeft<X>> {
  static constexpr unsigned value = get_bitsize< to_std_tuple_t<EitherLeft<X>> >::value;
};
template<class Y>
struct get_bitsize<EitherRight<Y>> {
  static constexpr unsigned value = get_bitsize< to_std_tuple_t<EitherRight<Y>> >::value;
};
template<class X, class Y>
struct get_bitsize<Either<X, Y>> {
  static constexpr unsigned value = get_bitsize< to_std_tuple_t<Either<X, Y>> >::value;
};
template<class T>
struct get_bitsize<lazy<T>> {
  static constexpr unsigned value = get_bitsize<T>::value;
};

// Helper template constant
template<class Element>
static constexpr unsigned get_bitsize_v = get_bitsize<Element>::value;

}} // namespace tvm::schema

