#pragma once

#include <tvm/error_code.hpp>
#include <tvm/cell.hpp>
#include <tvm/dictionary.hpp>
#include <tvm/schema/make_parser.hpp>

namespace tvm {

template<class Format>
class persistent_dict_elem {
public:
  persistent_dict_elem() : requested_(false), initialized_(false), changed_(false) {}
  bool initialized() {
    ensure_requested();
    return initialized_;
  }
  bool changed() const { return changed_; }
  Format get() {
    ensure_requested();
    return val_;
  }
  persistent_dict_elem& operator=(Format val) {
    val_ = val;
    changed_ = true;
    return *this;
  }
  operator Format() {
    return get();
  }
  void set_dict(dictionary dict, unsigned idx) {
    dict_ = dict;
    idx_ = idx;
  }
private:
  inline void ensure_requested() {
    if (!changed_ && !requested_) {
      requested_ = true;
      auto[val_slice, succ] = dict_.dictiget(idx_, 64);
      if (succ) {
        val_ = schema::parse<Format>(parser(val_slice),
                                     error_code::persistent_data_parse_error,
                                     true);
        initialized_ = true;
      }
    }
  }
  bool requested_;
  bool initialized_;
  bool changed_;
  dictionary dict_;
  unsigned idx_;
  Format val_;
};

class persistent_data {
public:
  static inline cell get() {
    return __builtin_tvm_get_persistent_data();
  }
  static inline void set(cell cl) {
    return __builtin_tvm_set_persistent_data(cl);
  }
};

} // namespace tvm

