#include "smart-contract-info.h"

SmartContractInfo get_SmartContractInfo () {
    return Deserialize_SmartContractInfo (ton_smart_contract_info);
}

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "smart-contract-info.inc"
