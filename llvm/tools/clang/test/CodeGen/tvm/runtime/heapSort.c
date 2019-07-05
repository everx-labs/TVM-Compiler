// XFAIL: *
//#define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {12,3,2,4,9,8,7,5,6,11,10,1};

void downHeap(long a[], long k, long n) 
{
    long new_elem;
    long child;
    new_elem = a[k];
    
    while(k <= n/2)
    {       
        child = 2*k;
        
        if( child < n && a[child] < a[child+1] )
            child++;
        if( new_elem >= a[child] ) 
            break; 
        a[k] = a[child];
        k = child;
    }
    a[k] = new_elem;
}
 
void heapSort(long a[], long size) 
{
    long i;
    long temp;
 
    for(i = size / 2 - 1; i >= 0; --i) 
        downHeap(a, i, size-1);
  
    for(i=size-1; i > 0; --i) 
    {
        temp = a[i]; 
        a[i] = a[0]; 
        a[0] = temp;
        downHeap(a, 0, i-1); 
    }
}

long test() {
    long a[] = {12,3,2,4,9,8,7,5,6,11,10,1};
    heapSort(a,12);
    heapSort(ag,12);
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