#pragma once

#include <experimental/coroutine>
#include <tvm/schema/message.hpp>

namespace tvm { namespace schema {

struct promise_type_void_impl {
  using coro_handle = std::experimental::coroutine_handle<promise_type_void_impl>;
  __always_inline
  auto get_return_object() {
    return coro_handle::from_promise(*this);
  }
  __always_inline
  auto initial_suspend() { return std::experimental::suspend_never(); }
  __always_inline
  auto final_suspend() { return std::experimental::suspend_always(); }
  __always_inline
  void return_void() {}
  __always_inline
  void unhandled_exception() {
    std::terminate();
  }
  __always_inline
  void set_addr(address addr) { wait_addr_ = addr; }
  address wait_addr_;
};

template<class T>
struct promise_type_impl {
  using coro_handle = std::experimental::coroutine_handle<promise_type_impl<T>>;
  __always_inline
  auto get_return_object() {
    return coro_handle::from_promise(*this);
  }
  __always_inline
  auto initial_suspend() { return std::experimental::suspend_never(); }
  __always_inline
  auto final_suspend() { return std::experimental::suspend_always(); }
  __always_inline
  void return_value(T val) { val_ = val; }
  __always_inline
  void unhandled_exception() {
    std::terminate();
  }
  __always_inline
  void set_addr(address addr) { wait_addr_ = addr; }
  T val_;
  address wait_addr_;
};

template<class T>
class resumable {
public:
  using promise_type = std::conditional_t<std::is_void_v<T>, promise_type_void_impl, promise_type_impl<T>>;
  using coro_handle = std::experimental::coroutine_handle<promise_type>;

  __always_inline
  resumable(coro_handle handle) : handle_(handle) { }
  __always_inline
  bool resume() {
    if (!handle_.done())
      handle_.resume();
    return !handle_.done();
  }
  __always_inline
  bool done() const { return handle_.done(); }

  __always_inline
  ~resumable() { handle_.destroy(); }

  template<class T1 = T, std::enable_if_t<!std::is_void_v<T1>, int> = 0>
  __always_inline
  T return_val() const {
    return handle_.promise().val_;
  }
  __always_inline
  address wait_addr() const { return handle_.promise().wait_addr_; }
  coro_handle handle_;
};

template<class T>
struct resumable_subtype {
  using type = T;
};
template<class T>
struct resumable_subtype<resumable<T>> {
  using type = T;
};

template<auto func, class Resumable>
__always_inline
__tvm_cell serialize_resumable(__tvm_builder b, Resumable val) {
  return __builtin_coro_tvm_serialize(b, val.handle_.address(), reinterpret_cast<void*>(func));
}

template<class Resumable, auto func, class Contract>
__always_inline
Resumable deserialize_resumable(__tvm_slice sl, Contract* new_this) {
  using coro_handle = typename Resumable::coro_handle;
  return { coro_handle::from_address(
    __builtin_coro_tvm_deserialize(sl, reinterpret_cast<void*>(func), (void*)new_this)) };
}

}} // namespace tvm::schema

