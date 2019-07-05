// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

void quickSortR(long *a, long N) {
    long i = 0, j = N;
    long temp, p;
 
    p = a[ N>>1 ];
 
    do {
        while ( a[i] < p ) i++;
        while ( a[j] > p ) j--;
 
        if (i <= j) {
            temp = a[i]; a[i] = a[j]; a[j] = temp;
            i++; j--;
        }
    } while ( i<=j );
 
    if ( j > 0 ) quickSortR(a, j);
    if ( N > i ) quickSortR(a+i, N-i);
}
long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    quickSortR( a, 12);
    quickSortR(ag, 12);
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