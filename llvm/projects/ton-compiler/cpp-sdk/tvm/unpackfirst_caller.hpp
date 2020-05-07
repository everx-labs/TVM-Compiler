#pragma once

#ifdef TVM_UNTUPLE_CASE
#error 'TVM_UNTUPLE_CASE' redefine
#endif

namespace tvm {

template<unsigned N>
struct unpackfirst_caller {
  static auto unpack(__tvm_tuple tpHandle) {}
};

#define TVM_UNTUPLE_CASE(N)                                \
        template<>                                         \
        struct unpackfirst_caller<N> {                     \
          static auto unpack(__tvm_tuple tpHandle) {       \
            return __builtin_tvm_unpackfirst##N(tpHandle); \
          }                                                \
        };

#include "untuple_cases.def"

#undef TVM_UNTUPLE_CASE

} // namespace tvm

