#pragma once

#include <optional>
#include <tuple>

#include <tvm/assert.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/error_code.hpp>

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
class dict_base {
public:
  void clear() {
    size_ = 0;
    dict_.clear();
  }
  bool empty() const { return dict_.empty(); }

  schema::uint32 size() const { return size_; }

  std::optional<Element> lookup(unsigned key) const {
    auto [slice, succ] = dict_.dictuget(key, KeyLen);
    if (succ)
      return schema::parse<Element>(slice);
    return {};
  }

  Element get_or_throw(unsigned key, unsigned exception) const {
    auto opt_val = lookup(key);
    require(!!opt_val, exception);
    return *opt_val;
  }

  // Returns {Key, Element, Success} to match the solidity array.min() function
  //  (not {Element, Key, Success} as dictumin returns)
  std::tuple<unsigned, Element, bool> min() const {
    auto [val, idx, succ] = dict_.dictumin(KeyLen);
    if (succ)
      return {idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  std::tuple<unsigned, Element, bool> next(unsigned idx) const {
    auto [val, next_idx, succ] = dict_.dictugetnext(idx, KeyLen);
    if (succ)
      return {next_idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  std::tuple<unsigned, Element, bool> max() const {
    auto [val, idx, succ] = dict_.dictumax(KeyLen);
    if (succ)
      return {idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  std::tuple<unsigned, Element, bool> prev(unsigned idx) const {
    auto [val, prev_idx, succ] = dict_.dictugetprev(idx, KeyLen);
    if (succ)
      return {prev_idx, schema::parse<Element>(val), succ};
    return {0, {}, succ};
  }
  std::tuple<unsigned, Element, bool> rem_min() {
    auto [val, idx, succ] = dict_.dicturemmin(KeyLen);
    if (succ) {
      size_--;
      return {idx, schema::parse<Element>(val), succ};
    }
    return {0, {}, succ};
  }
  std::tuple<unsigned, Element, bool> rem_max() {
    auto [val, idx, succ] = dict_.dicturemmax(KeyLen);
    if (succ) {
      size_--;
      return {idx, schema::parse<Element>(val), succ};
    }
    return {0, {}, succ};
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

