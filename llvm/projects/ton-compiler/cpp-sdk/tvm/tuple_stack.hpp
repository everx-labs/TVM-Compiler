#pragma once

#include <optional>
#include <tvm/builder.hpp>
#include <tvm/error_code.hpp>
#include <tvm/assert.hpp>

namespace tvm {

// tuple_stack allows to store supported elements in growing stack in tvm tuple
template<class T>
class tuple_stack {
};

// implementation of tuple_stack for builder elements
template<>
class tuple_stack<builder> {
public:
  tuple_stack() : tup_(__builtin_tvm_tuple()) {}
  bool empty() {
    return !top_;
  }
  void push(builder val) {
    if (top_)
      tup_ = __builtin_tvm_tpush(tup_, __builtin_tvm_cast_from_builder(top_->get()));
    top_ = val;
  }
  void pop() {
    require(!!top_, error_code::empty_container);
    if (__builtin_tvm_tlen(tup_)) {
      auto [=tup_, val] = __builtin_tvm_tpop(tup_);
      top_ = builder(__builtin_tvm_cast_to_builder(val));
    } else {
      top_.reset();
    }
  }
  builder& top() { return *top_; }
private:
  std::optional<builder> top_;
  __tvm_tuple tup_;
};

// implementation of tuple_stack for slice elements
template<>
class tuple_stack<slice> {
public:
  tuple_stack() : tup_(__builtin_tvm_tuple()) {}
  bool empty() {
    return !top_;
  }
  void push(slice val) {
    if (top_)
      tup_ = __builtin_tvm_tpush(tup_, __builtin_tvm_cast_from_slice(top_->get()));
    top_ = val;
  }
  void pop() {
    require(!!top_, error_code::empty_container);
    if (__builtin_tvm_tlen(tup_)) {
      auto [=tup_, val] = __builtin_tvm_tpop(tup_);
      top_ = slice(__builtin_tvm_cast_to_slice(val));
    } else {
      top_.reset();
    }
  }
  slice& top() { return *top_; }
private:
  std::optional<slice> top_;
  __tvm_tuple tup_;
};

} // namespace tvm

