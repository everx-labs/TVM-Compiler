#pragma once

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/estimate_element.hpp>
#include <experimental/type_traits>

namespace tvm {

// Array represented in flat sequence of elements
// Default constructor contains null cell (for optimization of arguments parsing)
// Keeps last cell reference and cells are linked last-to-begin
//  (optimized for push_back)
template<class Element>
class log_sequence {
  static constexpr unsigned max_cell_bits = cell::max_bits;
  using est = schema::estimate_element<Element>;
  static_assert(est::min_bits == est::max_bits, "Only fixed size elements supported");
  static_assert(est::max_bits < max_cell_bits, "Element must fit one cell");
  static_assert(est::max_refs == 0, "Element with refs unsupported");
public:
  // WARNING: this is not constructor of empty sequence, this is null sequence
  // use log_sequence::create() for empty sequence construction
  __always_inline
  log_sequence() {}
  __always_inline
  log_sequence(cell cl) : cl_(cl) {}

  __always_inline
  void reset() {
    cl_ = builder().make_cell();
  }

  __always_inline
  static log_sequence create() {
    return log_sequence(builder().make_cell());
  }

  __always_inline
  void push_back(Element elem) {
    slice sl = cl_.ctos();
    unsigned cur_bits = sl.sbits();
    if (cur_bits == 0) {
      cl_ = build(elem).make_cell();
    } else if (cur_bits + est::max_bits < max_cell_bits) {
      cl_ = build(elem).stslice(sl).make_cell();
    } else {
      cl_ = build(builder().stref(cl_), elem).make_cell();
    }
  }

  struct const_reverse_iterator {
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

    static const_reverse_iterator create_rbegin(log_sequence arr) {
      parser p(arr.cl_.ctos());
      if (!p.sl().sbits())
        return {p, {}};
      auto [elem, =p] = schema::parse_continue<Element>(p);
      return const_reverse_iterator{p, elem};
    }
    static const_reverse_iterator create_rend(log_sequence arr) {
      return {};
    }

    __always_inline const_reverse_iterator operator++() {
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
    bool operator==(const_reverse_iterator v) const {
      bool left_end = is_end();
      bool right_end = v.is_end();
      return left_end && right_end;
    }
    __always_inline
    bool operator!=(const_reverse_iterator v) const {
      return !(*this == v);
    }
  };

  const_reverse_iterator rbegin() const { return const_reverse_iterator::create_rbegin(*this); }
  const_reverse_iterator rend() const { return const_reverse_iterator::create_rend(*this); }

  cell cl_;
};

} // namespace tvm

