#pragma once

#include <tvm/chain_builder.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
struct big_dict_traits {
  __always_inline
  static Element parse(parser p) {
    return parse_chain<Element>(p);
  }
  __always_inline
  static std::optional<Element> lookup(const dictionary& dict, unsigned key) {
    auto [cell, succ] = dict.dictugetref(key, KeyLen);
    if (succ)
      return parse_chain<Element,0,0>(parser(cell));
    return {};
  }
  __always_inline
  static void set(dictionary& dict, unsigned idx, Element val) {
    dict.dictusetref(build_chain(to_std_tuple(val)).make_cell(), idx, KeyLen);
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> min(const dictionary& dict) {
    auto [cell, idx, succ] = dict.dictuminref(KeyLen);
    if (succ)
      return {idx, parse_chain<Element,0,0>(parser(cell)), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> next(const dictionary& dict, unsigned idx) {
    auto [sl, next_idx, succ] = dict.dictugetnext(idx, KeyLen);
    if (succ)
      return {next_idx, parse_chain<Element,0,0>(parser(parser(sl).ldref())), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> max(const dictionary& dict) {
    auto [cell, idx, succ] = dict.dictumaxref(KeyLen);
    if (succ)
      return {idx, parse_chain<Element,0,0>(parser(cell)), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> prev(const dictionary& dict, unsigned idx) {
    auto [sl, prev_idx, succ] = dict.dictugetprev(idx, KeyLen);
    if (succ)
      return {prev_idx, parse_chain<Element,0,0>(parser(parser(sl).ldref())), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> rem_min(dictionary& dict, schema::uint32& size) {
    auto [cell, idx, succ] = dict.dicturemminref(KeyLen);
    if (succ) {
      size--;
      return {idx, parse_chain<Element,0,0>(parser(cell)), succ};
    }
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> rem_max(dictionary& dict, schema::uint32& size) {
    auto [cell, idx, succ] = dict.dicturemmaxref(KeyLen);
    if (succ) {
      size--;
      return {idx, parse_chain<Element,0,0>(parser(cell)), succ};
    }
    return {0, {}, succ};
  }
};

} // namespace tvm

