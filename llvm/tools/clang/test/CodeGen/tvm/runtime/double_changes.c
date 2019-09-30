// RUN: %clang -O3 -S -c -target tvm %s -o - | FileCheck %s
// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s -check-prefix=CHECK-RUN

__attribute__((noinline))
int double_kill(int a, int b, int c, int d) {
// CHECK-LABEL: double_kill
// CHECK: XCHG2
  int rv = a + b;
  return rv * d;
}

__attribute__((noinline))
int double_push(int a, int b) {
// CHECK-LABEL: double_push
// CHECK: PUSH2
  int rv = a + b;
  return a * b - rv;
}

__attribute__((noinline))
int kill_push(int a, int b) {
// CHECK-LABEL: kill_push
// CHECK: XCPU
  int rv = a - b;
  return b * rv;
}

__attribute__((noinline))
int push_kill(int a, int b) {
// CHECK-LABEL: push_kill
// CHECK: PUXC
  int rv = b / a;
  return b * rv;
}

// CHECK-RUN-NOT: custom error
int main() {
  int ret = double_kill(10, 20, 0, 7);
  if (ret != 210)
    __builtin_tvm_throw(13);
  
  ret = double_push(10, 20);
  if (ret != 170)
    __builtin_tvm_throw(13);
  
  ret = kill_push(21, 14);
  if (ret != 98)
    __builtin_tvm_throw(13);

  ret = push_kill(10, 20);
  if (ret != 40)
    __builtin_tvm_throw(13);
  return ret;
}

