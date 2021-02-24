#pragma once

#include <tvm/dictionary.hpp>
#include <tvm/schema/estimate_element.hpp>

namespace tvm {

template<class Key, class Element>
class small_dict_map {
  using est_t = schema::estimate_element<std::tuple<Key, Element>>;
  static_assert(est_t::max_bits < cell::max_bits,
                "Key + Element must fit one cell");
  static_assert(est_t::max_bits == est_t::min_bits,
                "Key + Element must be fixed-size");
  static_assert(est_t::max_refs == 0,
                "Key and Element must not have references");
public:
  static constexpr unsigned KeyLen = schema::estimate_element<Key>::max_bits;

  using key_type = Key;
  using value_type = std::pair<Key, Element>;

  __always_inline
  small_dict_map() {}

  __always_inline
  small_dict_map(std::initializer_list<value_type> il) {
    size_ = 0;
    for (auto v : il)
      set_at(v.first, Element(v.second));
  }

  __always_inline
  small_dict_map& operator=(std::initializer_list<value_type> il) {
    clear();
    for (auto v : il)
      set_at(v.first, Element(v.second));
    return *this;
  }

  __always_inline
  void clear() {
    size_ = 0;
    dict_.clear();
  }

  __always_inline
  bool empty() const { return dict_.empty(); }

  __always_inline
  schema::uint32 size() const { return size_; }

  __always_inline
  bool contains(Key key) const {
    auto [sl, succ] = dict_.dictget(schema::build(key).make_slice(), KeyLen);
    return succ;
  }

  __always_inline
  void erase(Key key) {
    if (dict_.dictdel(key, KeyLen))
      --size_;
  }

  __always_inline
  bool set_at(Key key, Element val) {
    // increase size if adding new key
    auto [sl, existing] = dict_.dictsetget(
      schema::build(val).make_slice(), schema::build(key).make_slice(), KeyLen);
    if (!existing) {
      ++size_;
      return true;
    }
    return false;
  }
  __always_inline
  bool insert(value_type pair) {
    auto key = pair.first;
    auto value = pair.second;
    return set_at(key, value);
  }
  __always_inline
  Element get_at(Key key) const {
    auto [sl, succ] = dict_.dictget(schema::build(key).make_slice(), KeyLen);
    tvm_assert(succ, error_code::iterator_overflow);
    return schema::parse<Element>(sl);
  }
  __always_inline
  Element operator[](Key key) const {
    return get_at(key);
  }

  __always_inline
  std::optional<Element> lookup(Key key) const {
    auto [sl, succ] = dict_.dictget(schema::build(key).make_slice(), KeyLen);
    if (succ)
      return schema::parse<Element>(sl);
    return {}; 
  }
  __always_inline
  std::optional<Element> extract(Key key) const {
    auto [sl, succ] = dict_.dictdelget(schema::build(key).make_slice(), KeyLen);
    if (succ) {
      --size_;
      return schema::parse<Element>(sl);
    }
    return {};
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

