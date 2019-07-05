// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
struct structData
{
    long value1;
    long value2;
    long value3;
    long arr[3];
};
union setData
{
    struct structData dataSt;
    long dataArr[6];
};

union setData gun[5] = {
    {1, 2, 3, 4, 5, 6},
    {7, 8, 9, 0, 1, 2},
    {3, 4, 5, 6, 7, 8},
    {9, 0, 1, 2, 3, 4},
    {5, 6, 7, 8, 9, 0}
};

long sum_arr(long item[6]) {
    long res = 0;
    for (long i = 0; i < 6; i++)
        res += item[i];
    return res;
}

long sum_struct(struct structData item) {
    long res = item.value1 + item.value2 + item.value3;
    for (long i = 0; i < 3; i++)
        res += item.arr[i];
    return res;
}

long eq_arr(long item1[6], long item2[6]) {
    for (long i = 0; i < 6; i++)
        if(item1[i] != item2[i])
            return -1;
    return 0;
}

long eq_struct(struct structData item1, struct structData item2) {
    if (item1.value1 != item2.value1 ||
        item1.value2 != item2.value2 ||
        item1.value3 != item2.value3) 
        return -1;
    for (long i = 0; i < 3; i++)
        if(item1.arr[i] != item2.arr[i])
            return -1;
    return 0;
}

void swap(struct structData item1, long item2[6]) {
    struct structData tmp = item1;
    
    item1.value1 = item2[0];
    item1.value2 = item2[1];
    item1.value3 = item2[2];
    for (long i = 0; i < 3; i++) item1.arr[i] = item2[i+3];
    
    item2[0] = tmp.value1;
    item2[1] = tmp.value2;;
    item2[2] = tmp.value3;;
    for (long i = 0; i < 3; i++) item2[i+3] = tmp.arr[i];
}

void bubbleSort(union setData arr[], long size)
{
    long i, j;
    for(i = 0; i < size - 1; ++i)
    {            
        for(j = 0; j < size - 1; ++j)
            if (sum_arr(arr[j + 1].dataArr) < sum_struct(arr[j].dataSt)) 
                swap(arr[j + 1].dataSt, arr[j].dataArr);
    }
}

long test() {
    union setData un[5] = {
        {1, 2, 3, 4, 5, 6},
        {7, 8, 9, 0, 1, 2},
        {3, 4, 5, 6, 7, 8},
        {9, 0, 1, 2, 3, 4},
        {5, 6, 7, 8, 9, 0}
    };
    bubbleSort( un, 5);
    bubbleSort(gun, 5);
    for(long i = 0; i < 5; i++)
        if((i > 0 && sum_arr(un[i].dataArr) < sum_struct(gun[i-1].dataSt)) || 
            eq_arr(un[i].dataArr, gun[i].dataArr) != 0 ||
            eq_struct(un[i].dataSt, gun[i].dataSt) != 0) {
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