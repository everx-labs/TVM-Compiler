#pragma once

namespace tvm {

template<class Element, unsigned KeyLen>
struct small_dict_traits {
  __always_inline
  static Element parse(parser p) {
    return schema::parse<Element>(p);
  }

  __always_inline
  static std::optional<Element> lookup(const dictionary& dict, unsigned key) {
    auto [slice, succ] = dict.dictuget(key, KeyLen);
    if (succ)
      return schema::parse<Element>(slice);
    return {};
  }
  __always_inline
  static void set(dictionary& dict, unsigned idx, Element val) {
    dict.dictuset(schema::build(val).make_slice(), idx, KeyLen);
  }

  // Returns {Key, Element, Success} to match the solidity array.min() function
  //  (not {Element, Key, Success} as dictumin returns)
  __always_inline
  static std::tuple<unsigned, Element, bool> min(const dictionary& dict) {
    auto [val, idx, succ] = dict.dictumin(KeyLen);
    if (succ)
      return {idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> next(const dictionary& dict, unsigned idx) {
    auto [val, next_idx, succ] = dict.dictugetnext(idx, KeyLen);
    if (succ)
      return {next_idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> max(const dictionary& dict) {
    auto [val, idx, succ] = dict.dictumax(KeyLen);
    if (succ)
      return {idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> prev(const dictionary& dict, unsigned idx) {
    auto [val, prev_idx, succ] = dict.dictugetprev(idx, KeyLen);
    if (succ)
      return {prev_idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> rem_min(dictionary& dict, schema::uint32& size) {
    auto [val, idx, succ] = dict.dicturemmin(KeyLen);
    if (succ) {
      size--;
      return {idx, schema::parse<Element>(val), succ};
    }
    return {0, {}, succ};
  }
  __always_inline
  static std::tuple<unsigned, Element, bool> rem_max(dictionary& dict, schema::uint32& size) {
    auto [val, idx, succ] = dict.dicturemmax(KeyLen);
    if (succ) {
      size--;
      return {idx, schema::parse<Element>(val), succ};
    }
    return {0, {}, succ};
  }
};

} // namespace tvm

