#pragma once

#include <optional>
#include <tvm/schema/make_parser.hpp>

namespace tvm {

template<class Element>
class queue {
  static constexpr unsigned KeyLen = 64;
  using est_t = schema::estimate_element<std::tuple<schema::uint64, Element>>;
  static_assert(est_t::max_bits < cell::max_bits,
                "Key + Element must fit one cell");
  static_assert(est_t::max_bits == est_t::min_bits,
                "Key + Element must be fixed-size");
  static_assert(est_t::max_refs == 0,
                "Key and Element must not have references");
public:
  using value_type = Element;

  __always_inline
  std::optional<value_type> front_opt() const {
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    if (succ)
      return schema::parse<value_type>(sl);
    return {};
  }

  __always_inline
  std::optional<std::pair<unsigned, value_type>> front_with_idx_opt() const {
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    if (succ)
      return std::make_pair(idx, schema::parse<value_type>(sl));
    return {};
  }

  __always_inline
  value_type front() const {
    auto rv_opt = front_opt();
    require(!!rv_opt, error_code::iterator_overflow);
    return *rv_opt;
  }

  __always_inline
  std::pair<unsigned, value_type> front_with_idx() const {
    auto rv_opt = front_with_idx_opt();
    require(!!rv_opt, error_code::iterator_overflow);
    return *rv_opt;
  }

  __always_inline
  void change_front(value_type val) {
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    require(succ, error_code::iterator_overflow);
    auto val_sl = schema::build(val).make_slice();
    dict_.dictuset(val_sl, idx, KeyLen);
  }

  __always_inline
  std::optional<value_type> back_opt() const {
    auto [sl, idx, succ] = dict_.dictumax(KeyLen);
    if (succ)
      return schema::parse<value_type>(sl);
    return {};
  }

  __always_inline
  std::optional<std::pair<unsigned, value_type>> back_with_idx_opt() const {
    auto [sl, idx, succ] = dict_.dictumax(KeyLen);
    if (succ)
      return std::make_pair(idx, schema::parse<value_type>(sl));
    return {};
  }

  __always_inline
  value_type back() const {
    auto rv_opt = back_opt();
    require(!!rv_opt, error_code::iterator_overflow);
    return *rv_opt;
  }

  __always_inline
  std::pair<unsigned, value_type> back_with_idx() const {
    auto rv_opt = back_with_idx_opt();
    require(!!rv_opt, error_code::iterator_overflow);
    return *rv_opt;
  }

  __always_inline
  bool empty() const { return size_ == 0; }

  __always_inline
  schema::uint32 size() const { return size_; }

  __always_inline
  void push(value_type val) {
    auto val_sl = schema::build(val).make_slice();
    auto [sl, idx, succ] = dict_.dictumax(KeyLen);
    if (!succ) {
      dict_.dictuset(val_sl, 1, KeyLen);
      ++size_;
      return;
    } else {
      if (idx != 0xFFFFFFFFFFFFFFFF) {
        dict_.dictuset(val_sl, idx + 1, KeyLen);
        ++size_;
      } else {
        reindex();
        auto [=sl, =idx, =succ] = dict_.dictumax(KeyLen);
        dict_.dictuset(val_sl, idx + 1, KeyLen);
        ++size_;
      }
    }
  }

  __always_inline
  std::optional<value_type> pop_opt() {
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    if (succ) {
      dict_.dictudel(idx, KeyLen);
      --size_;
      return schema::parse<value_type>(sl);
    }
    return {};
  }

  __always_inline
  void pop() {
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    require(succ, error_code::iterator_overflow);
    dict_.dictudel(idx, KeyLen);
    --size_;
  }

  __always_inline
  void reindex() {
    dictionary new_dict;
    unsigned new_idx = 1;
    auto [sl, idx, succ] = dict_.dictumin(KeyLen);
    while (succ) {
      new_dict.dictuset(sl, new_idx++, KeyLen);
      auto [=sl, =idx, =succ] = dict_.dictugetnext(idx, KeyLen);
    }
    dict_ = new_dict;
  }

  using const_iterator = small_dictionary_const_iterator<Element, KeyLen>;

  __always_inline
  bool erase(const_iterator it) {
    if (dict_.dictudel(it.idx_, KeyLen)) {
      --size_;
      return true;
    }
    return false;
  }

  __always_inline
  const_iterator begin() const {
    return const_iterator::create_begin(dict_);
  }
  __always_inline
  const_iterator end() const {
    return const_iterator::create_end(dict_);
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

