#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include <tvm/cell.hpp>
#include <tvm/slice.hpp>
#include <tvm/tuple.hpp>

#include <tvm/to_std_tuple.hpp>

namespace tvm { namespace schema {

// TODO: Merge with upstream to have default comparison operators
#ifdef DEFAULT_EQUAL
#error Overriding DEFAULT_EQUAL macros
#endif
#define DEFAULT_EQUAL(T)                             \
  bool operator==(const T& v) const {                \
    return to_std_tuple(*this) == to_std_tuple(v);   \
  }                                                  \
  bool operator!=(const T& v) const {                \
    return to_std_tuple(*this) != to_std_tuple(v);   \
  }

template<unsigned _bitlen, unsigned _code>
struct bitconst {
  constexpr bool operator==(bitconst<_bitlen, _code>) const {
    return true;
  }
};

template<unsigned _bitlen>
struct bitfield {
  slice operator()() const { return sl_; }
  void operator()(slice sl) { sl_ = sl; }
  auto& operator=(slice sl) { sl_ = sl; return *this; }
  DEFAULT_EQUAL(bitfield)
  slice sl_;
};

template<unsigned _bitlen>
struct int_t {
  int_t() : val_(0) {}
  int_t(int val) : val_(val) {}
  int operator()() const { return val_; }
  void operator()(int val) { val_ = val; }
  auto& operator=(int val) { val_ = val; return *this; }
  auto& operator+=(int val) { val_ += val; return *this; }
  auto& operator-=(int val) { val_ -= val; return *this; }
  operator int() const { return val_; }
  DEFAULT_EQUAL(int_t)
  int val_;
};

template<unsigned _bitlen>
struct uint_t {
  uint_t() : val_(0) {}
  uint_t(unsigned val) : val_(val) {}
  unsigned operator()() const { return val_; }
  void operator()(unsigned val) { val_ = val; }
  auto& operator=(unsigned val) { val_ = val; return *this; }
  auto& operator+=(unsigned val) { val_ += val; return *this; }
  auto& operator-=(unsigned val) { val_ -= val; return *this; }
  operator unsigned() const { return val_; }
  DEFAULT_EQUAL(uint_t)
  unsigned val_;
};

using int8_t = int_t<8>;
using int16_t = int_t<16>;
using int32_t = int_t<32>;
using int64_t = int_t<64>;
using int128_t = int_t<128>;
using int256_t = int_t<256>;

using bool_t = uint_t<1>;
using uint8_t = uint_t<8>;
using uint16_t = uint_t<16>;
using uint32_t = uint_t<32>;
using uint64_t = uint_t<64>;
using uint128_t = uint_t<128>;
using uint256_t = uint_t<256>;

template<auto _Field>
struct dynamic_bitfield {
  dynamic_bitfield() : bitlen_(0) {}
  dynamic_bitfield(slice sl, unsigned bitlen) : sl_(sl), bitlen_(bitlen) {}
  slice operator()() const { return sl_; }
  template<class _ParentT>
  void operator()(slice sl, _ParentT& parent) {
    sl_ = sl;
    bitlen_ = sl.sbits();
    // setting bitlen for this field into another field
    auto &fieldref = (&parent)->*_Field;
    fieldref(bitlen_);
  }
  DEFAULT_EQUAL(dynamic_bitfield)
  slice sl_;
  unsigned bitlen_;
};

template<auto _Field>
struct dynamic_uint {
  dynamic_uint() : val_(0), bitlen_(0) {}
  dynamic_uint(unsigned val, unsigned bitlen) : val_(val), bitlen_(bitlen) {}
  unsigned operator()() const { return val_; }
  template<class _ParentT>
  void operator()(unsigned val, unsigned bitlen, _ParentT& parent) {
    val_ = val;
    bitlen_ = bitlen;
    // setting bitlen for this field into another field
    auto &fieldref = (&parent)->*_Field;
    fieldref(bitlen_);
  }
  DEFAULT_EQUAL(dynamic_uint)
  unsigned val_;
  unsigned bitlen_;
};

template<auto _Field>
struct dynamic_int {
  dynamic_int() : val_(0), bitlen_(0) {}
  dynamic_int(int val, unsigned bitlen) : val_(val), bitlen_(bitlen) {}
  int operator()() const { return val_; }
  template<class _ParentT>
  void operator()(int val, unsigned bitlen, _ParentT& parent) {
    val_ = val;
    bitlen_ = bitlen;
    // setting bitlen for this field into another field
    auto &fieldref = (&parent)->*_Field;
    fieldref(bitlen_);
  }
  DEFAULT_EQUAL(dynamic_int)
  int val_;
  unsigned bitlen_;
};

template<unsigned _bitlen>
struct varuint {
  varuint() : val_(0) {}
  varuint(unsigned val) : val_(val) {}
  unsigned operator()() const { return val_; }
  void operator()(unsigned val) { val_ = val; }
  auto& operator=(unsigned val) { val_ = val; return *this; }
  DEFAULT_EQUAL(varuint)
  unsigned val_;
};

template<unsigned _bitlen>
struct varint {
  varint() : val_(0) {}
  varint(int val) : val_(val) {}
  int operator()() const { return val_; }
  void operator()(int val) { val_ = val; }
  auto& operator=(int val) { val_ = val; return *this; }
  DEFAULT_EQUAL(varint)
  int val_;
};

using varuint16 = varuint<16>;
using varuint32 = varuint<32>;
using varint16 = varint<16>;
using varint32 = varint<32>;

using Grams = varuint16;

template<unsigned _keylen, class _element_type>
struct HashmapE {
  cell operator()() const { return dict_; }
  void operator()(cell dict) { dict_ = dict; }
  auto& operator=(cell dict) { dict_ = dict; return *this; }
  DEFAULT_EQUAL(HashmapE)
  cell dict_;
};

template<class X>
struct EitherLeft {
  bitconst<1, 0b0> tag;
  X val;

