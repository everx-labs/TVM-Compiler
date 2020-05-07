#pragma once

namespace tvm {

namespace schema {

template<unsigned _bitlen, unsigned _code>
struct bitconst {};

template<unsigned _bitlen, bool _sign = false>
struct bitfield {};

using int8 = bitfield<8, true>;
using int16 = bitfield<16, true>;
using int32 = bitfield<32, true>;
using int64 = bitfield<64, true>;

using uint8 = bitfield<8, false>;
using uint16 = bitfield<16, false>;
using uint32 = bitfield<32, false>;
using uint64 = bitfield<64, false>;

template<typename _Tp>
struct dynamic_bitfield {
  static_assert("dynamic_bitfield works only with member object fields (previously declared in parent struct)");
};
template<typename _Tp, typename _Up>
struct dynamic_bitfield<_Tp _Up::*> {};

template<unsigned _bitlen>
struct varuint {};

template<unsigned _bitlen>
struct varint {};

using varuint16 = varuint<16>;
using varuint32 = varuint<32>;
using varint16 = varint<16>;
using varint32 = varint<32>;

using Grams = varuint16;

template<unsigned _keylen, typename _element_type>
struct HashmapE {};

template<typename X, typename Y>
using Either = variant<seq<bitconst<1, 0b>, X>, seq<bitconst<1, 1b>, Y>>;

template<typename _Tp>
struct ref {};

} // namespace schema

} // namespace tvm

