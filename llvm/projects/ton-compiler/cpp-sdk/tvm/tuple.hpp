#pragma once

#include <tvm/untuple_caller.hpp>
#include <tvm/unpackfirst_caller.hpp>

namespace tvm {

/// Template, to pack struct T into tuple
template<class T>
class __attribute__((tvm_tuple)) tuple {
public:
  tuple() {
    tp_ = (__tvm_tuple)__builtin_tvm_pushnull();
  }

  /// Constructor - wrap existing tuple value
  explicit tuple(__tvm_tuple tp) : tp_(tp) {}

  /// Constructor - pack struct T into tuple
  explicit tuple(T tpVal) : tp_(__builtin_tvm_tuple(tpVal)) {}

  /// Unpack tuple into struct value
  T unpack() const { return tvm::untuple_caller<sizeof(T)>::untuple(tp_); }

  /// Pack struct value into tuple
  void pack(const T &tpVal) { tp_ = __builtin_tvm_tuple(tpVal); }

  static tuple<T> create(T val) {
    tuple<T> rv;
    rv.pack(val);
    return rv;
  }

  T unpackfirst() const { return tvm::unpackfirst_caller<sizeof(T)>::unpack(tp_); }

  /// Get one-byte value from offset. TODO: implement better way to request field.
  template<typename F>
  F index(unsigned offset) const {
    auto val = __builtin_tvm_index(tp_, offset);
    return *reinterpret_cast<const F*>(&val);
  }

  /// Set one-byte value to offset in tuple. TODO: implement better way to request field.
  template<typename F>
  void setindex(unsigned offset, F val) {
    tp_ = __builtin_tvm_setindex(tp_, *reinterpret_cast<const unsigned*>(&val), offset);
  }

  /// Is it a null tuple
  bool isnull() const { return __builtin_tvm_isnull_tuple(tp_); }

  /// Reset tuple to null
  void reset() {
    tp_ = (__tvm_tuple)__builtin_tvm_pushnull();
  }

  __tvm_tuple get() const { return tp_; }

  operator __tvm_tuple() const { return get(); }
private:
  __tvm_tuple tp_;
};

template<class T>
inline tuple<T> entuple(T v) {
  return tuple<T>(v);
}

template<class T>
inline T detuple(__tvm_tuple tup) {
  return tuple<T>(tup).unpack();
}

} // namespace tvm
