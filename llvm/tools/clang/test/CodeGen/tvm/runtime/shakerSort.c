// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

void shaker_sort(long array[], long size)
{
    for (long left_idx = 0, right_idx = size - 1;
         left_idx < right_idx;)
    {
        for (long idx = left_idx; idx < right_idx; idx++)
        {
            if (array[idx + 1] < array[idx])
            {
                long tmp = array[idx];
                array[idx] = array[idx + 1];
                array[idx + 1] = tmp;
            }
        }
        right_idx--;

        for (long idx = right_idx; idx > left_idx; idx--)
        {
            if (array[idx - 1] >  array[idx])
            {
                long tmp = array[idx - 1];
                array[idx - 1] = array[idx];
                array[idx] = tmp;
            }
        }
        left_idx++;
    }
}

long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    shaker_sort( a, 12);
    shaker_sort(ag, 12);
    for(long i = 0; i < 12; i++)
        if((i > 0 && a[i] < a[i-1]) || a[i] != ag[i]) {
#ifndef NATIVE
            // CHECK-NOT: custom error
            __builtin_tvm_throw(13);
#endif
            return -1;
        }

    return 0;
}
#ifdef NATIVE
#include <stdio.h>

int main () {
    long res = test();
    printf("Finishing with exit code %d\n", res);
    return (int)res;
}
#endif