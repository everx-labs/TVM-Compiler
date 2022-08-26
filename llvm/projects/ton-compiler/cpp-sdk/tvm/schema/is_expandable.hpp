#pragma once

namespace tvm { namespace schema {

// Helper to understand if this type is an expandable structure
template<class T>
struct is_expandable : std::true_type {};

template<class... Elems>
struct is_expandable<std::tuple<Elems...>> : std::true_type {};

template<class... Elems>
struct is_expandable<std::variant<Elems...>> : std::false_type {};

template<unsigned _bitlen, unsigned _code>
struct is_expandable<bitconst<_bitlen, _code>> : std::false_type {};

template<unsigned _bitlen>
struct is_expandable<bitfield<_bitlen>> : std::false_type {};

template<unsigned _bitlen>
struct is_expandable<uint_t<_bitlen>> : std::false_type {};

template<unsigned _bitlen>
struct is_expandable<int_t<_bitlen>> : std::false_type {};

template<>
struct is_expandable<varuint<16>> : std::false_type {};

template<>
struct is_expandable<varuint<32>> : std::false_type {};

template<>
struct is_expandable<varint<16>> : std::false_type {};

template<>
struct is_expandable<varint<32>> : std::false_type {};

template<unsigned _keylen, class _element_type>
struct is_expandable<HashmapE<_keylen, _element_type>> : std::false_type {};

template<>
struct is_expandable<anydict> : std::false_type {};

template<class _Tp>
struct is_expandable<ref<_Tp>> : std::false_type {};

template<>
struct is_expandable<cell> : std::false_type {};

template<>
struct is_expandable<anyval> : std::false_type {};

template<>
struct is_expandable<MsgAddressSlice> : std::false_type {};

template<class _Tp>
struct is_expandable<lazy<_Tp>> : std::false_type {};

template<>
struct is_expandable<empty> : std::false_type {};

template<class _Tp>
struct is_expandable<optional<_Tp>> : std::false_type {};

template<class _Tp>
const bool is_expandable_v = is_expandable<_Tp>::value;

}} // namespace tvm::schema

