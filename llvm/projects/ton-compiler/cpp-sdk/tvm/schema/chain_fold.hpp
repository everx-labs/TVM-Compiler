#pragma once

#include <tvm/schema/is_expandable.hpp>

namespace tvm { inline namespace schema {

template<class T>
struct tuple_pop_front_type {};
template<class Elem, class... Elems>
struct tuple_pop_front_type<std::tuple<Elem, Elems...>> {
  using type = std::tuple<Elems...>;
};
template<class T>
using tuple_pop_front_type_t = typename tuple_pop_front_type<T>::type;

template<class T>
struct is_ref : std::false_type {};
template<class T>
struct is_ref<ref<T>> : std::true_type {};
template<class T>
const bool is_ref_v = is_ref<T>::value;

template<class RT, class LinearTup>
__always_inline
auto chain_fold_impl(LinearTup linear_tup);

template<class Elem, class FoldedTup, class LinearTup>
__always_inline
auto chain_fold_elem(FoldedTup folded_tup, LinearTup linear_tup) {
  if constexpr (is_ref_v<std::tuple_element_t<0, LinearTup>> && !is_ref_v<Elem>) {
    static_assert(std::tuple_size_v<LinearTup> == 1,
      "chain_fold_elem: Non-single value at cell wrap");
    auto ref_val = std::get<0>(linear_tup);
    auto next_tuple = ref_val();
    return chain_fold_elem<Elem>(folded_tup, next_tuple);
  } else if constexpr (is_expandable_v<Elem>) {
    auto [new_val, new_linear_tup] = chain_fold_impl<Elem, LinearTup>(linear_tup);
    auto new_folded_tup = std::tuple_cat(folded_tup, std::make_tuple(new_val));
    return std::make_pair(new_folded_tup, new_linear_tup);
  } else {
    auto new_val = std::get<0>(linear_tup);
    auto new_folded_tup = std::tuple_cat(folded_tup, std::make_tuple(new_val));
    auto new_linear_tup = hana::take_back_c<std::tuple_size_v<LinearTup> - 1>(linear_tup);
    return std::make_pair(new_folded_tup, new_linear_tup);
  }
}

template<class ExpandedTup, class FoldedTup, class LinearTup>
__always_inline
auto chain_fold_cont(FoldedTup folded_tup, LinearTup linear_tup) {
  constexpr auto ExpandedSize = std::tuple_size_v<ExpandedTup>;
  if constexpr (ExpandedSize == 1) {
    using Elem = std::tuple_element_t<0, ExpandedTup>;
    auto [new_folded_tup, new_linear_tup] = chain_fold_elem<Elem>(folded_tup, linear_tup);
    return std::make_pair(new_folded_tup, new_linear_tup);
  } else if constexpr (ExpandedSize > 1) {
    using Elem = std::tuple_element_t<0, ExpandedTup>;
    auto [new_folded_tup, new_linear_tup] = chain_fold_elem<Elem>(folded_tup, linear_tup);

    using RestExpandedTup = tuple_pop_front_type_t<ExpandedTup>;
    auto [new_folded_tup2, new_linear_tup2] = chain_fold_cont<RestExpandedTup>(new_folded_tup, new_linear_tup);

    return std::make_pair(new_folded_tup2, new_linear_tup2);
  } else {
    return std::make_pair(folded_tup, linear_tup);
  }
}

template <typename> struct is_tuple: std::false_type {};
template <typename ...T> struct is_tuple<std::tuple<T...>>: std::true_type {};
template <typename> struct is_tvm_tuple: std::false_type {};
template <typename T> struct is_tvm_tuple<tvm::tuple<T>>: std::true_type {};
template <typename> struct get_tvm_tuple_struct {};
template <typename T> struct get_tvm_tuple_struct<tvm::tuple<T>> { using type = T; };

// Fold from expanded linear tuple into return type (may be tuple, structure)
template<class RT, class LinearTup>
__always_inline
auto chain_fold_impl(LinearTup linear_tup) {
  if constexpr (is_tuple<RT>::value) {
    auto [folded_tup, new_linear_tup] = chain_fold_cont<RT>(std::tuple<>{}, linear_tup);
    return std::make_pair(folded_tup, new_linear_tup);
  } else if constexpr (is_tvm_tuple<RT>::value) {
    using SubT = typename get_tvm_tuple_struct<RT>::type;
    using ExpandedTup = to_std_tuple_t<SubT>;
    auto [folded_tup, new_linear_tup] = chain_fold_cont<ExpandedTup>(std::tuple<>{}, linear_tup);
    return std::make_pair(RT(to_struct<SubT>(folded_tup)), new_linear_tup);
  } else {
    using ExpandedTup = to_std_tuple_t<RT>;
    auto [folded_tup, new_linear_tup] = chain_fold_cont<ExpandedTup>(std::tuple<>{}, linear_tup);
    return std::make_pair(to_struct<RT>(folded_tup), new_linear_tup);
  }
}

// Fold from expanded linear tuple into structure
// (cell, address, uint128, ref<uint128, uint128>) => DContract{cell, address, point3d{uint128, uint128, uint128}}
template<class RT, class LinearTup>
__always_inline
auto chain_fold(LinearTup linear_tup) {
  auto [rv, new_linear_tup] = chain_fold_impl<RT>(linear_tup);
  static_assert(std::tuple_size_v<decltype(new_linear_tup)> == 0);
  return rv;
}

// Fold from expanded linear tuple into structured tuple
// (cell, address, uint128, ref<uint128, uint128>) => (cell, address, point3d{uint128, uint128, uint128})
template<class Tup, class LinearTup>
__always_inline
auto chain_fold_tup(LinearTup linear_tup) {
  auto [folded_tup, new_linear_tup] = chain_fold_cont<Tup>(std::tuple<>{}, linear_tup);
  static_assert(std::tuple_size_v<decltype(new_linear_tup)> == 0);
  return folded_tup;
}

}} // namespace tvm::schema

