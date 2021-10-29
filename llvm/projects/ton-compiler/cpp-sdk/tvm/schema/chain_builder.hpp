#pragma once

#include <tvm/schema/estimate_element.hpp>
#include <tvm/builder.hpp>
#include <tvm/parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/is_expandable.hpp>

#include <tvm/to_struct.hpp>
#include <tvm/error_code.hpp>
#include <tvm/assert.hpp>
#include <tvm/sequence.hpp>

#include <boost/hana/take_front.hpp>
#include <boost/hana/take_back.hpp>
#include <boost/hana/ext/std/tuple.hpp>

namespace tvm { inline namespace schema {

namespace hana = boost::hana;

// extract tuple format elements, that will for sure fit into Bits and Refs (when serialized)
template<unsigned Bits, unsigned Refs, unsigned Idx, class Elem, class... Elems>
struct extract_fit {
  using est_t = estimate_element<Elem>;
  static constexpr bool fit = est_t::max_bits <= Bits && est_t::max_refs <= Refs;

  static constexpr unsigned value = std::conditional_t<fit,
      extract_fit<Bits - est_t::max_bits, Refs - est_t::max_refs, Idx + 1, Elems...>,
      std::integral_constant<unsigned, Idx>>::value;
};

template<unsigned Bits, unsigned Refs, unsigned Idx, class Elem>
struct extract_fit<Bits, Refs, Idx, Elem> {
  using est_t = estimate_element<Elem>;
  static constexpr bool fit = est_t::max_bits <= Bits && est_t::max_refs <= Refs;
  static constexpr unsigned value = std::conditional_t<fit,
      std::integral_constant<unsigned, Idx + 1>,
      std::integral_constant<unsigned, Idx>>::value;
};

template<class T>
struct is_optional : std::false_type {};
template<class T>
struct is_optional<std::optional<T>> : std::true_type {};

// If element with such estimation Est is for sure parsable ({bits, refs} >= estimation max)
template<class Est>
__always_inline bool check_sure_parsable(unsigned bits, unsigned refs) {
  // If estimated element is more than cell size
  if constexpr (Est::max_bits > cell::max_bits)
    return false;
  if constexpr (Est::max_refs >= cell::max_refs)
    return false;
  if (bits < Est::max_bits)
    return false;
  if constexpr (Est::max_refs)
    if (refs <= Est::max_refs) // '<=' because we need to reserve additional ref for chaining
      return false;
  return true;
}

// If element with such estimation Est is for sure not parsable ({bits, refs} < estimation min)
template<bool IsLastElem, class Est>
__always_inline bool check_sure_unparsable(unsigned bits, unsigned refs) {
  if (bits < Est::min_bits)
    return true;
  if constexpr (Est::min_refs) {
    if (refs < Est::min_refs)
      return true;
    if constexpr (!IsLastElem)
      if (refs == Est::min_refs && !bits) // '<=' because we need to reserve additional ref for chaining
        return true;
  }
  return false;
}

template<bool IsTop, class Tup>
struct dyn_chain_parser_impl {};
template<bool IsTop, class... Elems>
struct dyn_chain_parser_impl<IsTop, std::tuple<Elems...>> {
  using Tup = std::tuple<Elems...>;
  static __always_inline std::pair<Tup, parser> parse(parser p, unsigned bits, unsigned refs) {
    if constexpr (sizeof...(Elems) == 0) {
      return { Tup{}, p };
    } else {
      // All elements may be parsed without additional checks
      using est_t = estimate_element<Tup>;
      if (check_sure_parsable<est_t>(bits, refs)) {
        auto [full_tup, =p] = parse_continue<Tup>(p);
        return { *full_tup, p };
      } else {
        using PrevT = decltype(hana::take_front_c<sizeof...(Elems) - 1>(Tup{}));
        auto [prev_tup, =p] = dyn_chain_parser_impl<false, PrevT>::parse(p, bits, refs);
        using last_elem_t = typename std::tuple_element<sizeof...(Elems) - 1, Tup>::type;
        using est_last_t = estimate_element<last_elem_t>;
        auto [last_bits, last_refs] = p.sl().sbitrefs();
        if (check_sure_unparsable<IsTop, est_last_t>(last_bits, last_refs)) {
          // TODO: investigate problem with RootTokenContractNF constructor without this `if`
          if (last_refs != 1)
            tvm_throw(error_code::non_single_refs_at_cell_wrap);
          require(last_bits == 0, error_code::non_empty_bits_at_cell_wrap);
          require(last_refs == 1, error_code::non_single_refs_at_cell_wrap);
          p = parser(p.ldrefrtos());
        }
        auto [last_elem, =p] = parse_continue<last_elem_t>(p);
        return { std::tuple_cat(prev_tup, std::make_tuple(*last_elem)), p };
      }
    }
  }
};

template<class Data>
static Data parse_chain_dynamic(parser p) {
  using Tup = to_std_tuple_t<Data>;
  if constexpr (std::tuple_size<Tup>::value == 0) {
    return Data{};
  } else {
    auto [bits, refs] = p.sl().sbitrefs();
    auto [rv_tup, =p] = dyn_chain_parser_impl<true, Tup>::parse(p, bits, refs);
    p.ends();
    return to_struct<Data>(rv_tup);
  }
}

}} // namespace tvm::schema

