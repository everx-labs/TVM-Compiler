#ifndef TON_SDK_MESSAGES_H
#define TON_SDK_MESSAGES_H

#include "tvm.h"

#define HEADER_OR_C "define-ton-struct-header.inc"

#include "datatype.inc"
#include "address.inc"

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

CommonMsgInfo Deserialize_CommonMsgInfo_Impl(__tvm_slice *slice);
void Serialize_CommonMsgInfo_Impl(__tvm_builder *builder, CommonMsgInfo* value);

MsgAddressInt build_msg_address_int (int workchain, unsigned account);

void build_internal_message (MsgAddressInt* dest, unsigned value);
void build_internal_message_bounce(MsgAddressInt *dest, unsigned value,
                                   unsigned bounce);
void send_raw_message (int flags);
void build_external_output_int256_message (int value);
void build_external_output_common_message_header ();

unsigned sender_workchain_id();
unsigned sender_address();

#endif
