// XFAIL: *
// #define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,7,4,3,0,716,74,2,5,9,23,24};

void qSortI(long a[], long size) {
 
    long i, j;
    long lb, ub;
 
    long lbstack[50], ubstack[50];
    long stackpos = 1;
    long ppos;
    long pivot;
    long temp;
 
    lbstack[1] = 0;
    ubstack[1] = size-1;
 
    do {
        lb = lbstack[ stackpos ];
        ub = ubstack[ stackpos ];
        stackpos--;
 
        do {
            ppos = ( lb + ub ) >> 1;
            i = lb; j = ub; pivot = a[ppos];
            do {
                while ( a[i] < pivot ) i++;
                while ( pivot < a[j] ) j--;
                if ( i <= j ) {
                    temp = a[i]; a[i] = a[j]; a[j] = temp;
                    i++; j--;
                }
            } while ( i <= j );
 
            if ( i < ppos ) {
                if ( i < ub ) {
                    stackpos++;
                    lbstack[ stackpos ] = i;
                    ubstack[ stackpos ] = ub;
                }
            ub = j;
            } else {
                if ( j > lb ) {
                    stackpos++;
                    lbstack[ stackpos ] = lb;
                    ubstack[ stackpos ] = j;
                }
                lb = i;
            }
        } while ( lb < ub );
    } while ( stackpos != 0 );
}
long test() {
    long a[] = {1,7,4,3,0,716,74,2,5,9,23,24};
    qSortI( a, 12);
    qSortI(ag, 12);
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