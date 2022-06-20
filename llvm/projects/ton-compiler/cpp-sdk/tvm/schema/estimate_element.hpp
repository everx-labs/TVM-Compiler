#pragma once

#include <tvm/schema/basics.hpp>
#include <tvm/schema/message.hpp>

namespace tvm { namespace schema {

template<class _Tp>
struct estimate_element;

namespace detail {
  template<unsigned _min, unsigned _max>
  struct bits {
    static constexpr unsigned min_bits = _min, max_bits = _max;
  };
  template<unsigned _min, unsigned _max>
  struct refs {
    static constexpr unsigned min_refs = _min, max_refs = _max;
  };
  struct no_refs {
    static constexpr unsigned min_refs = 0, max_refs = 0;
  };
  struct one_ref {
    static constexpr unsigned min_refs = 1, max_refs = 1;
  };
  template<class _Tp>
  struct chain_to {
    using Est = estimate_element<_Tp>;
    static constexpr unsigned min_bits = Est::min_bits;
    static constexpr unsigned max_bits = Est::max_bits;
    static constexpr unsigned min_refs = Est::min_refs;
    static constexpr unsigned max_refs = Est::max_refs;
  };
  template<unsigned _sz>
  struct exact_size : bits<_sz, _sz>, no_refs {};
}

template<class _Tp>
struct estimate_element : detail::chain_to<to_std_tuple_t<_Tp>> {
  static_assert(std::is_class_v<_Tp>);
};

// Implementations for different types

template <class C, typename T>
T get_pointer_type(T C::*v);

template<auto _Field>
constexpr unsigned get_dynamic_field_max_bits() {
  using FieldT = decltype(get_pointer_type(_Field));
  static_assert(std::is_same_v<uint_t<FieldT::bitlen>, FieldT>, "Dynamic field size is not uint");
  static_assert(FieldT::bitlen <= 16, "Dynamic field size is too large");
  return (1u << FieldT::bitlen) - 1;
}

// Dynamic size types
template<auto _Field>
struct estimate_element<dynamic_bitfield<_Field>>
  : detail::bits<0, get_dynamic_field_max_bits<_Field>()>
  , detail::no_refs {};
template<auto _Field>
struct estimate_element<dynamic_uint<_Field>>
  : detail::bits<0, std::min(256u, get_dynamic_field_max_bits<_Field>())>
  , detail::no_refs {};
template<auto _Field>
struct estimate_element<dynamic_int<_Field>>
  : detail::bits<0, std::min(257u, get_dynamic_field_max_bits<_Field>())>
  , detail::no_refs {};
template<>
struct estimate_element<varuint<16>>
  : detail::bits<4, 4 + 15 * 8>
  , detail::no_refs {};
template<>
struct estimate_element<varint<16>>
  : detail::bits<4, 4 + 15 * 8>
  , detail::no_refs {};
template<>
struct estimate_element<varuint<32>>
  : detail::bits<5, 5 + 31 * 8>
  , detail::no_refs {};
template<>
struct estimate_element<varint<32>>
  : detail::bits<5, 5 + 31 * 8>
  , detail::no_refs {};
template<unsigned _keylen, class _element_type>
struct estimate_element<HashmapE<_keylen, _element_type>>
  : detail::bits<1, 1>
  , detail::refs<0, 1> {};

template<class _Tp>
struct estimate_element<ref<_Tp>>
  : detail::bits<0, 0>
  , detail::one_ref {};
template<>
struct estimate_element<cell>
  : detail::bits<0, 0>
  , detail::one_ref {};
template<>
struct estimate_element<anydict>
  : detail::bits<1, 1>
  , detail::refs<0, 1> {};
template<>
struct estimate_element<anyval>
  : detail::bits<0, cell::max_bits>
  , detail::no_refs {};
template<>
struct estimate_element<MsgAddressSlice>
  : detail::chain_to<MsgAddress> {};

// Static size types
template<unsigned _bitlen>
struct estimate_element<bitfield<_bitlen>>
  : detail::exact_size<_bitlen> {};
template<unsigned _bitlen, unsigned _code>
struct estimate_element<bitconst<_bitlen, _code>>
  : detail::exact_size<_bitlen> {};
template<unsigned _bitlen>
struct estimate_element<uint_t<_bitlen>>
  : detail::exact_size<_bitlen> {};
template<unsigned _bitlen>
struct estimate_element<int_t<_bitlen>>
  : detail::exact_size<_bitlen> {};

// Container types
template<class... Types>
struct estimate_element<std::tuple<Types...>> {
  static constexpr unsigned min_bits = (0u + ... + estimate_element<Types>::min_bits);
  static constexpr unsigned max_bits = (0u + ... + estimate_element<Types>::max_bits);
  static constexpr unsigned min_refs = (0u + ... + estimate_element<Types>::min_refs);
  static constexpr unsigned max_refs = (0u + ... + estimate_element<Types>::max_refs);
};
template<class... Types>
struct estimate_element<std::variant<Types...>> {
  static constexpr unsigned min_bits = std::min({estimate_element<Types>::min_bits ... });
  static constexpr unsigned max_bits = std::max({estimate_element<Types>::max_bits ... });
  static constexpr unsigned min_refs = std::min({estimate_element<Types>::min_refs ... });
  static constexpr unsigned max_refs = std::max({estimate_element<Types>::max_refs ... });
};
template<class X>
struct estimate_element<EitherLeft<X>> : detail::chain_to< to_std_tuple_t<EitherLeft<X>> > {};
template<class Y>
struct estimate_element<EitherRight<Y>> : detail::chain_to< to_std_tuple_t<EitherRight<Y>> > {};

template<class X, class Y>
struct estimate_element<Either<X, Y>> : detail::chain_to< to_std_tuple_t<Either<X, Y>> > {};

template<class _Tp>
struct estimate_element<lazy<_Tp>> : detail::chain_to<_Tp> {};

template<class _Tp>
struct estimate_element<std::optional<_Tp>> {
  using Est = estimate_element<_Tp>;
  static constexpr unsigned min_bits = 1;
  static constexpr unsigned max_bits = 1 + Est::max_bits;
  static constexpr unsigned min_refs = 0;
  static constexpr unsigned max_refs = Est::max_refs;
};

}} // namespace tvm::schema

