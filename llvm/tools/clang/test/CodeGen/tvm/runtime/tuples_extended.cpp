// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -o - | tvm-testrun --no-trace --entry main | FileCheck %s -check-prefix=CHECK-RUN

// CHECK-RUN-NOT: Unhandled exception
int main() {
  auto tup_internal = __builtin_tvm_tuple(156, 44);
  auto tup_external = __builtin_tvm_tuple(124, tup_internal);
  auto tup_external2 = __builtin_tvm_tuple(177, tup_external);
  
  auto v1 = __builtin_tvm_cadr(tup_external);
  __builtin_tvm_throwif(v1 != 156, 11);

  auto v2 = __builtin_tvm_cddr(tup_external);
  __builtin_tvm_throwif(v2 != 44, 12);

  auto v3 = __builtin_tvm_caddr(tup_external2);
  __builtin_tvm_throwif(v3 != 156, 13);

  auto v4 = __builtin_tvm_cdddr(tup_external2);
  __builtin_tvm_throwif(v4 != 44, 14);

  auto v5 = __builtin_tvm_index2(tup_external, 1, 0);
  __builtin_tvm_throwif(v5 != 156, 15);

  auto v6 = __builtin_tvm_index2(tup_external, 1, 1);
  __builtin_tvm_throwif(v6 != 44, 16);

  auto v7 = __builtin_tvm_index3(tup_external2, 1, 1, 0);
  __builtin_tvm_throwif(v7 != 156, 17);

  auto v8 = __builtin_tvm_index3(tup_external2, 1, 1, 1);
  __builtin_tvm_throwif(v8 != 44, 18);

  __tvm_tuple v9 = __builtin_tvm_cast_to_tuple(__builtin_tvm_index2(tup_external2, 1, 1));
  auto [v10, v11] = __builtin_tvm_untuple2(v9);
  __builtin_tvm_throwif(v10 != 156, 19);
  __builtin_tvm_throwif(v11 != 44, 20);

  __tvm_tuple v12 = (__tvm_tuple)__builtin_tvm_index2(tup_external2, 1, 1);
  auto [v13, v14] = __builtin_tvm_untuple2(v12);
  __builtin_tvm_throwif(v13 != 156, 21);
  __builtin_tvm_throwif(v14 != 44, 22);

  return 0;
}

