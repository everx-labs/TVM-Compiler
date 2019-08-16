// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %s

int global1;
int global2;
int global3;
int global4= -4;
int global5= 5;
int global6_persistent = -7;
int global7_persistent;
int global8[5];
int global9[] = {-3,1,4,1,-5,9,-2,6};

int result_persistent = 0;

__attribute__((__noinline__))
int test_Impl () {
    result_persistent = global1 + global2 + global3 + global4 +
        global5 + global6_persistent + global7_persistent;
    for (int i = 0; i < 4; i++)
        result_persistent += global8[i];
    for (int i = 0; i < 8; i++)
        result_persistent += global9[i];
        
    global5++;
    global6_persistent++;
    
    return result_persistent;
}

long test() {
    if(test_Impl() != 0x2f - 2*(11+5+3+2)) {
#ifndef NATIVE
        // CHECK-NOT: custom error
        __builtin_tvm_throw(13);
#endif
        return -1;
    }

    return 0;
}
