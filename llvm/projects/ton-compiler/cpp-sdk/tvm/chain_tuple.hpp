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

#include <tvm/chain_builder.hpp>

namespace tvm {

namespace hana = boost::hana;

// Selects subsequence of elements wich is statically fits into cell,
//  replaces continuation as ref<> to rest elements (and nested refs)
// { a, b, c, d, e, f, g, h } => { a, b, ref{c, d, e, ref{f, g, h}} }
template<class... Elems>
auto make_chain_tuple(std::tuple<Elems...> tup) {
  static constexpr unsigned fit_count = extract_fit<cell::max_bits, cell::max_refs - 1, 0, Elems...>::value;
  auto cur_subtup = hana::take_front_c<fit_count>(tup);

  if constexpr (fit_count == 0 && sizeof...(Elems) != 0) {
    // expanding first element
    auto first_elem = std::get<0>(tup);
    using first_elem_t = decltype(first_elem);
    static_assert(is_expandable<first_elem_t>::value,
                  "Can't place even 1 sequence element into cell");
    auto split_tup = make_chain_tuple(to_std_tuple(first_elem));
    if constexpr (sizeof...(Elems) > 1) {
      auto next_subtup = hana::take_back_c<sizeof...(Elems) - 1>(tup);
      return std::tuple_cat(split_tup, make_chain_tuple(next_subtup));
    } else {
      return split_tup;
    }
  } else {
    if constexpr (sizeof...(Elems) > fit_count) {
      auto next_subtup = hana::take_back_c<sizeof...(Elems) - fit_count>(tup);
      auto cont_tup = make_chain_tuple(next_subtup);
      return std::tuple_cat(cur_subtup, std::make_tuple(schema::ref<decltype(cont_tup)>{cont_tup}));
    } else {
      return cur_subtup;
    }
  }
}

} // namespace tvm

