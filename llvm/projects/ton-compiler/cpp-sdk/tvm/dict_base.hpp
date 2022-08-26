#pragma once

#include <optional>
#include <tuple>

#include <tvm/assert.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/error_code.hpp>

#include <tvm/schema/make_parser.hpp>
#include <tvm/schema/make_builder.hpp>
#include <tvm/dict_traits.hpp>

namespace tvm {

template<class Element, unsigned KeyLen>
class dict_base {
public:
  using traits = dict_traits<Element, KeyLen>;

  void clear() {
    size_ = 0;
    dict_.clear();
  }
  bool empty() const { return dict_.empty(); }

  schema::uint32 size() const { return size_; }

  std::optional<Element> lookup(unsigned key) const {
    return traits::lookup(dict_, key);
  }

  Element get_or_throw(unsigned key, unsigned exception) const {
    auto opt_val = lookup(key);
    require(!!opt_val, exception);
    return *opt_val;
  }

  // Returns {Key, Element, Success} to match the solidity array.min() function
  //  (not {Element, Key, Success} as dictumin returns)
  std::tuple<unsigned, Element, bool> min() const {
    return traits::min(dict_);
  }
  std::tuple<unsigned, Element, bool> next(unsigned idx) const {
    return traits::next(dict_, idx);
  }
  std::tuple<unsigned, Element, bool> max() const {
    return traits::max(dict_);
  }
  std::tuple<unsigned, Element, bool> prev(unsigned idx) const {
    return traits::prev(dict_, idx);
  }
  std::tuple<unsigned, Element, bool> rem_min() {
    return traits::rem_min(dict_, size_);
  }
  std::tuple<unsigned, Element, bool> rem_max() {
    return traits::rem_max(dict_, size_);
  }

  schema::uint32 size_;
  dictionary dict_;
};

} // namespace tvm

