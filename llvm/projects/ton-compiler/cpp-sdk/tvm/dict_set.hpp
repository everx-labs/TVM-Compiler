#pragma once

#include <tvm/dict_base.hpp>

namespace tvm {

// Set represented in tvm dictionary
template<class Key>
class dict_set {};

template<unsigned KeyLen>
struct dict_set_const_iterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = schema::uint_t<KeyLen>;
  using difference_type = int;
  using pointer = value_type*;
  using reference = value_type&;

  dictionary dict_;
  unsigned idx_;
  bool is_end_;

  __always_inline value_type operator*() const {
    return value_type(idx_);
  }

  __always_inline bool is_end() const { return is_end_; }

  static dict_set_const_iterator create_begin(dictionary dict) {
    auto [sl, idx, succ] = dict.dictumin(KeyLen);
    return dict_set_const_iterator{dict, idx, !succ};
  }
  static dict_set_const_iterator create_end() {
    return dict_set_const_iterator{{}, 0, true};
  }

  __always_inline dict_set_const_iterator operator++() {
    require(!is_end_, error_code::iterator_overflow);
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      idx_ = next_idx;
      is_end_ = false;
    } else {
      is_end_ = true;
    }
    return *this;
  }
  __always_inline
  bool operator==(dict_set_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return left_end && right_end;
  }
  __always_inline
  bool operator!=(dict_set_const_iterator v) const {
    return !(*this == v);
  }
};

template<unsigned KeyLen>
class dict_set<schema::uint_t<KeyLen>> {
public:
  using key_type = schema::uint_t<KeyLen>;

  __always_inline
  dict_set() {}
  __always_inline
  dict_set(schema::uint32 size, dictionary dict) : size_{size}, dict_{dict} {}

  __always_inline
  dict_set(std::initializer_list<key_type> il) {
    size_ = 0;
    for (auto v : il)
      insert(v);
  }
  __always_inline
  dict_set(std::initializer_list<unsigned> il) {
    size_ = 0;
    for (auto v : il)
      insert(key_type(v));
  }

  __always_inline
  dict_set& operator=(std::initializer_list<key_type> il) {
    clear();
    for (auto v : il)
      insert(v);
    return *this;
  }
  __always_inline
  dict_set& operator=(std::initializer_list<unsigned> il) {
    clear();
    for (auto v : il)
      insert(key_type(v));
    return *this;
  }
  template<class It>
  __always_inline
  dict_set(It begin, It end) {
    size_ = 0;
    for (auto it = begin; it != end; ++it)
      insert(*it);
  }

  __always_inline
  bool contains(key_type key) const {
    auto [slice, succ] = dict_.dictuget(key.get(), KeyLen);
    return succ;
  }

  __always_inline
  void erase(key_type key) {
    if (dict_.dictudel(key.get(), KeyLen))
      --size_;
  }

  __always_inline
  void insert(key_type key) {
    // increase size if adding new key
    auto [sl, existing] = dict_.dictusetget(slice::create_empty(), key.get(), KeyLen);
    if (!existing)
      ++size_;
  }

  __always_inline
  void clear() {
    size_ = 0;
    dict_.clear();
  }
  __always_inline
  bool empty() const { return size_ == 0; }
  __always_inline
  schema::uint32 size() const { return size_; }

  __always_inline
  dict_set_const_iterator<KeyLen> begin() const {
    return dict_set_const_iterator<KeyLen>::create_begin(dict_);
  }
  __always_inline
  dict_set_const_iterator<KeyLen> end() const {
    return dict_set_const_iterator<KeyLen>::create_end();
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

