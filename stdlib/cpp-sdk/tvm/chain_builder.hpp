#pragma once

#include <tvm/schema/estimate_element.hpp>

#include <boost/hana/take_front.hpp>
#include <boost/hana/take_back.hpp>
#include <boost/hana/ext/std/tuple.hpp>

namespace tvm {

// extract tuple format elements, that will for sure fit into Bits and Refs (when serialized)
template<unsigned Bits, unsigned Refs, unsigned Idx, class Elem, class... Elems>
struct extract_fit {
  using est_t = schema::estimate_element<Elem>;
  static constexpr bool fit = est_t::max_bits <= Bits && est_t::max_refs <= Refs;

  static constexpr unsigned value = std::conditional_t<fit,
      extract_fit<Bits - est_t::max_bits, Refs - est_t::max_refs, Idx + 1, Elems...>,
      std::integral_constant<unsigned, Idx>>::value;
};

template<unsigned Bits, unsigned Refs, unsigned Idx, class Elem>
struct extract_fit<Bits, Refs, Idx, Elem> {
  using est_t = schema::estimate_element<Elem>;
  static constexpr bool fit = est_t::max_bits <= Bits && est_t::max_refs <= Refs;
  static constexpr unsigned value = std::conditional_t<fit,
      std::integral_constant<unsigned, Idx + 1>,
      std::integral_constant<unsigned, Idx>>::value;
};

template<class... Elems>
static __always_inline cell build_chain(std::tuple<Elems...> tup) {
  static constexpr unsigned fit_count = extract_fit<cell::max_bits, cell::max_refs - 1, 0, Elems...>::value;
  static_assert(fit_count > 0, "Can't place even 1 sequence element into cell");
    
  auto cur_subtup = hana::take_front_c<fit_count>(tup);
  builder b = schema::build(cur_subtup);
  if constexpr (sizeof...(Elems) > fit_count) {
    auto next_subtup = hana::take_back_c<sizeof...(Elems) - fit_count>(tup);
    b.stref(build_chain(next_subtup));
  }
  return b.make_cell();
}

template<class Tup, unsigned Offset>
struct chain_parser_impl {};
template<class... Elems, unsigned Offset>
struct chain_parser_impl<std::tuple<Elems...>, Offset> {
  using Tup = std::tuple<Elems...>;
  static __always_inline Tup parse(parser p) {
    static constexpr unsigned fit_count =
      extract_fit<cell::max_bits - Offset, cell::max_refs - 1, 0, Elems...>::value;
    static_assert(fit_count > 0, "Can't place even 1 sequence element into cell");
    using FrontT = decltype(hana::take_front_c<fit_count>(Tup{}));
    auto [front_tup, =p] = schema::parse_continue<FrontT>(p);
    require(!!front_tup, error_code::custom_data_parse_error);
    if constexpr (sizeof...(Elems) > fit_count) {
      using NextT = decltype(hana::take_back_c<sizeof...(Elems) - fit_count>(Tup{}));
      parser next_p(p.ldrefrtos());
      auto next_tup = chain_parser_impl<NextT, 0>::parse(next_p);
      return std::tuple_cat(*front_tup, next_tup);
    } else {
      return *front_tup;
    }
  }
};

template<class Data, unsigned Offset>
static __always_inline Data parse_chain(parser p) {
  using Tup = to_std_tuple_t<Data>;
  auto rv_tup = chain_parser_impl<Tup, Offset>::parse(p);
  return to_struct<Data>(rv_tup);
}

} // namespace tvm

