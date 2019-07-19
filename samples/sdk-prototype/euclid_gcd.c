#include "euclid_gcd.h"
#include "ton-sdk/tvm.h"
#include "ton-sdk/messages.h"
#include "ton-sdk/constructor.h"
#include "ton-sdk/arguments.h"
#include "ton-sdk/smart-contract-info.h"

volatile unsigned a_persistent = 0;
volatile unsigned b_persistent = 0;

void order () {
    if (a_persistent < b_persistent) {
        unsigned temp = a_persistent;
        a_persistent = b_persistent;
        b_persistent = temp;
    }
}

unsigned init_Impl (unsigned a, unsigned b) {
    a_persistent = a;
    b_persistent = b;
    order ();

    return a_persistent;
}

unsigned step_Impl () {
    a_persistent -= b_persistent;
    order ();

    return a_persistent;
}
