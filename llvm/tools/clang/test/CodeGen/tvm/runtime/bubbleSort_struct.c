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

long sum(struct structData item) {
    long res = item.value1 + item.value2 + item.value3;
    for (long i = 0; i < 3; i++)
        res += item.arr[i];
    return res;
}

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

void bubbleSort_struct(struct structData arr[], long size)
{
    long i, j;
    for(i = 0; i < size - 1; ++i)
    {            
        for(j = 0; j < size - 1; ++j)
            if (sum(arr[j + 1]) < sum(arr[j])) 
                swap(&arr[j + 1], &arr[j]);
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
    bubbleSort_struct( a, 5);
    bubbleSort_struct(ag, 5);
    for(long i = 0; i < 5; i++)
        if((i > 0 && sum(a[i]) < sum(a[i-1])) || eq(a[i], ag[i])!=0) {
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