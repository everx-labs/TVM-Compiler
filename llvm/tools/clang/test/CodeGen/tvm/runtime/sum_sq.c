// RUN: %clang -O1 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s
// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s

#define int long long 

int sq(int x) { return x * x; }

__attribute__((noinline))
int sum(int n) {
  int result = 0;
  for(int i = 0; i < n; ++i) {
    result += sq(i);
  }
  return result;
}

int main() {
  int correct = 1;

  int retval;
  
  retval = sum(3);
  correct &= (retval == 5);

  retval = sum(10);
  correct &= (retval == 285);

  retval = sum(1000);
  correct &= (retval == 332833500);

// CHECK-NOT: custom error
  if (!correct)
    __builtin_tvm_throwif(1, 13);

  return 0;
}

