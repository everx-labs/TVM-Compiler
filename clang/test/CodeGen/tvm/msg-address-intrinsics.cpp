// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o - | FileCheck %s
// REQUIRES: tvm-registered-target

int get_msg_kind(__tvm_slice sl) {
// CHECK: get_msg_kind
// CHECK: @llvm.tvm.ldmsgaddr
// CHECK: @llvm.tvm.parsemsgaddr

  auto [prefix, remainder] = __builtin_tvm_ldmsgaddr(sl);
  __tvm_tuple tp = __builtin_tvm_parsemsgaddr(prefix);
  return __builtin_tvm_index(tp, 0);
}

int get_msg_kind_quiet(__tvm_slice sl) {
// CHECK: get_msg_kind_quiet
// CHECK: @llvm.tvm.ldmsgaddrq
// CHECK: @llvm.tvm.parsemsgaddrq

  auto [prefix, remainder, succ1] = __builtin_tvm_ldmsgaddrq(sl);
  if (!succ1) return -1;
  auto [tp, succ2] = __builtin_tvm_parsemsgaddrq(prefix);
  if (!succ2) return -1;
  return __builtin_tvm_index(tp, 0);
}

struct __attribute__((tvm_tuple)) parsed_addr_std {
  int workchain;
  int address;
};

parsed_addr_std rewritestdaddr(__tvm_slice sl) {
// CHECK: rewritestdaddr
// CHECK: @llvm.tvm.rewritestdaddr

  return __builtin_tvm_rewritestdaddr(sl);
}

template<typename T>
class __attribute__((tvm_tuple)) optional {
public:
  explicit optional(T val) : val_(val), initialized_(true) {}
  optional() : initialized_(false) {}
  bool is_initialized() const { return initialized_; }
  T get() const { return val_; }
private:
  bool initialized_;
  T val_;
};

optional<parsed_addr_std> rewritestdaddrq(__tvm_slice sl) {
// CHECK: rewritestdaddrq
// CHECK: @llvm.tvm.rewritestdaddrq

  auto [workchain, address, succ] = __builtin_tvm_rewritestdaddrq(sl);
  if (succ) {
    return optional<parsed_addr_std>(parsed_addr_std({workchain, address}));
  }
  return optional<parsed_addr_std>();
}

struct __attribute__((tvm_tuple)) parsed_addr_var {
  int workchain;
  __tvm_slice address;
};

parsed_addr_var rewritevaraddr(__tvm_slice sl) {
// CHECK: rewritevaraddr
// CHECK: @llvm.tvm.rewritevaraddr

  return __builtin_tvm_rewritevaraddr(sl);
}

optional<parsed_addr_var> rewritevaraddrq(__tvm_slice sl) {
// CHECK: rewritevaraddrq
// CHECK: @llvm.tvm.rewritevaraddrq

  auto [workchain, address, succ] = __builtin_tvm_rewritevaraddrq(sl);
  if (succ) {
    return optional<parsed_addr_var>(parsed_addr_var({workchain, address}));
  }
  return optional<parsed_addr_var>();
}

