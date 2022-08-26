#pragma once

#include <tvm/dict_traits.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
struct small_dictionary_map_const_iterator : boost::operators<small_dictionary_map_const_iterator<Element, KeyLen>> {
  using Pair = std::pair<schema::uint_t<KeyLen>, Element>;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = const std::pair<schema::uint_t<KeyLen>, Element>*;
  using reference = const std::pair<schema::uint_t<KeyLen>, Element>&;

  dictionary dict_;
  unsigned idx_;
  std::optional<slice> sl_;

  __always_inline Pair operator*() const {
    require(!!sl_, error_code::iterator_overflow);
    return {schema::uint_t<KeyLen>(idx_), schema::parse<Element>(*sl_)};
  }
  __always_inline bool is_end() const { return !sl_; }

  static small_dictionary_map_const_iterator create_begin(dictionary dict) {
    std::optional<slice> opt_sl;
    auto [sl, idx, succ] = dict.dictumin(KeyLen);
    if (succ)
      opt_sl = sl;
    return small_dictionary_map_const_iterator{{}, dict, idx, opt_sl};
  }
  static small_dictionary_map_const_iterator create_end(dictionary dict) {
    return small_dictionary_map_const_iterator{{}, dict, 0, {}};
  }

  bool operator<(small_dictionary_map_const_iterator x) const { return idx_ < x.idx_; }

  small_dictionary_map_const_iterator& operator++() {
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = next_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  small_dictionary_map_const_iterator& operator--() {
    auto [sl, prev_idx, succ] = dict_.dictugetprev(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = prev_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  bool operator==(small_dictionary_map_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};

template<class Element, unsigned KeyLen>
struct big_dictionary_map_const_iterator : boost::operators<big_dictionary_map_const_iterator<Element, KeyLen>> {
  using Pair = std::pair<schema::uint_t<KeyLen>, Element>;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = const std::pair<schema::uint_t<KeyLen>, Element>*;
  using reference = const std::pair<schema::uint_t<KeyLen>, Element>&;

  dictionary dict_;
  unsigned idx_;
  std::optional<cell> cl_;

  __always_inline Pair operator*() const {
    require(!!cl_, error_code::iterator_overflow);
    return {schema::uint_t<KeyLen>(idx_), parse_chain<Element,0,0>(parser(*cl_))};
  }
  __always_inline bool is_end() const { return !cl_; }

  static big_dictionary_map_const_iterator create_begin(dictionary dict) {
    std::optional<cell> opt_cl;
    auto [cl, idx, succ] = dict.dictuminref(KeyLen);
    if (succ)
      opt_cl = cl;
    return big_dictionary_map_const_iterator{{}, dict, idx, opt_cl};
  }
  static big_dictionary_map_const_iterator create_end(dictionary dict) {
    return big_dictionary_map_const_iterator{{}, dict, 0, {}};
  }

  bool operator<(big_dictionary_map_const_iterator x) const { return idx_ < x.idx_; }

  big_dictionary_map_const_iterator& operator++() {
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      cl_ = parser(sl).ldref();
      idx_ = next_idx;
    } else {
      cl_.reset();
    }
    return *this;
  }
  big_dictionary_map_const_iterator& operator--() {
    auto [sl, prev_idx, succ] = dict_.dictugetprev(idx_, KeyLen);
    if (succ) {
      cl_ = parser(sl).ldref();
      idx_ = prev_idx;
    } else {
      cl_.reset();
    }
    return *this;
  }
  bool operator==(big_dictionary_map_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};

template<class Element, unsigned KeyLen>
using dictionary_map_const_iterator =
  std::conditional_t<small_element<Element, KeyLen>,
                     small_dictionary_map_const_iterator<Element, KeyLen>,
                     big_dictionary_map_const_iterator<Element, KeyLen>>;

} // namespace tvm

