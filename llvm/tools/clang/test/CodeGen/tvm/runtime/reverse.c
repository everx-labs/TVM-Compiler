// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
struct structData
{
    long value1;
    long value2;
    long value3;
    long arr[3];
} ag[] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 0, 1, 2},
    {3, 4, 5, 6, 7, 8},
    {9, 0, 1, 2, 3, 4},
    {5, 6, 7, 8, 9, 0}
};

long eq(struct structData item1, struct structData item2) {
    if( item1.value1!=item2.value1 ||
        item1.value2!=item2.value2 ||
        item1.value3!=item2.value3) 
        return -1;
    for (long i = 0; i < 3; i++)
        if(item1.arr[i] != item2.arr[i])
            return -1;
    return 0;
}

void swap(struct structData* item1, struct structData* item2) {
    struct structData tmp = *item1;
    *item1 = *item2;
    *item2 = tmp;
}

void reverse(struct structData arr[], long size)
{
    long pos=0;
    while(size-2*pos-1>0) {
        swap(&arr[pos], &arr[size - pos - 1]);
        pos++;
    }
}
long test() {
    struct structData a[] = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 0, 1, 2},
        {3, 4, 5, 6, 7, 8},
        {9, 0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9, 0}
    };
    reverse( a, 5);
    reverse(ag, 5);
    for(long i = 0; i < 5; i++)
        if(eq(a[i], ag[i])!=0) {
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