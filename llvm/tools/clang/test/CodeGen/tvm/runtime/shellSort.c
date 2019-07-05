// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

long increment(long inc[], long size) 
{
    long p1, p2, p3, s;
    p1 = p2 = p3 = 1;
    s = -1;
    do 
    {
        if (++s % 2) 
        {
            inc[s] = 8*p1 - 6*p2 + 1;
        } 
        else 
        {
            inc[s] = 9*p1 - 9*p3 + 1;
            p2 *= 2;
            p3 *= 2;
        }
    p1 *= 2;
    } 
    while(3*inc[s] < size);  
 
    return s > 0 ? --s : 0;
}
 
 
void shellSort(long a[], long size) 
{
    long inc, i, j, seq[40];
    long s;
 
    s = increment(seq, size);
    while (s >= 0)
    {
         inc = seq[s--];         
         for (i = inc; i < size; ++i) 
         {
             long temp = a[i];
             for (j = i; (j >= inc) && (temp < a[j-inc]); j -= inc) {
                a[j] = a[j - inc];
             }
             a[j] = temp;
         }
    }
}

long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    shellSort( a, 12);
    shellSort(ag, 12);
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