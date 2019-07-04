// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s

int min(int arr[], int len) {
  int result = 4;
  for (int i = 0; i < len; ++i)
    if (result > arr[i])
      result = arr[i];
  return result;
}

int max(int arr[], int len) {
  int result = -4;
  for (int i = 0; i < len; ++i)
    if (result < arr[i])
      result = arr[i];
  return result;
}

int main() {
  int status = 1;
  int array[] = {-1, 1, -2, 2, -3, 3};
  status &= min(array, 1) == -1;
  status &= max(array, 1) == -1;
  status &= min(array, 2) == -1;
  status &= max(array, 2) == 1;
  status &= min(array, 3) == -2;
  status &= max(array, 3) == 1;
  status &= min(array, 4) == -2;
  status &= max(array, 4) == 2;
  status &= min(array, 5) == -3;
  status &= max(array, 5) == 2;
  status &= min(array, 6) == -3;
  status &= max(array, 6) == 3;
// CHECK-NOT: custom error
  if (!status)
    __builtin_tvm_throwif(1, 13);

  return 0;
}
