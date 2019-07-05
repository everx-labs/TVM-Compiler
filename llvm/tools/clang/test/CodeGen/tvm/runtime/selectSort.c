// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

void selectSort(long* arr, long size) 
{
    long tmp, i, j, pos;
    for(i = 0; i < size; ++i)
    { 
        pos = i; 
        tmp = arr[i];
        for(j = i + 1; j < size; ++j)
        {
            if (arr[j] < tmp) 
            {
               pos = j; 
               tmp = arr[j]; 
            }
        }
        arr[pos] = arr[i]; 
        arr[i] = tmp;
    }
}

long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    selectSort( a, 12);
    selectSort(ag, 12);
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