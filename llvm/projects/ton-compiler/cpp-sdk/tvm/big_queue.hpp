#pragma once

#include <optional>
#include <tvm/schema/make_parser.hpp>

namespace tvm {

// queue with big elements (>1023 bits for key + value + 12)
template<class Element>
class big_queue {
  static constexpr unsigned KeyLen = 64;
public:
  using value_type = Element;

  __always_inline
  std::optional<value_type> front_opt() const {
    auto [cl, idx, succ] = dict_.dictuminref(KeyLen);
    if (succ)
      return parse_val(cl);
    return {};
  }

  __always_inline
  std::optional<std::pair<unsigned, value_type>> front_with_idx_opt() const {
    auto [cl, idx, succ] = dict_.dictuminref(KeyLen);
    if (succ)
      return std::make_pair(idx, parse_val(cl));
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
    auto [cl, idx, succ] = dict_.dictuminref(KeyLen);
    require(succ, error_code::iterator_overflow);
    dict_.dictusetref(build_val(val), idx, KeyLen);
  }

  __always_inline
  std::optional<value_type> back_opt() const {
    auto [cl, idx, succ] = dict_.dictumaxref(KeyLen);
    if (succ)
      return parse_val(cl);
    return {};
  }

  __always_inline
  std::optional<std::pair<unsigned, value_type>> back_with_idx_opt() const {
    auto [cl, idx, succ] = dict_.dictumaxref(KeyLen);
    if (succ)
      return std::make_pair(idx, parse_val(cl));
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
    cell val_cl = build_val(val);
    auto [sl, idx, succ] = dict_.dictumax(KeyLen);
    if (!succ) {
      dict_.dictusetref(val_cl, 1, KeyLen);
      ++size_;
      return;
    } else {
      if (idx != 0xFFFFFFFFFFFFFFFF) {
        dict_.dictusetref(val_cl, idx + 1, KeyLen);
        ++size_;
      } else {
        reindex();
        auto [=sl, =idx, =succ] = dict_.dictumax(KeyLen);
        dict_.dictusetref(val_cl, idx + 1, KeyLen);
        ++size_;
      }
    }
  }

  __always_inline
  std::optional<value_type> pop_opt() {
    auto [cl, idx, succ] = dict_.dicturemminref(KeyLen);
    if (succ) {
      --size_;
      return parse_val(cl);
    }
    return {};
  }

  __always_inline
  void pop() {
    auto [cl, idx, succ] = dict_.dicturemminref(KeyLen);
    require(succ, error_code::iterator_overflow);
    --size_;
  }

  __always_inline
  void reindex() {
#ifndef TVM_NO_QUEUE_REINDEX
    dictionary new_dict;
    unsigned new_idx = 1;
    bool succ = true;
    do {
      auto [cl, idx, =succ] = dict_.dicturemminref(KeyLen);
      if (succ) {
        new_dict.dictusetref(cl, new_idx++, KeyLen);
      }
    } while(succ);
    dict_ = new_dict;
#else
    tvm_throw(error_code::iterator_overflow);
#endif
  }

  using const_iterator = big_dictionary_const_iterator<Element, KeyLen>;

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

  __always_inline
  cell build_val(value_type val) const {
    using namespace schema;
    auto data_tup = to_std_tuple(val);
    auto chain_tup = make_chain_tuple(data_tup);
    return build(chain_tup).make_cell();
  }

  __always_inline
  value_type parse_val(cell cl) const {
    using namespace schema;
    using data_tup_t = to_std_tuple_t<value_type>;
    using LinearTup = decltype(make_chain_tuple(data_tup_t{}));
    // __reflect_echo<print_chain_tuple<LinearTup>().c_str()>{};
    auto linear_tup = parse<LinearTup>(parser(cl));
    return to_struct<value_type>(chain_fold_tup<data_tup_t>(linear_tup));
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

