#pragma once

#include <tvm/sequence.hpp>

namespace tvm {

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
  string() {}

  string(cell cl) : data_(cl) {}

  string(bytes data) : data_(data) {}

  string(std::initializer_list<Element> il) {
    data_.assign(il.begin(), il.end());
  }

  string(std::initializer_list<int> il) {
    data_.assign(il.begin(), il.end());
  }

  template <std::size_t N>
  constexpr string(const char (&str) [N]) {
    data_ = bytes(string_into_cells<N - 1>(str));
  }

  /*__always_inline
  constexpr string(const char *str) {
    constexpr size_t N = __builtin_strlen(str);
    data_ = bytes(string_into_cells<N>(str));
  }*/

  string& operator=(std::initializer_list<Element> il) {
    data_.assign(il.begin(), il.end());
    return *this;
  }

  string& operator=(std::initializer_list<int> il) {
    data_.assign(il.begin(), il.end());
    return *this;
  }

  template <std::size_t N>
  constexpr string& operator=(const char (&str)[N]) {
    data_ = bytes(string_into_cells<N>(str));
    return *this;
  }

  static string create() {
    return string(builder().make_cell());
  }

  using const_iterator = bytes::const_iterator;

  const_iterator begin() const { return const_iterator::create_begin(data_); }
  const_iterator end() const { return const_iterator::create_end(data_); }

  bytes data_;
};

// ==== string with 32-bit values ====

template<unsigned Count, class Iterator>
__always_inline
constexpr unsigned combine_words32(Iterator it) {
  constexpr unsigned WORD_SZ = 32;
  if constexpr (Count > 1)
    return (*it << (WORD_SZ * (Count - 1))) | combine_words32<Count - 1>(std::next(it));
  else
    return *it;
}

template<unsigned Size, class Iterator>
__always_inline
cell string32_into_cells(Iterator it) {
  constexpr unsigned MAX_UINT_SZ = 256;
  constexpr unsigned WORD_SZ = 32;
  constexpr unsigned MAX_WORDS_IN_CELL = cell::max_bits / WORD_SZ;
  constexpr unsigned MAX_WORDS_IN_UINT = MAX_UINT_SZ / WORD_SZ;

  builder b;
  if constexpr (Size > MAX_WORDS_IN_CELL) {
    cell next_cell = string32_into_cells<Size - MAX_WORDS_IN_CELL, Iterator>(
      std::next(it, MAX_WORDS_IN_CELL));
    b.stref(next_cell);
  }
  if constexpr (Size >= MAX_WORDS_IN_UINT) {
    unsigned value = combine_words32<MAX_WORDS_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_WORDS_IN_UINT);
  }
  if constexpr (Size >= 2 * MAX_WORDS_IN_UINT) {
    unsigned value = combine_words32<MAX_WORDS_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_WORDS_IN_UINT);
  }
  if constexpr (Size >= 3 * MAX_WORDS_IN_UINT) {
    unsigned value = combine_words32<MAX_WORDS_IN_UINT>(it);
    b.stu(value, MAX_UINT_SZ);
    std::advance(it, MAX_WORDS_IN_UINT);
  }
  constexpr auto RestSz = std::min(Size, MAX_WORDS_IN_CELL) % MAX_WORDS_IN_UINT;
  if constexpr (RestSz) {
    unsigned value = combine_words32<RestSz>(it);
    b.stu(value, RestSz * WORD_SZ);
  }
  return b.make_cell();
}

struct string32 {
  using Element = uint32;

  // WARNING: this is not constructor of empty string, this is null string
  // use string::create() for empty string construction
  string32() {}

  string32(cell cl) : data_(cl) {}

  string32(sequence<uint32> data) : data_(data) {}

  //string32(std::initializer_list<Element> il) {
  //  data_.assign(il.begin(), il.end());
  //}

  //string32(std::initializer_list<int> il) {
  //  data_.assign(il.begin(), il.end());
  //}

  template <std::size_t N>
  constexpr string32(const unsigned (&str) [N]) {
    data_ = sequence<uint32>(string32_into_cells<N - 1>(str));
  }

  /*__always_inline
  constexpr string(const char *str) {
    constexpr size_t N = __builtin_strlen(str);
    data_ = bytes(string_into_cells<N>(str));
  }*/

  //string32& operator=(std::initializer_list<Element> il) {
  //  data_.assign(il.begin(), il.end());
  //  return *this;
  //}

  //string32& operator=(std::initializer_list<int> il) {
  //  data_.assign(il.begin(), il.end());
  //  return *this;
  //}

  template <std::size_t N>
  constexpr string32& operator=(const unsigned (&str)[N]) {
    data_ = sequence<uint32>(string32_into_cells<N>(str));
    return *this;
  }

  static string32 create() {
    return string32(builder().make_cell());
  }

  uint32 operator[](unsigned idx) const {
    return data_[idx];
  }

  using const_iterator = typename sequence<uint32>::const_iterator;

  const_iterator begin() const { return const_iterator::create_begin(data_); }
  const_iterator end() const { return const_iterator::create_end(data_); }

  sequence<uint32> data_;
};

} // namespace tvm

