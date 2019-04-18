// Compile with -O1 flag and uncomment macro EXECUTE_ON_TVM
// to compile & run on TVM emulator

//#define EXECUTE_ON_TVM

long long deg (long long power) {
    if (power > 0)
        return 2 * deg (power-1);
    else
        return 1;
}

long long do_deg () {
    return deg (23);
}

#ifndef EXECUTE_ON_TVM
#include <stdio.h>

int main () {
    printf ("deg(%Ld) = %Ld\n", 23ll, deg(23));
}

#endif
