#ifndef TON_SDK_SMART_CONTRACT_INFO_H
#define TON_SDK_SMART_CONTRACT_INFO_H

#include "tvm.h"
#include "messages.h"

extern Cell ton_smart_contract_info;

#define HEADER_OR_C "define-ton-struct-header.inc"
#include "smart-contract-info.inc"

SmartContractInfo get_SmartContractInfo ();

#endif
