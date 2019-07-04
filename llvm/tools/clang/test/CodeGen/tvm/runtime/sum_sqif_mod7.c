// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s
int sq_m7(int x) {
  return __builtin_tvm_mod(x * x, 7);
}

int sum(int n) {
  int result = 0;
  for(int i = 0; i < n; ++i) {
    if (__builtin_tvm_mod(result + i, 2))
      result += sq_m7(i);
  }
  return result;
}

int main()
{
  int correct = 1;

  int retval = sum(3);
  correct &= (retval == 5);

  retval = sum(10);
  correct &= (retval == 12);

  retval = sum(1000);
  correct &= (retval == 1001);

// CHECK-NOT: custom error
  if (!correct)
    __builtin_tvm_throwif(1, 13);

  return 0;
}
