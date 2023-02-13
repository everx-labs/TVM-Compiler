#pragma once

#include <tvm/tuple_stack.hpp>
#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/get_bitsize.hpp>
#include <experimental/type_traits>

namespace tvm {

// Array represented in flat sequence of elements
// Default constructor contains null cell (for optimization of arguments parsing)
template<class Element>
class sequence {
  static constexpr unsigned max_cell_bits = 1023;

  using est = estimate_element<Element>;
  static_assert(est::max_bits < max_cell_bits);
  static_assert(est::max_refs == 0);
  static_assert(est::min_bits == est::max_bits);
  static constexpr unsigned elements_in_cell = max_cell_bits / est::max_bits;
public:
  // WARNING: this is not constructor of empty sequence, this is null sequence
  // use sequence::create() for empty sequence construction
  sequence() {}
  sequence(cell cl) : cl_(cl) {}
  sequence(std::initializer_list<Element> il) {
    assign(il.begin(), il.end());
  }
  sequence(std::initializer_list<int> il) {
    assign(il.begin(), il.end());
  }
  sequence& operator=(std::initializer_list<Element> il) {
    assign(il.begin(), il.end());
    return *this;
  }
  sequence& operator=(std::initializer_list<int> il) {
    assign(il.begin(), il.end());
    return *this;
  }

  static sequence create() {
    return sequence(builder().make_cell());
  }

  template<class _Iterator>
  void assign(_Iterator __first, _Iterator __last) {
    tuple_stack<builder> bldrs;
    bldrs.push(builder());
    unsigned rem_bits = max_cell_bits;
    for (; __first != __last; ++__first) {
      Element elem(*__first);
      constexpr unsigned new_bits = est::max_bits;
      if (new_bits > rem_bits) {
        bldrs.push(builder());
        rem_bits = max_cell_bits;
      }
      bldrs.top() = build(bldrs.top(), elem);
      rem_bits -= new_bits;
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

  Element operator[](unsigned idx) const {
    unsigned cell_idx = idx / elements_in_cell;
    unsigned idx_in_cell = idx % elements_in_cell;
    slice cur_sl = cl_.ctos();
    for (unsigned i = 0; i < cell_idx; ++i)
      cur_sl = parser(cur_sl).ldrefrtos();
    parser p(cur_sl);
    unsigned skip_bits = idx_in_cell * est::max_bits;
    if (skip_bits)
      p.skip(skip_bits);
    return parse<Element>(p);
  }

  template<class Func>
  void enumerate(Func func) {
    slice cur_sl = cl_.ctos();

    do {
      parser p(cur_sl);
      while (p.sl().sbits()) {
        auto [opt_val, =p] = parse_continue<Element>(p);
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

    Element operator*() const {
      return *elem_;
    }

    bool is_end() const { return !elem_; }

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

    const_iterator operator++() {
      require(!!elem_, error_code::iterator_overflow);
      auto [bits, refs] = p_.sl().sbitrefs();
      if (!bits) {
        if (!refs) {
          elem_.reset();
          return *this;
        }
        p_ = parser(p_.ldrefrtos());
      }
      auto [=elem_, =p_] = parse_continue<Element>(p_);
      return *this;
    }
    bool operator==(const_iterator v) const {
      bool left_end = is_end();
      bool right_end = v.is_end();
      return left_end && right_end;
    }
    bool operator!=(const_iterator v) const {
      return !(*this == v);
    }
  };

  const_iterator begin() const { return const_iterator::create_begin(*this); }
  const_iterator end() const { return const_iterator::create_end(*this); }

  cell get() const { return cl_; }

  cell cl_;
};

using bytes = sequence<schema::uint8>;

} // namespace tvm

