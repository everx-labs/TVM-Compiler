// RUN: %clang -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s

__attribute__((noinline))
unsigned hw1 () {
  int c = 0;
  while (c < 3)
  {
    c = c + 1;
  }
  return c;
}

int main() {
  int retval = hw1();
// CHECK-NOT: custom error
  if (retval != 3)
    __builtin_tvm_throwif(1, 13);

  return 0;
}

