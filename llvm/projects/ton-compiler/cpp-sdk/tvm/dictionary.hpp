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
  void dictusetref(cell val, unsigned key, unsigned key_len) {
    dict_ = __builtin_tvm_dictusetref(val.get(), key, get(), key_len);
  }
  std::tuple<slice, bool> dictusetget(slice val, unsigned key, unsigned key_len) {
    auto [dict, sl, succ] = __builtin_tvm_dictusetget(val.get(), key, get(), key_len);
    dict_ = dict;
    return std::tuple(sl, succ);
  }
  std::tuple<cell, bool> dictusetgetref(cell val, unsigned key, unsigned key_len) {
    auto [dict, cl, succ] = __builtin_tvm_dictusetgetref(val.get(), key, get(), key_len);
    dict_ = dict;
    return std::tuple(cl, succ);
  }
  std::tuple<slice, bool> dictsetget(slice val, slice key, unsigned key_len) {
    auto [dict, sl, succ] = __builtin_tvm_dictsetget(val.get(), key, get(), key_len);
    dict_ = dict;
    return std::tuple(sl, succ);
  }
  std::tuple<cell, bool> dictsetgetref(cell val, slice key, unsigned key_len) {
    auto [dict, cl, succ] = __builtin_tvm_dictsetgetref(val.get(), key, get(), key_len);
    dict_ = dict;
    return std::tuple(cl, succ);
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
  std::tuple<cell, unsigned, bool> dictuminref(unsigned key_len) const {
    auto [cl, idx, succ] = __builtin_tvm_dictuminref(get(), key_len);
    return std::tuple(cl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dictugetnext(unsigned prev_idx, unsigned key_len) const {
    auto [sl, idx, succ] = __builtin_tvm_dictugetnext(prev_idx, get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dictumax(unsigned key_len) const {
    auto [sl, idx, succ] = __builtin_tvm_dictumax(get(), key_len);
    return std::tuple(sl, idx, succ);
  }
  std::tuple<cell, unsigned, bool> dictumaxref(unsigned key_len) const {
    auto [cl, idx, succ] = __builtin_tvm_dictumaxref(get(), key_len);
    return std::tuple(cl, idx, succ);
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
  std::tuple<cell, unsigned, bool> dicturemminref(unsigned key_len) {
    auto [dict, cl, idx, succ] = __builtin_tvm_dicturemminref(get(), key_len);
    dict_ = dict;
    return std::tuple(cl, idx, succ);
  }
  std::tuple<slice, unsigned, bool> dicturemmax(unsigned key_len) {
    auto [dict, sl, idx, succ] = __builtin_tvm_dicturemmax(get(), key_len);
    dict_ = dict;
    return std::tuple(sl, idx, succ);
  }
  std::tuple<cell, unsigned, bool> dicturemmaxref(unsigned key_len) {
    auto [dict, cl, idx, succ] = __builtin_tvm_dicturemmaxref(get(), key_len);
    dict_ = dict;
    return std::tuple(cl, idx, succ);
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

  bool dictdel(slice key, unsigned key_len) {
    auto [dict, succ] = __builtin_tvm_dictdel(key, get(), key_len);
    dict_ = dict;
    return succ;
  }

  cell get() const { return dict_(); }

  bool empty() const { return get().isnull(); }

  schema::anydict dict_;
};

} // namespace tvm

