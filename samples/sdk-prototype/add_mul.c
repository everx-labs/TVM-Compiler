#include "add_mul.h"
#include "ton-sdk/tvm.h"
#include "ton-sdk/messages.h"
#include "ton-sdk/constructor.h"
#include "ton-sdk/arguments.h"

volatile unsigned a_persistent = 0;
volatile unsigned b_persistent = 0;
volatile unsigned res_persistent = 0;

unsigned add_Impl (unsigned a, unsigned b) {
    a_persistent = a;
    b_persistent = b;
    res_persistent = a + b;

    return res_persistent;
}

unsigned mul_Impl (unsigned a, unsigned b) {
    a_persistent = a;
    b_persistent = b;
    res_persistent = a * b;

    return res_persistent;
}
