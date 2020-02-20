#include "smart-contract-info.h"

SmartContractInfo get_SmartContractInfo () {
    tonstdlib_get_smart_contract_info ();
    __tvm_slice slice = __builtin_tvm_cast_to_slice(__builtin_tvm_getglobal(3));
    return tvm_deserialize_SmartContractInfo (&slice);
}

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "smart-contract-info.inc"

unsigned contract_balance() {
  return __builtin_tvm_hiddenstack(4);
}

unsigned message_balance() {
  return __builtin_tvm_hiddenstack(3);
}
