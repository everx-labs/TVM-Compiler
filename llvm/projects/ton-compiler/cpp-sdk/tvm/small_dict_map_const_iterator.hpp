#pragma once

#include <tvm/schema/estimate_element.hpp>

namespace tvm {

template<class Key, class Element>
struct small_dict_map_const_iterator : boost::operators<small_dict_map_const_iterator<Key, Element>> {
  using Pair = std::pair<Key, Element>;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = const Pair*;
  using reference = const Pair&;

  static constexpr unsigned KeyLen = schema::estimate_element<Key>::max_bits;

  dictionary dict_;
  std::optional<slice> key_sl_;
  std::optional<slice> elem_sl_;

  __always_inline Pair operator*() const {
    require(key_sl_ && elem_sl_, error_code::iterator_overflow);
    return {schema::parse<Key>(*key_sl_), schema::parse<Element>(*elem_sl_)};
  }

  __always_inline bool is_end() const { return !key_sl_; }

  __always_inline
  static small_dict_map_const_iterator create_begin(dictionary dict) {
    std::optional<slice> opt_elem_sl;
    std::optional<slice> opt_key_sl;
    auto [elem_sl, key_sl, succ] = dict.dictmin(KeyLen);
    if (succ) {
      opt_elem_sl = elem_sl;
      opt_key_sl = key_sl;
    }
    return small_dict_map_const_iterator{{}, dict, opt_key_sl, opt_elem_sl};
  }

  __always_inline
  static small_dict_map_const_iterator create_end(dictionary dict) {
    return small_dict_map_const_iterator{{}, dict, {}, {}};
  }

  __always_inline
  small_dict_map_const_iterator& operator++() {
    require(key_sl_ && elem_sl_, error_code::iterator_overflow);
    auto [elem_sl, key_sl, succ] = dict_.dictgetnext(*key_sl_, KeyLen);
    if (succ) {
      elem_sl_ = elem_sl;
      key_sl_ = key_sl;
    } else {
      elem_sl_.reset();
      key_sl_.reset();
    }
    return *this;
  }

  /* TODO: implement when dictgetprev will be added
  small_dict_map_const_iterator& operator--() {
    require(key_sl_ && elem_sl_, error_code::iterator_overflow);
    auto [elem_sl, key_sl, succ] = dict_.dictgetprev(*key_sl_, KeyLen);
    if (succ) {
      elem_sl_ = elem_sl;
      key_sl_ = key_sl;
    } else {
      elem_sl_.reset();
      key_sl_.reset();
    }
    return *this;
  }
  */

  bool operator==(small_dict_map_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && key_sl_ == v.key_sl_);
  }
};

} // namespace tvm

