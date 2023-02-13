#pragma once

#include <optional>
#include <tvm/builder.hpp>
#include <tvm/error_code.hpp>
#include <tvm/assert.hpp>
#include <tvm/schema/basics.hpp>

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
  unsigned size() const { return top_ ? (__builtin_tvm_tlen(tup_) + 1) : 0; }
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
  unsigned size() const { return top_ ? (__builtin_tvm_tlen(tup_) + 1) : 0; }
  slice& top() { return *top_; }
private:
  std::optional<slice> top_;
  __tvm_tuple tup_;
};

// implementation of tuple_stack for cell elements
template<>
class tuple_stack<cell> {
public:
  tuple_stack() : tup_(__builtin_tvm_tuple()) {}
  bool empty() {
    return !top_;
  }
  void push(cell val) {
    if (top_)
      tup_ = __builtin_tvm_tpush(tup_, __builtin_tvm_cast_from_cell(top_->get()));
    top_ = val;
  }
  void pop() {
    require(!!top_, error_code::empty_container);
    if (__builtin_tvm_tlen(tup_)) {
      auto [=tup_, val] = __builtin_tvm_tpop(tup_);
      top_ = cell(__builtin_tvm_cast_to_cell(val));
    } else {
      top_.reset();
    }
  }
  optcell extract() {
    if (!empty()) {
      cell rv = top();
      pop();
      return { rv };
    }
    return {};
  }
  unsigned size() const { return top_ ? (__builtin_tvm_tlen(tup_) + 1) : 0; }
  cell& top() { return *top_; }
private:
  std::optional<cell> top_;
  __tvm_tuple tup_;
};

// implementation of tuple_stack for unsigned elements
template<>
class tuple_stack<unsigned> {
public:
  tuple_stack() : tup_(__builtin_tvm_tuple()) {}
  bool empty() {
    return !top_;
  }
  void push(unsigned val) {
    if (top_)
      tup_ = __builtin_tvm_tpush(tup_, *top_);
    top_ = val;
  }
  void pop() {
    require(!!top_, error_code::empty_container);
    if (__builtin_tvm_tlen(tup_)) {
      auto [=tup_, val] = __builtin_tvm_tpop(tup_);
      top_ = val;
    } else {
      top_.reset();
    }
  }
  std::optional<unsigned> extract() {
    if (!empty()) {
      auto rv = top();
      pop();
      return { rv };
    }
    return {};
  }
  unsigned size() const { return top_ ? (__builtin_tvm_tlen(tup_) + 1) : 0; }
  unsigned& top() { return *top_; }
private:
  std::optional<unsigned> top_;
  __tvm_tuple tup_;
};

// implementation of tuple_stack for nested raw tuple elements
template<>
class tuple_stack<__tvm_tuple> {
public:
  tuple_stack() : tup_(__builtin_tvm_tuple()) {}
  bool empty() {
    return !top_;
  }
  void push(__tvm_tuple val) {
    if (top_)
      tup_ = __builtin_tvm_tpush(tup_, (unsigned)*top_);
    top_ = val;
  }
  void pop() {
    require(!!top_, error_code::empty_container);
    if (__builtin_tvm_tlen(tup_)) {
      auto [=tup_, val] = __builtin_tvm_tpop(tup_);
      top_ = (__tvm_tuple)val;
    } else {
      top_.reset();
    }
  }
  std::optional<__tvm_tuple> extract() {
    if (!empty()) {
      auto rv = top();
      pop();
      return { rv };
    }
    return {};
  }
  unsigned size() const { return top_ ? (__builtin_tvm_tlen(tup_) + 1) : 0; }
  __tvm_tuple& top() { return *top_; }
private:
  std::optional<__tvm_tuple> top_;
  __tvm_tuple tup_;
};

} // namespace tvm

