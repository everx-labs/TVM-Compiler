#pragma once

#include <tvm/sequence.hpp>

namespace tvm {

__always_inline
constexpr size_t strlength(const char* str) {
  size_t __len = 0;
  for (; str[__len]; ++__len);
  return __len;
}

template<unsigned Count, class Iterator>
__always_inline
constexpr unsigned combine_bytes(Iterator it) {
  constexpr unsigned BYTE_SZ = 8;
  if constexpr (Count > 1)
    return (*it << (BYTE_SZ * (Count - 1))) | combine_bytes<Count - 1>(std::next(it));
  else
    return *it;
}

template<unsigned Size, class Iterator>
__always_inline
cell string_into_cells(Iterator it) {
  constexpr unsigned MAX_UINT_SZ = 256;
  constexpr unsigned BYTE_SZ = 8;
  constexpr unsigned MAX_BYTES_IN_CELL = cell::max_bits / BYTE_SZ;
  constexpr unsigned MAX_BYTES_IN_UINT = MAX_UINT_SZ / BYTE_SZ;

  builder b;
  if constexpr (Size > MAX_BYTES_IN_CELL) {
    cell next_cell = string_into_cells<Size - MAX_BYTES_IN_CELL, Iterator>(
      std::next(it, MAX_BYTES_IN_CELL));
    b.stref(next_cell);
  }
  if constexpr (Size >= MAX_BYTES_IN_UINT) {
    unsigned value = combine_bytes<MAX_BYTES_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_BYTES_IN_UINT);
  }
  if constexpr (Size >= 2 * MAX_BYTES_IN_UINT) {
    unsigned value = combine_bytes<MAX_BYTES_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_BYTES_IN_UINT);
  }
  if constexpr (Size >= 3 * MAX_BYTES_IN_UINT) {
    unsigned value = combine_bytes<MAX_BYTES_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_BYTES_IN_UINT);
  }
  constexpr auto RestSz = std::min(Size, MAX_BYTES_IN_CELL) % MAX_BYTES_IN_UINT;
  if constexpr (RestSz) {
    unsigned value = combine_bytes<RestSz>(it);
    b.stu(value, RestSz * BYTE_SZ);
  }
  return b.make_cell();
}

struct string {
  using Element = uint8;

  // WARNING: this is not constructor of empty string, this is null string
  // use string::create() for empty string construction
  __always_inline
  string() {}

  __always_inline
  string(cell cl) : data_(cl) {}

  __always_inline
  string(bytes data) : data_(data) {}

  __always_inline
  string(std::initializer_list<Element> il) {
    data_.assign(il.begin(), il.end());
  }

  __always_inline
  string(std::initializer_list<int> il) {
    data_.assign(il.begin(), il.end());
  }

  template <std::size_t N>
  __always_inline
  constexpr string(const char (&str) [N]) {
    data_ = bytes(string_into_cells<N>(str));
  }

  __always_inline
  string& operator=(std::initializer_list<Element> il) {
    data_.assign(il.begin(), il.end());
    return *this;
  }

  __always_inline
  string& operator=(std::initializer_list<int> il) {
    data_.assign(il.begin(), il.end());
    return *this;
  }

  template <std::size_t N>
  __always_inline
  constexpr string& operator=(const char (&str) [N]) {
    data_ = bytes(string_into_cells<N>(str));
    return *this;
  }

  __always_inline
  static string create() {
    return string(builder().make_cell());
  }

  using const_iterator = bytes::const_iterator;

  const_iterator begin() const { return const_iterator::create_begin(data_); }
  const_iterator end() const { return const_iterator::create_end(data_); }

  bytes data_;
};

} // namespace tvm

