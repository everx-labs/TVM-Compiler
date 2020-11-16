#pragma once

#include <tvm/tuple_stack.hpp>
#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/get_bitsize.hpp>
#include <experimental/type_traits>

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

// Array represented in flat sequence of elements
// Default constructor contains null cell (for optimization of arguments parsing)
template<class Element>
class sequence {
  static constexpr unsigned max_cell_bits = 1023;

  static_assert(schema::get_bitsize<Element>::value < max_cell_bits);
public:
  // WARNING: this is not constructor of empty sequence, this is null sequence
  // use sequence::create() for empty sequence construction
  __always_inline
  sequence() {}
  __always_inline
  sequence(cell cl) : cl_(cl) {}
  __always_inline
  sequence(std::initializer_list<Element> il) {
    assign(il.begin(), il.end());
  }
  __always_inline
  sequence(std::initializer_list<int> il) {
    assign(il.begin(), il.end());
  }
  template <std::size_t N>
  __always_inline
  constexpr sequence(const char (&str) [N]) {
    cl_ = string_into_cells<N>(str);
  }

  __always_inline
  sequence& operator=(std::initializer_list<Element> il) {
    assign(il.begin(), il.end());
    return *this;
  }
  __always_inline
  sequence& operator=(std::initializer_list<int> il) {
    assign(il.begin(), il.end());
    return *this;
  }
  template <std::size_t N>
  __always_inline
  constexpr sequence& operator=(const char (&str) [N]) {
    cl_ = string_into_cells<N>(str);
    return *this;
  }

  __always_inline
  static sequence create() {
    return sequence(builder().make_cell());
  }

  template<class _Iterator>
  __always_inline
  void assign(_Iterator __first, _Iterator __last) {
    tuple_stack<builder> bldrs;
    bldrs.push(builder());
    unsigned rem_bits = max_cell_bits;
    for (; __first != __last; ++__first) {
      Element elem(*__first);
      if constexpr (schema::get_bitsize_v<Element>) {
        constexpr unsigned new_bits = schema::get_bitsize_v<Element>;
        if (new_bits > rem_bits) {
          bldrs.push(builder());
          rem_bits = max_cell_bits;
        }
        bldrs.top() = schema::build(bldrs.top(), elem);
        rem_bits -= new_bits;
      } else {
        slice elem_sl = schema::build(elem).make_slice();
        unsigned new_bits = elem_sl.sbits();
        if (new_bits > rem_bits) {
          bldrs.push(builder());
          rem_bits = max_cell_bits;
        }
        bldrs.top().stslice(elem_sl);
        rem_bits -= new_bits;
      }
    }

    cell next_cell = bldrs.top().make_cell();
    bldrs.pop();
    while (!bldrs.empty()) {
      bldrs.top().stref(next_cell);
      next_cell = bldrs.top().make_cell();
      bldrs.pop();
    }
    cl_ = next_cell;
  }

  template<class Func>
  __always_inline
  void enumerate(Func func) {
    slice cur_sl = cl_.ctos();

    do {
      parser p(cur_sl);
      while (p.sl().sbits()) {
        auto [opt_val, =p] = schema::parse_continue<Element>(p);
        if (opt_val)
          func(*opt_val);
        else
          break;
      }
      if (p.sl().srefs()) {
        cur_sl = p.ldrefrtos();
        p.ends();
        continue;
      }
      p.ends();
      break;
    } while(true);
  }

  struct const_iterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = Element;
    using difference_type = int;
    using pointer = Element*;
    using reference = Element&;

    parser p_;
    std::optional<Element> elem_;

    __always_inline Element operator*() const {
      return *elem_;
    }

    __always_inline bool is_end() const { return !elem_; }

    static const_iterator create_begin(sequence arr) {
      parser p(arr.cl_.ctos());
      if (!p.sl().sbits())
        return {};
      auto [elem, =p] = schema::parse_continue<Element>(p);
      return const_iterator{p, elem};
    }
    static const_iterator create_end(sequence arr) {
      return {};
    }

    __always_inline const_iterator operator++() {
      require(!!elem_, error_code::iterator_overflow);
      auto [bits, refs] = p_.sl().sbitrefs();
      if (!bits) {
        if (!refs) {
          elem_.reset();
          return *this;
        }
        p_ = parser(p_.ldrefrtos());
      }
      auto [=elem_, =p_] = schema::parse_continue<Element>(p_);
      return *this;
    }
    __always_inline
    bool operator==(const_iterator v) const {
      bool left_end = is_end();
      bool right_end = v.is_end();
      return left_end && right_end;
    }
    __always_inline
    bool operator!=(const_iterator v) const {
      return !(*this == v);
    }
  };

  const_iterator begin() const { return const_iterator::create_begin(*this); }
  const_iterator end() const { return const_iterator::create_end(*this); }

  cell cl_;
};

using bytes = sequence<schema::uint8>;

} // namespace tvm