  DEFAULT_EQUAL(EitherLeft)
};
template<class Y>
struct EitherRight {
  bitconst<1, 0b1> tag;
  Y val;

  DEFAULT_EQUAL(EitherRight)
};
template<typename X, typename Y>
struct Either {
  using base_t = std::variant<EitherLeft<X>, EitherRight<Y>>;

  Either& operator=(X left) {
    val_ = EitherLeft<X>{ {}, left };
    return *this;
  }
  Either& operator=(Y right) {
    val_ = EitherRight<X>{ {}, right };
    return *this;
  }

  bool operator == (const Either& v) const {
    return val_ == v.val_;
  }
  base_t operator()() const { return val_; }
  base_t val_;
};

template<typename _Tp>
struct ref {
  _Tp operator()() const { return val_; }
  void operator()(_Tp val) { val_ = val; }
  auto& operator=(_Tp val) { val_ = val; return *this; }

  DEFAULT_EQUAL(ref)
  _Tp val_;
};

struct anyref {
  cell operator()() const { return val_; }
  void operator()(cell val) { val_ = val; }
  auto& operator=(cell val) { val_ = val; return *this; }
  cell val_;
};

struct anyval {
  slice operator()() const { return val_; }
  void operator()(slice val) { val_ = val; }
  auto& operator=(slice val) { val_ = val; return *this; }
  DEFAULT_EQUAL(anyval)
  slice val_;
};

struct MsgAddressSlice {
  slice operator()() const { return sl_; }
  void operator()(slice sl) { sl_ = sl; }
  auto& operator=(slice sl) { sl_ = sl; return *this; }
  DEFAULT_EQUAL(MsgAddressSlice)
  slice sl_;
};

template<typename _Tp>
struct lazy {
  using Tup = tvm::tuple<_Tp>;
  __always_inline _Tp operator()(); // implemented in make_parser.hpp

  void operator()(_Tp val) { val_ = Tup(val); }
  void operator=(_Tp val) { val_ = Tup(val); }
  void operator=(slice sl) { val_ = sl; }

  bool is_slice() const { return std::holds_alternative<slice>(val_); }
  bool is_val() const { return std::holds_alternative<Tup>(val_); }
  slice raw_slice() const { return std::get<slice>(val_); }

  // For already parsed values (ensure is_val() before)
  __always_inline _Tp raw_val() const { return std::get<Tup>(val_).unpack(); }

  // Performs parsing if necessary (if in slice state)
  __always_inline _Tp val() { return (*this)(); }

  DEFAULT_EQUAL(lazy<_Tp>)
  std::variant<slice, tvm::tuple<_Tp>> val_;
};

struct empty {};

template<class _Tp>
struct make_parser_impl;

template<class _Tp>
struct make_builder_impl;

template<class _ParentStruct, class _ParentTuple>
struct ParseContext : _ParentTuple {
  const _ParentTuple& get_tuple() const {
    return *static_cast<const _ParentTuple*>(this);
  }
};

using std::optional;
using std::variant;

}} // namespace tvm::schema
