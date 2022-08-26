#pragma once

#include <tvm/ElementRef.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
struct dictionary_array_iterator : boost::operators<dictionary_array_iterator<Element, KeyLen>> {
  using iterator_category = std::random_access_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = Element*;
  using reference = ElementRef<Element, KeyLen>;
  using traits = dict_traits<Element, KeyLen>;

  dictionary& dict_;
  unsigned idx_;
  schema::uint32 size_;

  __always_inline Element operator*() const {
    auto opt_elem = traits::lookup(dict_, idx_);
    require(opt_elem, error_code::iterator_overflow);
    return *opt_elem;
  }
  __always_inline reference operator*() {
    return reference{dict_, size_, idx_};
  }

  __always_inline bool is_end() const { return idx_ >= size_.get(); }

  static dictionary_array_iterator create_begin(dictionary& dict, schema::uint32 size) {
    return dictionary_array_iterator{{}, dict, 0, size};
  }
  static dictionary_array_iterator create_end(dictionary& dict, schema::uint32 size) {
    return dictionary_array_iterator{{}, dict, size.get(), size};
  }

  bool operator<(dictionary_array_iterator x) const { return idx_ < x.idx_; }
  
  dictionary_array_iterator& operator+=(int v) {
    idx_ += v;
    return *this;
  }
  dictionary_array_iterator& operator-=(int v) {
    idx_ -= v;
    return *this;
  }
  dictionary_array_iterator& operator++() {
    ++idx_;
    return *this;
  }
  dictionary_array_iterator& operator--() {
    --idx_;
    return *this;
  }
  bool operator==(dictionary_array_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};

} // namespace tvm

