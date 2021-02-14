#pragma once

#include <optional>
#include <tuple>
#include <variant>

#include <boost/operators.hpp>

#include <tvm/cell.hpp>
#include <tvm/slice.hpp>

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

#ifdef DEFAULT_PROXY_OPERATORS
#error Overriding DEFAULT_PROXY_OPERATORS macros
#endif
#define DEFAULT_PROXY_OPERATORS(T, V)                                                       \
  bool operator<(T x) const { return val_ < x.val_; }                                       \
  bool operator<(V x) const { return val_ < x; }                                            \
  template<unsigned _len> bool operator<(T<_len> x) const { return val_ < x.val_; }         \
  bool operator<=(T x) const { return val_ <= x.val_; }                                     \
  bool operator<=(V x) const { return val_ <= x; }                                          \
  template<unsigned _len> bool operator<=(T<_len> x) const { return val_ <= x.val_; }       \
  bool operator>(T x) const { return val_ > x.val_; }                                       \
  bool operator>(V x) const { return val_ > x; }                                            \
  template<unsigned _len> bool operator>(T<_len> x) const { return val_ > x.val_; }         \
  bool operator>=(T x) const { return val_ >= x.val_; }                                     \
  bool operator>=(V x) const { return val_ >= x; }                                          \
  template<unsigned _len> bool operator>=(T<_len> x) const { return val_ >= x.val_; }       \
  bool operator==(T x) const { return val_ == x.val_; }                                     \
  bool operator==(V x) const { return val_ == x; }                                          \
  template<unsigned _len> bool operator==(T<_len> x) const { return val_ == x.val_; }       \
  bool operator!=(T x) const { return val_ != x.val_; }                                     \
  bool operator!=(V x) const { return val_ != x; }                                          \
  template<unsigned _len> bool operator!=(T<_len> x) const { return val_ != x.val_; }       \
  T& operator+=(T x) { val_ += x.val_; return *this; }                                      \
  T& operator+=(V x) { val_ += x; return *this; }                                           \
  template<unsigned _len> T& operator+=(T<_len> x) { val_ += x.val_; return *this; }        \
  T& operator-=(T x) { val_ -= x.val_; return *this; }                                      \
  T& operator-=(V x) { val_ -= x; return *this; }                                           \
  template<unsigned _len> T& operator-=(T<_len> x) { val_ -= x.val_; return *this; }        \
  T& operator*=(T x) { val_ *= x.val_; return *this; }                                      \
  T& operator*=(V x) { val_ *= x; return *this; }                                           \
  template<unsigned _len> T& operator*=(T<_len> x) { val_ *= x.val_; return *this; }        \
  T& operator/=(T x) { val_ /= x.val_; return *this; }                                      \
  T& operator/=(V x) { val_ /= x; return *this; }                                           \
  template<unsigned _len> T& operator/=(T<_len> x) { val_ /= x.val_; return *this; }        \
  T& operator%=(T x) { val_ %= x.val_; return *this; }                                      \
  T& operator%=(V x) { val_ %= x; return *this; }                                           \
  template<unsigned _len> T& operator%=(T<_len> x) { val_ %= x.val_; return *this; }        \
  T& operator|=(T x) { val_ |= x.val_; return *this; }                                      \
  T& operator|=(V x) { val_ |= x; return *this; }                                           \
  template<unsigned _len> T& operator|=(T<_len> x) { val_ |= x.val_; return *this; }        \
  T& operator&=(T x) { val_ &= x.val_; return *this; }                                      \
  T& operator&=(V x) { val_ &= x; return *this; }                                           \
  template<unsigned _len> T& operator&=(T<_len> x) { val_ &= x.val_; return *this; }        \
  T& operator^=(T x) { val_ ^= x.val_; return *this; }                                      \
  T& operator^=(V x) { val_ ^= x; return *this; }                                           \
  template<unsigned _len> T& operator^=(T<_len> x) { val_ ^= x.val_; return *this; }        \
  T& operator++() { ++val_; return *this; }                                                 \
  T& operator--() { --val_; return *this; }                                                 \
  T operator++(int) { T old(*this); ++val_; return old; }                                   \
  T operator--(int) { T old(*this); --val_; return old; }                                   \
  T operator~() const { return T(~val_); }

