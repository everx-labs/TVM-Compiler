#ifndef TON_SDK_MESSAGES_H
#define TON_SDK_MESSAGES_H

#include "tvm.h"

#define HEADER_OR_C "define-ton-struct-header.inc"
#include "datatype.inc"

// addr_std$10 anycast:(Maybe Anycast)
//   workchain_id:int8 address:uint256 = MsgAddressInt;
// addr_var$11 anycast:(Maybe Anycast) addr_len:(## 9)
//   workchain_id:int32 address:(addr_len * Bit) = MsgAddressInt;
typedef struct MsgAddressInt {
  unsigned workchain_id;
  unsigned address;
} MsgAddressInt;

typedef struct CommonMsgInfo {
  unsigned ihr_disabled;
  unsigned bounce;
  unsigned bounced;
  MsgAddressInt src;
  MsgAddressInt dst;
  CurrencyCollection value;
  Grams ihr_fee;
  Grams fwd_fee;
  unsigned created_lt;
  unsigned created_at;
  unsigned amount;
} CommonMsgInfo;

#include "messages.inc"
#undef HEADER_OR_C

MsgAddressInt Deserialize_MsgAddressInt_Impl();
void Serialize_MsgAddressInt_Impl(MsgAddressInt* value);

CommonMsgInfo Deserialize_CommonMsgInfo_Impl();
void Serialize_CommonMsgInfo_Impl(CommonMsgInfo* value);

MsgAddressInt build_msg_address_int (int workchain, unsigned account);

void build_internal_message (MsgAddressInt* dest, unsigned value);
void send_raw_message (int flags);
void build_external_output_int256_message (int value);
void build_external_output_common_message_header ();

#endif
