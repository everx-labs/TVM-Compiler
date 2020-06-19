#pragma once

#include <tuple>

#include <tvm/cell.hpp>
#include <tvm/slice.hpp>
#include <tvm/builder.hpp>

#include <tvm/schema/make_builder.hpp>
#include <tvm/schema/make_parser.hpp>
#include <tvm/assert.hpp>

namespace tvm {

class dictionary {
public:
  dictionary() {}
  explicit dictionary(cell dict) : dict_{dict} {}
  dictionary(schema::anydict dict) : dict_{dict} {}

  void clear() {
    dict_(cell{});
  }

  static dictionary get_persistent() { 
    return dictionary(__builtin_tvm_plddict(
      __builtin_tvm_ctos(__builtin_tvm_get_persistent_data())));
  }

  static void set_persistent(dictionary dict) {
    __builtin_tvm_set_persistent_data(builder().stdict(dict.get()).endc());
  }

  std::tuple<slice, bool> dictget(slice key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictget(key, get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictgetref(slice key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictgetref(key, get(), key_len);
    return std::tuple(cell(cl), succ);
  }
  std::tuple<slice, bool> dictiget(int key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictiget(key, get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictigetref(int key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictigetref(key, get(), key_len);
    return std::tuple(cell(cl), succ);
  }
  std::tuple<slice, bool> dictuget(unsigned key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictuget(key, get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictugetref(unsigned key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictugetref(key, get(), key_len);
    return std::tuple(cell(cl), succ);
  }

  void dictset(slice val, slice key, unsigned key_len) {
    dict_ = __builtin_tvm_dictset(val.get(), key, get(), key_len);
  }
  void dictsetref(cell val, slice key, unsigned key_len) {
    dict_ = __builtin_tvm_dictsetref(val.get(), key, get(), key_len);
  }
  void dictiset(slice val, int key, unsigned key_len) {
    dict_ = __builtin_tvm_dictiset(val.get(), key, get(), key_len);
  }
  void dictuset(slice val, unsigned key, unsigned key_len) {
    dict_ = __builtin_tvm_dictuset(val.get(), key, get(), key_len);
  }
  std::tuple<slice, bool> dictusetget(slice val, unsigned key, unsigned key_len) {
    auto [dict, sl, succ] = __builtin_tvm_dictusetget(val.get(), key, get(), key_len);
    dict_ = dict;
    return std::tuple(sl, succ);
  }

  std::tuple<slice, slice, bool> dictmin(unsigned key_len) const {
    auto [sl, key_sl, succ] = __builtin_tvm_dictmin(get(), key_len);
    return std::tuple(sl, key_sl, succ);
  }
  std::tuple<slice, slice, bool> dictgetnext(slice prev_idx, unsigned key_len) const {
    auto [sl, key_sl, succ] = __builtin_tvm_dictgetnext(prev_idx, get(), key_len);
    return std::tuple(sl, key_sl, succ);
  }

  std::tuple<slice, unsigned, bool> dictumin(unsigned key_len) const {
    auto [sl, idx, succ] = __builtin_tvm_dictumin(get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dictugetnext(unsigned prev_idx, unsigned key_len) const {
    auto [sl, idx, succ] = __builtin_tvm_dictugetnext(prev_idx, get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dictumax(unsigned key_len) {
    auto [sl, idx, succ] = __builtin_tvm_dictumax(get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dictugetprev(unsigned next_idx, unsigned key_len) const {
    auto [sl, idx, succ] = __builtin_tvm_dictugetprev(next_idx, get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dicturemmin(unsigned key_len) {
    auto [dict, sl, idx, succ] = __builtin_tvm_dicturemmin(get(), key_len);
    dict_ = dict;
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dicturemmax(unsigned key_len) {
    auto [dict, sl, idx, succ] = __builtin_tvm_dicturemmax(get(), key_len);
    dict_ = dict;
    return std::tuple(sl, idx, succ);
  }

  std::tuple<slice, bool> dictdelget(slice key, unsigned key_len) {
    auto [dict, sl, succ] = __builtin_tvm_dictdelget(key, get(), key_len);
    dict_ = dict;
    return std::tuple(slice(sl), succ);
  }

  std::tuple<cell, bool> dictdelgetref(slice key, unsigned key_len) {
    auto [dict, cl, succ] = __builtin_tvm_dictdelgetref(key, get(), key_len);
    dict_ = dict;
    return std::tuple(cell(cl), succ);
  }

  bool dictudel(unsigned key, unsigned key_len) {
    auto [dict, succ] = __builtin_tvm_dictudel(key, get(), key_len);
    dict_ = dict;
    return succ;
  }

  cell get() const { return dict_(); }

  bool empty() const { return get().isnull(); }

  schema::anydict dict_;
};

template<class Element, unsigned KeyLen>
struct dictionary_const_iterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = Element*;
  using reference = Element&;

  dictionary dict_;
  unsigned idx_;
  std::optional<slice> sl_;

  __always_inline Element operator*() const {
    return schema::parse<Element>(*sl_);
  }

  __always_inline bool is_end() const { return !sl_; }

  static dictionary_const_iterator create_begin(dictionary dict) {
    std::optional<slice> opt_sl;
    auto [sl, idx, succ] = dict.dictumin(KeyLen);
    if (succ)
      opt_sl = sl;
    return dictionary_const_iterator{dict, idx, opt_sl};
  }
  static dictionary_const_iterator create_end(dictionary dict) {
    return dictionary_const_iterator{{}, 0, {}};
  }

  __always_inline dictionary_const_iterator operator++() {
    require(!!sl_, error_code::iterator_overflow);
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      idx_ = next_idx;
      sl_ = sl;
    } else {
      sl_.reset();
    }
    return *this;
  }
  bool operator==(dictionary_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return left_end && right_end;
  }
  bool operator!=(dictionary_const_iterator v) const {
    return !(*this == v);
  }
};

template<class Element, unsigned KeyLen>
struct ElementRef {
  __always_inline ElementRef& operator=(Element elem) {
    auto [sl, succ] = dict_.dictusetget(schema::build(elem).make_slice(), idx_, KeyLen);
    if (!succ)
      ++size_;
    return *this;
  }
  operator bool() const { return !!static_cast<Element>(*this); }

  __always_inline void swap(ElementRef v) const {
    auto [sl1, succ1] = dict_.dictuget(idx_, KeyLen);
    require(succ1, error_code::iterator_overflow);
    auto [sl2, succ2] = v.dict_.dictuget(v.idx_, KeyLen);
    require(succ2, error_code::iterator_overflow);
    dict_.dictuset(sl2, idx_, KeyLen);
    v.dict_.dictuset(sl1, v.idx_, KeyLen);
  }
  dictionary& dict_;
  schema::uint32& size_;
  unsigned idx_;
};
  
template<class Element, unsigned KeyLen>
struct dictionary_array_iterator : boost::operators<dictionary_array_iterator<Element, KeyLen>> {
  using iterator_category = std::random_access_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = Element*;
  using reference = ElementRef<Element, KeyLen>;

  dictionary& dict_;
  unsigned idx_;
  schema::uint32 size_;

  __always_inline Element operator*() const {
    auto [sl, succ] = dict_.dictuget(idx_, KeyLen);
    require(succ, error_code::iterator_overflow);
    return schema::parse<Element>(sl);
  }
  __always_inline reference operator*() {
    return reference{dict_, size_, idx_};
  }

  __always_inline bool is_end() const { return idx_ >= size_.get(); }

  static dictionary_array_iterator create_begin(dictionary& dict, schema::uint32 size) {
    return dictionary_array_iterator{{}, dict, 0, size};
  }
  static dictionary_array_iterator create_end(dictionary& dict, schema::uint32 size) {
    return dictionary_array_iterator{{}, dict, size.get(), size};
  }

  bool operator<(dictionary_array_iterator x) const { return idx_ < x.idx_; }
  
  dictionary_array_iterator& operator+=(int v) {
    idx_ += v;
    return *this;
  }
  dictionary_array_iterator& operator-=(int v) {
    idx_ -= v;
    return *this;
  }
  dictionary_array_iterator& operator++() {
    ++idx_;
    return *this;
  }
  dictionary_array_iterator& operator--() {
    --idx_;
    return *this;
  }
  bool operator==(dictionary_array_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};
template<class Element, unsigned KeyLen>
void swap(ElementRef<Element, KeyLen> v1, ElementRef<Element, KeyLen> v2) {
  v1.swap(v2);
}

template<class Element, unsigned KeyLen>
struct dictionary_map_iterator : boost::operators<dictionary_map_iterator<Element, KeyLen>> {
  using Pair = std::pair<schema::uint_t<KeyLen>, Element>;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = std::pair<schema::uint_t<KeyLen>, Element>*;
  using reference = std::pair<schema::uint_t<KeyLen>, Element>&;

  dictionary& dict_;
  unsigned idx_;
  std::optional<slice> sl_;

  __always_inline Pair operator*() const {
    require(!!sl_, error_code::iterator_overflow);
    return {idx_, schema::parse<Element>(*sl_)};
  }
  __always_inline bool is_end() const { return !sl_; }

  static dictionary_map_iterator create_begin(dictionary& dict) {
    std::optional<slice> opt_sl;
    auto [sl, idx, succ] = dict.dictumin(KeyLen);
    if (succ)
      opt_sl = sl;
    return dictionary_map_iterator{{}, dict, idx, opt_sl};
  }
  static dictionary_map_iterator create_end(dictionary& dict) {
    return dictionary_map_iterator{{}, dict, 0, {}};
  }

  bool operator<(dictionary_map_iterator x) const { return idx_ < x.idx_; }

  dictionary_map_iterator& operator++() {
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = next_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  dictionary_map_iterator& operator--() {
    auto [sl, prev_idx, succ] = dict_.dictugetprev(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = prev_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  bool operator==(dictionary_map_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};

template<class Element, unsigned KeyLen>
struct dictionary_map_const_iterator : boost::operators<dictionary_map_const_iterator<Element, KeyLen>> {
  using Pair = std::pair<schema::uint_t<KeyLen>, Element>;

  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = Element;
  using difference_type = int;
  using pointer = const std::pair<schema::uint_t<KeyLen>, Element>*;
  using reference = const std::pair<schema::uint_t<KeyLen>, Element>&;

  dictionary dict_;
  unsigned idx_;
  std::optional<slice> sl_;

  __always_inline Pair operator*() const {
    require(!!sl_, error_code::iterator_overflow);
    return {schema::uint_t<KeyLen>(idx_), schema::parse<Element>(*sl_)};
  }
  __always_inline bool is_end() const { return !sl_; }

  static dictionary_map_const_iterator create_begin(dictionary dict) {
    std::optional<slice> opt_sl;
    auto [sl, idx, succ] = dict.dictumin(KeyLen);
    if (succ)
      opt_sl = sl;
    return dictionary_map_const_iterator{{}, dict, idx, opt_sl};
  }
  static dictionary_map_const_iterator create_end(dictionary dict) {
    return dictionary_map_const_iterator{{}, dict, 0, {}};
  }

  bool operator<(dictionary_map_const_iterator x) const { return idx_ < x.idx_; }

  dictionary_map_const_iterator& operator++() {
    auto [sl, next_idx, succ] = dict_.dictugetnext(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = next_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  dictionary_map_const_iterator& operator--() {
    auto [sl, prev_idx, succ] = dict_.dictugetprev(idx_, KeyLen);
    if (succ) {
      sl_ = sl;
      idx_ = prev_idx;
    } else {
      sl_.reset();
    }
    return *this;
  }
  bool operator==(dictionary_map_const_iterator v) const {
    bool left_end = is_end();
    bool right_end = v.is_end();
    return (left_end && right_end) || (!left_end && !right_end && idx_ == v.idx_);
  }
};

} // namespace tvm

