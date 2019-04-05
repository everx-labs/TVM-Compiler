#include "tvm_std.h"

int64_t allowance(int64_t tokenOwner, int64_t spender) {
  Pair p = tvm_dictuget(1, __builtin_tvm_get_persistent_data(), 256);
  int64_t dict;
  if (p.second) {
    dict = __builtin_tvm_newdict();
  } else {
    Pair refrtos = tvm_ldrefrtos(p.first);
    dict = refrtos.first;
  }
  Pair spenderAmountStatus = tvm_dictuget(spender, dict, 256);
  int64_t resultSlice;
  if (spenderAmountStatus.second) {
    resultSlice = __builtin_tvm_inttoslice(0);
  } else {
    resultSlice = spenderAmountStatus.first;
  }
  Pair resultStatus = tvm_ldu(resultSlice);
  return resultStatus.first;
}
