#pragma once

#include <type_traits>

#include <tvm/cell.hpp>

#include <tvm/small_dict_traits.hpp>
#include <tvm/big_dict_traits.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
struct small_element_impl {
  using est_t = schema::estimate_element<Element>;
  //static constexpr bool value =
  //  (est_t::max_bits + KeyLen < cell::max_bits) && (est_t::max_refs + 1 < cell::max_refs);
  static constexpr bool value =
    (est_t::max_bits + KeyLen < cell::max_bits) && (est_t::max_refs == 0);
};

template<class Element, unsigned KeyLen>
static constexpr bool small_element = small_element_impl<Element, KeyLen>::value;

// selector between big-element (>cell) dictionary and small element dictionary
template<class Element, unsigned KeyLen>
using dict_traits =
  std::conditional_t<small_element<Element, KeyLen>,
                     small_dict_traits<Element, KeyLen>,
                     big_dict_traits<Element, KeyLen>>;

} // namespace tvm

