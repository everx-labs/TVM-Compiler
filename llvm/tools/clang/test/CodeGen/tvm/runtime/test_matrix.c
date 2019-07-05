// XFAIL: *
//#define NATIVE
// RUN: %clang -O3 -S -c -target tvm %s -o - | tvm-testrun --entry test | FileCheck %s
#define DIM 5
long ag[DIM][DIM];
long bg[DIM][DIM];
    
void mul(long arr1[DIM][DIM], long arr2[DIM][DIM], long res[DIM]) {
    for (long  r = 0; r < DIM; r++) {
        res[r]=0;
        for (long  i = 0; i < DIM; i++) {
            res[r] += arr1[r][i] * arr2[i][r];
        }
    }
    
}
void init(long arr[DIM][DIM], char reverse){
    for(long i1=0; i1<DIM; i1++) 
        for(long i2=0; i2<DIM; i2++) 
            arr[i1][i2] = (reverse==0) ? DIM * i1 + i2 : DIM*DIM - (DIM * i1 + i2) - 1;
}
void test_local(long res[DIM]) {
    long a[DIM][DIM];
    long b[DIM][DIM];
    init(a, 0);
    init(b, 1);
    mul(a, b, res);
}
void test_global(long res[DIM]) {
    init(ag, 0);
    init(bg, 1);
    mul(ag, bg, res);
}
long test() {
    long res1[DIM], res2[DIM];
	test_local(res1);
    test_global(res2);
    long res=0;
    for (long i = 0; i < DIM; i++)
        res += res1[i]-res2[i];
    return res;
}

#ifdef NATIVE
#include <stdio.h>

int main () {
    long res = test();
    printf("Finishing with exit code %d\n", res);
    return (int)res;
}
#endif