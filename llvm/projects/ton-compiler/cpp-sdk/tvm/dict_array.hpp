#pragma once

#include <tvm/dict_base.hpp>
#include <tvm/ElementRef.hpp>
#include <tvm/dictionary_const_iterator.hpp>
#include <tvm/dictionary_array_iterator.hpp>

namespace tvm {

// Array represented in tvm dictionary
template<class Element, unsigned KeyLen = 32>
class dict_array : public dict_base<Element, KeyLen> {
  using base = dict_base<Element, KeyLen>;
public:
  dict_array() {}
  dict_array(schema::uint32 size, dictionary dict) : base{size, dict} {}

  dict_array(std::initializer_list<Element> il) {
    base::size_ = 0;
    for (auto v : il)
      push_back(v);
  }
  dict_array(std::initializer_list<unsigned> il) {
    base::size_ = 0;
    for (auto v : il)
      push_back(Element(v));
  }
  dict_array(std::initializer_list<int> il) {
    base::size_ = 0;
    for (auto v : il)
      push_back(Element(v));
  }
  template <class _Iterator>
  dict_array(_Iterator __first, _Iterator __last) {
    assign(__first, __last);
  }

  dict_array& operator=(std::initializer_list<Element> il) {
    base::clear();
    for (auto v : il)
      push_back(v);
    return *this;
  }
  dict_array& operator=(std::initializer_list<unsigned> il) {
    base::clear();
    for (auto v : il)
      push_back(Element(v));
    return *this;
  }
  dict_array& operator=(std::initializer_list<int> il) {
    base::clear();
    for (auto v : il)
      push_back(Element(v));
    return *this;
  }

  template <class _Iterator>
  void assign(_Iterator __first, _Iterator __last) {
    base::clear();
    for (; __first != __last; ++__first)
      push_back(Element(*__first));
  }

  void push_back(Element elem) {
    base::traits::set(base::dict_, base::size_.get(), elem);
    ++base::size_;
  }
  void pop_back() {
    auto [val, idx, succ] = base::dict_.dictureremmax(KeyLen);
    require(succ, error_code::iterator_overflow);
    --base::size_;
  }

  Element front() const {
    return *begin();
  }

  void set_at(unsigned idx, Element val) {
    require(idx < base::size_, error_code::iterator_overflow);
    base::traits::set(base::dict_, idx, val);
  }
  Element get_at(unsigned idx) const {
    auto opt_val = base::lookup(idx);
    require(!!opt_val, error_code::iterator_overflow);
    return *opt_val;
  }

  Element operator[](unsigned idx) const {
    return get_at(idx);
  }
  using Ref = ElementRef<Element, KeyLen>;
  Ref operator[](unsigned idx) {
    return Ref{base::dict_, idx};
  }

  using const_iterator = dictionary_const_iterator<Element, KeyLen>;
  const_iterator begin() const {
    return const_iterator::create_begin(base::dict_);
  }
  const_iterator end() const {
    return const_iterator::create_end(base::dict_);
  }

  using iterator = dictionary_array_iterator<Element, KeyLen>;
  iterator begin() {
    return iterator::create_begin(base::dict_, base::size_);
  }
  iterator end() {
    return iterator::create_end(base::dict_, base::size_);
  }
};

} // namespace tvm

