#include "smart-contract-info.h"

SmartContractInfo get_SmartContractInfo () {
    tonstdlib_get_smart_contract_info ();
    return Deserialize_SmartContractInfo ();
}

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "smart-contract-info.inc"
