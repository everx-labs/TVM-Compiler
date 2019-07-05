// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

long largest(long a[], long n)
{
    long large = a[0], i;
    for(i = 1; i < n; i++)
    {
        if(large < a[i])
            large = a[i];
    }
    return large;
}
void RadixSort(long a[], long n)
{
    long bucket[10][10], bucket_count[10];
    long i, j, k, remainder, NOP=0, divisor=1, large, pass;
 
    large = largest(a, n);
    while(large > 0)
    {
        NOP++;
        large/=10;
    }
 
    for(pass = 0; pass < NOP; pass++)
    {
        for(i = 0; i < 10; i++)
        {
            bucket_count[i] = 0;
        }
        for(i = 0; i < n; i++)
        {
            remainder = (a[i] / divisor) % 10;
            bucket[remainder][bucket_count[remainder]] = a[i];
            bucket_count[remainder] += 1;
        }
 
        i = 0;
        for(k = 0; k < 10; k++)
        {
            for(j = 0; j < bucket_count[k]; j++)
            {
                a[i] = bucket[k][j];
                i++;
            }
        }
        divisor *= 10;
    }
}

long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    RadixSort( a, 12);
    RadixSort(ag, 12);
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