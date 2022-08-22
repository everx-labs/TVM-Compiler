#pragma once

#ifdef TVM_UNTUPLE_CASE
#error 'TVM_UNTUPLE_CASE' redefine
#endif

namespace tvm {

template<unsigned N>
struct untuple_caller {
  static auto untuple(__tvm_tuple tpHandle) {}
};

#define TVM_UNTUPLE_CASE(N)                            \
        template<>                                     \
        struct untuple_caller<N> {                     \
          static auto untuple(__tvm_tuple tpHandle) {  \
            return __builtin_tvm_untuple##N(tpHandle); \
          }                                            \
        };

#include "untuple_cases.def"

#undef TVM_UNTUPLE_CASE

} // namespace tvm

