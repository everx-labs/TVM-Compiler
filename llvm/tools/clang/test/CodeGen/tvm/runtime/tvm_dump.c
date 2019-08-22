// XFAIL: *
// RUN: %clang -O3 -S -c -target tvm -I %S/../../../../../../../samples/sdk-prototype %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s

#include <ton-sdk/debug.h>

int main() {
// CHECK: Test0
  TVM_DEBUG_PRINT("Test0");
// CHECK: Test1
// CHECK-NEXT: 1
  TVM_DEBUG_PRINT("Test1", 1);
// CHECK: Test2
// CHECK-NEXT: 1
// CHECK-NEXT: 2
  TVM_DEBUG_PRINT("Test2", 1, 2);
// CHECK: Test3
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
  TVM_DEBUG_PRINT("Test3", 1, 2, 3);
// CHECK: Test4
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
  TVM_DEBUG_PRINT("Test4", 1, 2, 3, 4);
// CHECK: Test5
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
// CHECK-NEXT: 5
  TVM_DEBUG_PRINT("Test5", 1, 2, 3, 4, 5);
// CHECK: Test6
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
// CHECK-NEXT: 5
// CHECK-NEXT: 6
  TVM_DEBUG_PRINT("Test6", 1, 2, 3, 4, 5, 6);
// CHECK: Test7
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
// CHECK-NEXT: 5
// CHECK-NEXT: 6
// CHECK-NEXT: 7
  TVM_DEBUG_PRINT("Test7", 1, 2, 3, 4, 5, 6, 7);
// CHECK: Test8
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
// CHECK-NEXT: 5
// CHECK-NEXT: 6
// CHECK-NEXT: 7
// CHECK-NEXT: 8
  TVM_DEBUG_PRINT("Test8", 1, 2, 3, 4, 5, 6, 7, 8);
// CHECK: Test9
// CHECK-NEXT: 1
// CHECK-NEXT: 2
// CHECK-NEXT: 3
// CHECK-NEXT: 4
// CHECK-NEXT: 5
// CHECK-NEXT: 6
// CHECK-NEXT: 7
// CHECK-NEXT: 8
// CHECK-NEXT: 9
  TVM_DEBUG_PRINT("Test9", 1, 2, 3, 4, 5, 6, 7, 8, 9);

  return 0;
}

