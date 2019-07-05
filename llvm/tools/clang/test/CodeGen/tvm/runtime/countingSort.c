// XFAIL: *
//#define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h

#define RANGE 255
long ag[] = {12,3,2,4,9,8,7,5,6,11,10,1};

void countSort(long arr[], long length) 
{ 
    long output[length]; 
  
    long count[RANGE + 1], i; 
    for(i = 0; i <= RANGE; i++) 
		count[i]=0;
  
    for(i = 0; arr[i]; ++i) 
        ++count[arr[i]]; 
  
    for (i = 1; i <= RANGE; ++i) 
        count[i] += count[i-1]; 
  
    for (i = 0; arr[i]; ++i) 
    { 
        output[count[arr[i]]-1] = arr[i]; 
        --count[arr[i]]; 
    } 
  
    for (i = 0; arr[i]; ++i) 
        arr[i] = output[i]; 
} 

long test() {
    long a[] = {12,3,2,4,9,8,7,5,6,11,10,1};
    countSort( a, 12);
    countSort(ag, 12);
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