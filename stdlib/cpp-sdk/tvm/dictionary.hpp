#pragma once

#include <tuple>

#include <tvm/cell.hpp>
#include <tvm/slice.hpp>
#include <tvm/builder.hpp>

namespace tvm {

class dictionary {
public:
  dictionary() {}
  explicit dictionary(cell dict) : dict_(dict) {}

  static dictionary get_persistent() { 
    return dictionary(__builtin_tvm_plddict(
      __builtin_tvm_ctos(__builtin_tvm_get_persistent_data())));
  }

  static void set_persistent(dictionary dict) {
    __builtin_tvm_set_persistent_data(builder().stdict(dict.get()).endc());
  }

  std::tuple<slice, bool> dictget(slice key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictget(key, dict_.get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictgetref(slice key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictgetref(key, dict_.get(), key_len);
    return std::tuple(cell(cl), succ);
  }
  std::tuple<slice, bool> dictiget(int key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictiget(key, dict_.get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictigetref(int key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictigetref(key, dict_.get(), key_len);
    return std::tuple(cell(cl), succ);
  }
  std::tuple<slice, bool> dictuget(unsigned key, unsigned key_len) const {
    auto [sl, succ] = __builtin_tvm_dictuget(key, dict_.get(), key_len);
    return std::tuple(slice(sl), succ);
  }
  std::tuple<cell, bool> dictugetref(unsigned key, unsigned key_len) const {
    auto [cl, succ] = __builtin_tvm_dictugetref(key, dict_.get(), key_len);
    return std::tuple(cell(cl), succ);
  }

  void dictiset(slice val, int key, unsigned key_len) {
    dict_ = __builtin_tvm_dictiset(val.get(), key, dict_.get(), key_len);
  }
  void dictuset(slice val, unsigned key, unsigned key_len) {
    dict_ = __builtin_tvm_dictuset(val.get(), key, dict_.get(), key_len);
  }
  
  cell get() const { return dict_; }
private:
  cell dict_;
};

} // namespace tvm

