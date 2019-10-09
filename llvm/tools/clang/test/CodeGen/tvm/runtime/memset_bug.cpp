// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -o - | tvm-testrun --no-trace --entry main --stdlibc-path %S/../../../../../../../stdlib/stdlib_c.tvm | FileCheck %s

#include <stddef.h>

extern "C" {
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
}

// CHECK: TVM terminated with exit code 0
// CHECK: Contract persistent data updated
// CHECK-NOT: Unhandled exception

int main() {
    char x[1024];
    memset(x, -1, 1024);
    x[1023] = -1;
    for (unsigned i = 0; i < 1024; ++i) {
        __builtin_tvm_throwif(x[i] + 1 != 0, 11);
    }
    return 0;
}

