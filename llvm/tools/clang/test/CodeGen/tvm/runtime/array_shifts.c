// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h

struct dataStruct
{
    long length;
    long data[3];
};

struct dataStruct ag[] = {
    {3, {1, 2, 3}},
    {3, {4, 5, 6}},
    {3, {7, 8, 9}}
};

long cmp(struct dataStruct lh, struct dataStruct rh) {
    if(lh.length != rh.length) 
        return -1;
    long i;
    for(i = 0; i < lh.length; i++) 
        if(lh.data[i] != rh.data[i]) 
            return -1;
    return 0;
}

void lshift(struct dataStruct arr[], long len) {
    if(len == 1) 
        return;
    struct dataStruct tmp = arr[0];
    for(long i = 1; i < len; i++)
        arr[i-1] = arr[i];
    arr[len-1] = tmp;
}

void rshift(struct dataStruct arr[], long len) {
    if(len == 1) 
        return;
    struct dataStruct tmp = arr[len-1];
    for(long i = len-1; i > 0; i--)
        arr[i] = arr[i-1];
    arr[0] = tmp;
}

long test(){
    long i;
    struct dataStruct a[] = {
        {3, {1, 2, 3}},
        {3, {4, 5, 6}},
        {3, {7, 8, 9}}
    };

    for(i = 0; i < 3; i++) {
        long j;
        lshift(ag, 3);
        for(j = 0; j < 3; j++)
            if(cmp(ag[j], a[j+i+1 > 2? j+i-2 : j+i+1]) != 0) 
#ifndef NATIVE
                // CHECK-NOT: custom error
                __builtin_tvm_throw(13);
#endif
                return 1;
    }

    for(i = 0; i < 3; i++) {
        long j;
        lshift(a, 3);
        for(j = 0; j < 3; j++)
            if(cmp(a[j], ag[j+i+1 > 2? j+i-2 : j+i+1]) != 0) 
#ifndef NATIVE
                // CHECK-NOT: custom error
                __builtin_tvm_throw(13);
#endif
                return 2;
    }
    
    for(i = 0; i < 3; i++) {
        long j;
        rshift(ag, 3);
        for(j = 0; j < 3; j++)
            if(cmp(ag[j], a[2+j-i > 2 ? j-i-1 : 2+j-i]) != 0) 
#ifndef NATIVE
                // CHECK-NOT: custom error
                __builtin_tvm_throw(13);
#endif
                return 3;
    }

    for(i = 0; i < 3; i++) {
        long j;
        rshift(a, 3);
        for(j = 0; j < 3; j++)
            if(cmp(a[j], ag[2+j-i > 2 ? j-i-1 : 2+j-i]) != 0) 
#ifndef NATIVE
                // CHECK-NOT: custom error
                __builtin_tvm_throw(13);
#endif
                return 4;
    }
    
    // everything is fine
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