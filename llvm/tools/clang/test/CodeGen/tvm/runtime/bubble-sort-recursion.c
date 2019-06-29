// RUN: %clang -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry main | FileCheck %s

void bubble_sort_step(int* a, int n)
{
  if (n <= 1)
    return;

  if (a[0] > a[1])
  {
    int tmp = a[0];
    a[0]    = a[1];
    a[1]    = tmp;
  }

  bubble_sort_step(a+1, n-1);
}

void bubble_sort(int* a, int n, int step)
{
  if (step == n)
    return;

  bubble_sort_step(a, n);

  bubble_sort(a, n, step + 1);
}

int check_sorting_sequence(int* a, int n)
{
  if (n <= 1)
    return 1;

  if (!check_sorting_sequence(a+1, n-1))
    return 0;

  return a[0] < a[1];
}

int main()
{
  int a[] = {1,5,4,2,3};
  int n = sizeof(a) / sizeof(*a);

  bubble_sort(a, n, 0);

  int result = check_sorting_sequence(a, n);

// CHECK-NOT: custom error
//unfortunately __builtin_tvm_throwif(!result, 13) does not work
  if (!result)
    __builtin_tvm_throwif(1, 13);

  return 0;
}
