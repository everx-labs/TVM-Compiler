#pragma once

#include <tvm/dict_base.hpp>
#include <tvm/dictionary_map_const_iterator.hpp>
#include <tvm/ElementRef.hpp>

namespace tvm {

// Map represented in tvm dictionary
template<class Key, class Element>
class dict_map {};

template<unsigned KeyLen, class Element>
class dict_map<schema::uint_t<KeyLen>, Element> : public dict_base<Element, KeyLen> {
  using base = dict_base<Element, KeyLen>;
public:
  using key_type = schema::uint_t<KeyLen>;
  using value_type = std::pair<key_type, Element>;
  static constexpr bool small_elem = small_element<Element, KeyLen>;

  dict_map() {}
  dict_map(schema::uint32 size, dictionary dict) : base{size, dict} {}

  dict_map(std::initializer_list<value_type> il) {
    base::size_ = 0;
    for (auto v : il)
      set_at(v.first.get(), Element(v.second));
  }
  dict_map(std::initializer_list<std::pair<int, int>> il) {
    base::size_ = 0;
    for (auto v : il)
      set_at(v.first, Element(v.second));
  }

  dict_map& operator=(std::initializer_list<value_type> il) {
    base::clear();
    for (auto v : il)
      set_at(v.first.get(), Element(v.second));
    return *this;
  }
  dict_map& operator=(std::initializer_list<std::pair<int, int>> il) {
    base::clear();
    for (auto v : il)
      set_at(v.first, Element(v.second));
    return *this;
  }

  bool contains(unsigned key) const {
    auto [slice, succ] = base::dict_.dictuget(key, KeyLen);
    return succ;
  }

  void erase(unsigned key) {
    if (base::dict_.dictudel(key, KeyLen))
      --base::size_;
  }

  void set_at(unsigned idx, Element val) {
    // increase size if adding new key
    if constexpr (small_elem) {
      auto [sl, existing] = base::dict_.dictusetget(schema::build(val).make_slice(), idx, KeyLen);
      if (!existing)
        ++base::size_;
    } else {
      auto [sl, existing] = base::dict_.dictusetgetref(schema::build(val).make_cell(), idx, KeyLen);
      if (!existing)
        ++base::size_;
    }
  }
  Element get_at(unsigned idx) const {
    if constexpr (small_elem) {
      auto [slice, succ] = base::dict_.dictuget(idx, KeyLen);
      tvm_assert(succ, error_code::iterator_overflow);
      return schema::parse<Element>(slice);
    } else {
      auto [cl, succ] = base::dict_.dictugetref(idx, KeyLen);
      tvm_assert(succ, error_code::iterator_overflow);
      return schema::parse<Element>(cl.ctos());
    }
  }

  Element operator[](unsigned idx) const {
    return get_at(idx);
  }
  using Ref = ElementRef<Element, KeyLen>;
  Ref operator[](unsigned idx) {
    return Ref{base::dict_, base::size_, idx};
  }
  
  using const_iterator = dictionary_map_const_iterator<Element, KeyLen>;
  const_iterator begin() const {
    return const_iterator::create_begin(base::dict_);
  }
  const_iterator end() const {
    return const_iterator::create_end(base::dict_);
  }
};

} // namespace tvm

