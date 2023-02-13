#pragma once

#include <tvm/schema/estimate_element.hpp>
#include <tvm/builder.hpp>
#include <tvm/parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/make_parser.hpp>

#include <tvm/to_struct.hpp>
#include <tvm/error_code.hpp>
#include <tvm/assert.hpp>
#include <tvm/sequence.hpp>

#include <boost/hana/take_front.hpp>
#include <boost/hana/take_back.hpp>
#include <boost/hana/ext/std/tuple.hpp>

#include <experimental/type_traits>

#include <tvm/schema/chain_builder.hpp>
#include <tvm/schema/chain_tuple_printer.hpp>

namespace tvm { inline namespace schema {

namespace hana = boost::hana;

template<class T>
struct expander {
  static auto execute(T elem) {
    return to_std_tuple(elem);
  }
};
template<class... Types>
struct expander<std::tuple<Types...>> {
  static auto execute(std::tuple<Types...> elem) {
    return elem;
  }
};
template<class T>
struct expander<tvm::tuple<T>> {
  static auto execute(tvm::tuple<T> elem) {
    return expander<T>::execute(elem.unpack());
  }
};
template<class T>
struct entupler {
  static auto execute(T elem) {
    return std::make_tuple(elem);
  }
};

template<class T>
constexpr auto expand_elem(T elem) {
  using expander_t = std::conditional_t<schema::is_expandable<T>::value, expander<T>, entupler<T>>;
  return expander_t::execute(elem);
}

template<class... Types>
constexpr bool has_expandable() {
  return (false || ... || schema::is_expandable<Types>::value);
}
template<class T>
struct tuple_has_expandable_elem {};
template<class... Types>
struct tuple_has_expandable_elem<std::tuple<Types...>> {
  static constexpr bool value = has_expandable<Types...>();
};

template<class... Elems>
auto make_expanded_tuple(std::tuple<Elems...> tup) {
  if constexpr (sizeof...(Elems) != 0) {
    auto first_elem = std::get<0>(tup);
    auto elem_tup = expand_elem(first_elem);
    if constexpr (sizeof...(Elems) > 1) {
      auto next_subtup = hana::take_back_c<sizeof...(Elems) - 1>(tup);
      if constexpr (!tuple_has_expandable_elem<decltype(elem_tup)>::value) {
        return std::tuple_cat(elem_tup, make_expanded_tuple(next_subtup));
      } else {
        return std::tuple_cat(make_expanded_tuple(elem_tup), make_expanded_tuple(next_subtup));
      }
    } else {
      if constexpr (!tuple_has_expandable_elem<decltype(elem_tup)>::value) {
        return elem_tup;
      } else {
        return make_expanded_tuple(elem_tup);
      }
    }
  } else {
    return tup;
  }
}

// Selects subsequence of elements wich is statically fits into cell,
//  replaces continuation as ref<> to rest elements (and nested refs)
// { a, b, c, d, e, f, g, h } => { a, b, ref{c, d, e, ref{f, g, h}} }
template<unsigned Offset = 0, unsigned RefsOffset = 0, class... Elems>
__always_inline
auto make_chain_tuple_impl(std::tuple<Elems...> tup) {
  // Fit count (of elems) with usage of the last `reserved` cell for reference elems
  static constexpr unsigned last_fit_count =
    extract_fit<cell::max_bits - Offset, cell::max_refs - RefsOffset, 0, Elems...>::value;
  // Fit count (of elems) without usage of the last `reserved` cell for reference elems
  static constexpr unsigned fit_count =
    extract_fit<cell::max_bits - Offset, cell::max_refs - 1 - RefsOffset, 0, Elems...>::value;
  // If all the remaining elems fit into cell (including `reserved` last reference)
  if constexpr (sizeof...(Elems) == last_fit_count) {
    return hana::take_front_c<last_fit_count>(tup);
  } else {
    auto cur_subtup = hana::take_front_c<fit_count>(tup);

    if constexpr (fit_count == 0 && sizeof...(Elems) != 0) {
      if constexpr (Offset || RefsOffset) {
        auto cont_tup = make_chain_tuple_impl(tup);
        return std::make_tuple(schema::ref<decltype(cont_tup)>{cont_tup});
      } else {
        static_assert(Offset || RefsOffset, "Can't place even 1 sequence element into cell");
        return std::tuple<>();
      }
    } else {
      if constexpr (sizeof...(Elems) > fit_count) {
        auto next_subtup = hana::take_back_c<sizeof...(Elems) - fit_count>(tup);
        auto cont_tup = make_chain_tuple_impl(next_subtup);
        return std::tuple_cat(cur_subtup, std::make_tuple(schema::ref<decltype(cont_tup)>{cont_tup}));
      } else {
        return cur_subtup;
      }
    }
  }
}

// Make tuple of expanded atomic elements, splitted into cells with
// carry-over by refs {u256, u256, u256, u256, point3d<u32>{}} => {u256, u256, u256, ref<u256, u32, u32, u32>}
template<unsigned Offset = 0, unsigned RefsOffset = 0, class... Elems>
__always_inline
auto make_chain_tuple(std::tuple<Elems...> tup_in) {
  auto tup = make_expanded_tuple(tup_in);
  if constexpr (std::tuple_size_v<decltype(tup)> == 0)
    return tup;
  else
    return make_chain_tuple_impl<Offset, RefsOffset>(tup);
}

}} // namespace tvm::schema

