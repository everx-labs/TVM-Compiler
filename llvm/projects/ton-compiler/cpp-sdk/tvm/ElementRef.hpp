#pragma once

#include <tvm/dict_traits.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
struct SmallElementRef {
  __always_inline SmallElementRef& operator=(Element elem) {
    auto [sl, succ] = dict_.dictusetget(schema::build(elem).make_slice(), idx_, KeyLen);
    if (!succ)
      ++size_;
    return *this;
  }
  __always_inline operator Element() const {
    auto [sl, succ] = dict_.dictuget(idx_, KeyLen);
    require(succ, error_code::iterator_overflow);
    return schema::parse<Element>(sl);
  }

  __always_inline void swap(SmallElementRef v) const {
    auto [sl1, succ1] = dict_.dictuget(idx_, KeyLen);
    require(succ1, error_code::iterator_overflow);
    auto [sl2, succ2] = v.dict_.dictuget(v.idx_, KeyLen);
    require(succ2, error_code::iterator_overflow);
    dict_.dictuset(sl2, idx_, KeyLen);
    v.dict_.dictuset(sl1, v.idx_, KeyLen);
  }
  dictionary& dict_;
  schema::uint32& size_;
  unsigned idx_;
};

template<class Element, unsigned KeyLen>
__always_inline
void swap(SmallElementRef<Element, KeyLen> ref1, SmallElementRef<Element, KeyLen> ref2) {
  auto v1 = static_cast<Element>(ref1);
  auto v2 = static_cast<Element>(ref2);
  ref1 = v2;
  ref2 = v1;
}

template<class Element, unsigned KeyLen>
struct BigElementRef {
  __always_inline BigElementRef& operator=(Element elem) {
    auto [cl, succ] = dict_.dictusetgetref(build_chain(to_std_tuple(elem)).make_cell(), idx_, KeyLen);
    if (!succ)
      ++size_;
    return *this;
  }
  __always_inline operator Element() const {
    auto [cl, succ] = dict_.dictugetref(idx_, KeyLen);
    require(succ, error_code::iterator_overflow);
    return parse_chain<Element,0,0>(parser(cl));
  }

  __always_inline void swap(BigElementRef v) const {
    auto [cl1, succ1] = dict_.dictugetref(idx_, KeyLen);
    require(succ1, error_code::iterator_overflow);
    auto [cl2, succ2] = v.dict_.dictugetref(v.idx_, KeyLen);
    require(succ2, error_code::iterator_overflow);
    dict_.dictusetref(cl2, idx_, KeyLen);
    v.dict_.dictusetref(cl1, v.idx_, KeyLen);
  }
  dictionary& dict_;
  schema::uint32& size_;
  unsigned idx_;
};

template<class Element, unsigned KeyLen>
__always_inline
void swap(BigElementRef<Element, KeyLen> ref1, BigElementRef<Element, KeyLen> ref2) {
  auto v1 = static_cast<Element>(ref1);
  auto v2 = static_cast<Element>(ref2);
  ref1 = v2;
  ref2 = v1;
}

template<class Element, unsigned KeyLen>
using ElementRef =
  std::conditional_t<small_element<Element, KeyLen>,
                     SmallElementRef<Element, KeyLen>,
                     BigElementRef<Element, KeyLen>>;

} // namespace tvm

