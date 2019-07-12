#include "hello_world_reply.h"

int n_persistent;
unsigned compute_Impl(unsigned x) {
    n_persistent = x;
    return n_persistent;
}
