// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <type_traits>

struct IWallet {
  void foo() = 1;
  int bar() = 8;
};

template<auto MethodPtr>
using id = __reflect_method_ptr_func_id<std::integral_constant, unsigned, MethodPtr>;

constexpr unsigned func_id = id<&IWallet::bar>::value;

static_assert(func_id == 8);

unsigned f() {
  return func_id;
}