template<unsigned _bitlen, unsigned _code>
struct bitconst {
  static constexpr unsigned code = _code;
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
  static constexpr unsigned max_bits = 257;

  constexpr int_t() : val_(0) {}
  explicit constexpr int_t(int val) : val_(val) {}
  int operator()() const { return val_; }
  void operator()(int val) { val_ = val; }
  auto& operator=(int val) { val_ = val; return *this; }
  template<unsigned _len>
  auto& operator=(int_t<_len> val) { val_ = val.get(); return *this; }
  DEFAULT_PROXY_OPERATORS(int_t, int)
  int get() const { return val_; }
  explicit operator bool() const { return val_ != 0; }
  explicit operator int() const { return val_; }
  int val_;
};

template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator+(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ + r.val_);
}
template<unsigned _len>
__always_inline auto operator+(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ + r);
}
template<unsigned _len>
__always_inline auto operator+(unsigned l, int_t<_len> r) {
  return int_t<_len>(l + r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator-(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ - r.val_);
}
template<unsigned _len>
__always_inline auto operator-(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ - r);
}
template<unsigned _len>
__always_inline auto operator-(unsigned l, int_t<_len> r) {
  return int_t<_len>(l - r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator*(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ * r.val_);
}
__always_inline auto operator*(int_t<8> l, int_t<8> r) { return int_t<8>{l.val_ * r.val_}; }
__always_inline auto operator*(int_t<16> l, int_t<16> r) { return int_t<16>{l.val_ * r.val_}; }
__always_inline auto operator*(int_t<32> l, int_t<32> r) { return int_t<32>{l.val_ * r.val_}; }
__always_inline auto operator*(int_t<64> l, int_t<64> r) { return int_t<64>{l.val_ * r.val_}; }
__always_inline auto operator*(int_t<128> l, int_t<128> r) { return int_t<128>{l.val_ * r.val_}; }
template<unsigned _len>
__always_inline auto operator*(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ * r);
}
template<unsigned _len>
__always_inline auto operator*(unsigned l, int_t<_len> r) {
  return int_t<_len>(l * r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator/(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ / r.val_);
}
template<unsigned _len>
__always_inline auto operator/(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ / r);
}
template<unsigned _len>
__always_inline auto operator/(unsigned l, int_t<_len> r) {
  return int_t<_len>(l / r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator%(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ % r.val_);
}
template<unsigned _len>
__always_inline auto operator%(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ % r);
}
template<unsigned _len>
__always_inline auto operator%(unsigned l, int_t<_len> r) {
  return int_t<_len>(l % r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator|(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ | r.val_);
}
template<unsigned _len>
__always_inline auto operator|(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ | r);
}
template<unsigned _len>
__always_inline auto operator|(unsigned l, int_t<_len> r) {
  return int_t<_len>(l | r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator&(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ & r.val_);
}
template<unsigned _len>
__always_inline auto operator&(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ & r);
}
template<unsigned _len>
__always_inline auto operator&(unsigned l, int_t<_len> r) {
  return int_t<_len>(l & r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator^(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ ^ r.val_);
}
template<unsigned _len>
__always_inline auto operator^(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ ^ r);
}
template<unsigned _len>
__always_inline auto operator^(unsigned l, int_t<_len> r) {
  return int_t<_len>(l ^ r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator<<(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ << r.val_);
}
template<unsigned _len>
__always_inline auto operator<<(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ << r);
}
template<unsigned _len>
__always_inline auto operator<<(unsigned l, int_t<_len> r) {
  return int_t<_len>(l << r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator>>(int_t<_left_len> l, int_t<_right_len> r) {
  return int_t<std::max(_left_len, _right_len)>(l.val_ >> r.val_);
}
template<unsigned _len>
__always_inline auto operator>>(int_t<_len> l, unsigned r) {
  return int_t<_len>(l.val_ >> r);
}
template<unsigned _len>
__always_inline auto operator>>(unsigned l, int_t<_len> r) {
  return int_t<_len>(l >> r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator==(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ == r.val_;
}
template<unsigned _len>
__always_inline bool operator==(unsigned l, int_t<_len> r) {
  return l == r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator!=(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ != r.val_;
}
template<unsigned _len>
__always_inline bool operator!=(unsigned l, int_t<_len> r) {
  return l != r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator<(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ < r.val_;
}
template<unsigned _len>
__always_inline bool operator<(unsigned l, int_t<_len> r) {
  return l < r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator<=(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ <= r.val_;
}
template<unsigned _len>
__always_inline bool operator<=(unsigned l, int_t<_len> r) {
  return l <= r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator>(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ > r.val_;
}
template<unsigned _len>
__always_inline bool operator>(unsigned l, int_t<_len> r) {
  return l > r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator>=(int_t<_left_len> l, int_t<_right_len> r) {
  return l.val_ >= r.val_;
}
template<unsigned _len>
__always_inline bool operator>=(unsigned l, int_t<_len> r) {
  return l >= r.val_;
}

template<unsigned _bitlen>
struct uint_t {
  static constexpr unsigned max_bits = 256;
  static constexpr unsigned bitlen = _bitlen;

  constexpr uint_t() : val_(0) {}
  explicit constexpr uint_t(unsigned val) : val_(val) {}
  template<unsigned _len>
  explicit constexpr  uint_t(uint_t<_len> val) : val_(val.get()) {}
  unsigned operator()() const { return val_; }
  void operator()(unsigned val) { val_ = val; }
  constexpr auto& operator=(unsigned val) { val_ = val; return *this; }
  template<unsigned _len>
  auto& operator=(uint_t<_len> val) { val_ = val.get(); return *this; }
  DEFAULT_PROXY_OPERATORS(uint_t, unsigned)
  unsigned get() const { return val_; }
  explicit operator bool() const { return val_ != 0; }
  explicit operator unsigned() const { return val_; }
  unsigned val_;
};

template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator+(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ + r.val_);
}
template<unsigned _len>
__always_inline auto operator+(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ + r);
}
template<unsigned _len>
__always_inline auto operator+(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l + r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator-(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ - r.val_);
}
template<unsigned _len>
__always_inline auto operator-(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ - r);
}
template<unsigned _len>
__always_inline auto operator-(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l - r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator*(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ * r.val_);
}
template<unsigned _len>
__always_inline auto operator*(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ * r);
}
template<unsigned _len>
__always_inline auto operator*(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l * r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator/(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ / r.val_);
}
template<unsigned _len>
__always_inline auto operator/(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ / r);
}
template<unsigned _len>
__always_inline auto operator/(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l / r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator%(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ % r.val_);
}
template<unsigned _len>
__always_inline auto operator%(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ % r);
}
template<unsigned _len>
__always_inline auto operator%(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l % r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator|(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ | r.val_);
}
template<unsigned _len>
__always_inline auto operator|(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ | r);
}
template<unsigned _len>
__always_inline auto operator|(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l | r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator&(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ & r.val_);
}
template<unsigned _len>
__always_inline auto operator&(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ & r);
}
template<unsigned _len>
__always_inline auto operator&(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l & r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator^(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ ^ r.val_);
}
template<unsigned _len>
__always_inline auto operator^(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ ^ r);
}
template<unsigned _len>
__always_inline auto operator^(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l ^ r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator<<(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ << r.val_);
}
template<unsigned _len>
__always_inline auto operator<<(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ << r);
}
template<unsigned _len>
__always_inline auto operator<<(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l << r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline auto operator>>(uint_t<_left_len> l, uint_t<_right_len> r) {
  return uint_t<std::max(_left_len, _right_len)>(l.val_ >> r.val_);
}
template<unsigned _len>
__always_inline auto operator>>(uint_t<_len> l, unsigned r) {
  return uint_t<_len>(l.val_ >> r);
}
template<unsigned _len>
__always_inline auto operator>>(unsigned l, uint_t<_len> r) {
  return uint_t<_len>(l >> r.val_);
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator==(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ == r.val_;
}
template<unsigned _len>
__always_inline bool operator==(unsigned l, uint_t<_len> r) {
  return l == r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator!=(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ != r.val_;
}
template<unsigned _len>
__always_inline bool operator!=(unsigned l, uint_t<_len> r) {
  return l != r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator<(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ < r.val_;
}
template<unsigned _len>
__always_inline bool operator<(unsigned l, uint_t<_len> r) {
  return l < r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator<=(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ <= r.val_;
}
template<unsigned _len>
__always_inline bool operator<=(unsigned l, uint_t<_len> r) {
  return l <= r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator>(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ > r.val_;
}
template<unsigned _len>
__always_inline bool operator>(unsigned l, uint_t<_len> r) {
  return l > r.val_;
}
template<unsigned _left_len, unsigned _right_len>
__always_inline bool operator>=(uint_t<_left_len> l, uint_t<_right_len> r) {
  return l.val_ >= r.val_;
}
template<unsigned _len>
__always_inline bool operator>=(unsigned l, uint_t<_len> r) {
  return l >= r.val_;
}

using int8 = int_t<8>;
using int16 = int_t<16>;
using int32 = int_t<32>;
using int64 = int_t<64>;
using int128 = int_t<128>;
using int256 = int_t<256>;

using bool_t = uint_t<1>;
using uint8 = uint_t<8>;
using uint16 = uint_t<16>;
using uint32 = uint_t<32>;
using uint64 = uint_t<64>;
using uint128 = uint_t<128>;
using uint256 = uint_t<256>;

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
struct varuint : boost::operators<varuint<_bitlen>> {
  varuint() : val_(0) {}
  varuint(unsigned val) : val_(val) {}
  unsigned operator()() const { return val_; }
  unsigned get() const { return val_; }
  void operator()(unsigned val) { val_ = val; }
  auto& operator=(unsigned val) { val_ = val; return *this; }
  DEFAULT_PROXY_OPERATORS(varuint, unsigned)
  unsigned val_;
};

template<unsigned _bitlen>
struct varint : boost::operators<varint<_bitlen>> {
  varint() : val_(0) {}
  varint(int val) : val_(val) {}
  int operator()() const { return val_; }
  int get() const { return val_; }
  void operator()(int val) { val_ = val; }
  auto& operator=(int val) { val_ = val; return *this; }
  DEFAULT_PROXY_OPERATORS(varint, int)
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
struct anydict {
  cell operator()() const { return dict_; }
  void operator()(cell dict) { dict_ = dict; }
  auto& operator=(cell dict) { dict_ = dict; return *this; }
  __always_inline operator bool() const { return !dict_.isnull(); }
  cell get() const { return dict_; }
  cell dict_;
};
using optcell = anydict;

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

  Either() {}
  Either(X left) : val_(EitherLeft<X>{ {}, left }) {}
  Either(Y right) : val_(EitherRight<Y>{ {}, right }) {}

  Either& operator=(X left) {
    val_ = EitherLeft<X>{ {}, left };
    return *this;
  }
  Either& operator=(Y right) {
    val_ = EitherRight<Y>{ {}, right };
    return *this;
  }

  bool operator == (const Either& v) const {
    return val_ == v.val_;
  }
  template<class T>
  __always_inline bool isa() const {
    if constexpr (std::is_same_v<T, X>)
      return std::holds_alternative<EitherLeft<X>>(val_);
    else if constexpr (std::is_same_v<T, Y>)
      return std::holds_alternative<EitherRight<Y>>(val_);
    else
      return false;
  }
  template<class T>
  __always_inline T get() const {
    if constexpr (std::is_same_v<T, X>)
      return std::get<EitherLeft<X>>(val_).val;
    else if constexpr (std::is_same_v<T, Y>)
      return std::get<EitherRight<Y>>(val_).val;
    else
      static_assert(std::is_same_v<T, X> || std::is_same_v<T, Y>,
                    "bad get in Either variant");
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
  __always_inline _Tp operator()() const;  // implemented in make_parser.hpp
  __always_inline _Tp val() const;         // implemented in make_parser.hpp
  __always_inline slice sl() const {
    return sl_;
  }

  __always_inline lazy(slice sl) : sl_(sl) {}

  __always_inline lazy() {}
  __always_inline lazy(_Tp val);           // implemented in make_builder.hpp
  __always_inline void operator=(_Tp val); // implemented in make_builder.hpp
  __always_inline void operator=(slice sl) { sl_ = sl; }

  __always_inline
  static lazy make_std(int8 workchain, uint256 addr); // implemented in make_builder.hpp

  DEFAULT_EQUAL(lazy<_Tp>)
  slice sl_;
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

template<typename _Tp>
struct resumable;

using std::optional;
using std::variant;

}} // namespace tvm::schema
