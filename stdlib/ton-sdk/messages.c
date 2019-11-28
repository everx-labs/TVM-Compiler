#include "messages.h"

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "datatype.inc"

MsgAddressInt Deserialize_MsgAddressInt_Impl() {
  MsgAddressInt value;
  // Address kind + anycast flag.
  unsigned selector = Deserialize_Unsigned_Impl(3);
  tvm_assert(selector == 4u, 58);
  // TODO: Deserialize 0b110 if these addresses appear.
  value.workchain_id = Deserialize_Unsigned_Impl(8);
  value.address = Deserialize_Unsigned_Impl(256);
  return value;
}

void Serialize_MsgAddressInt_Impl(MsgAddressInt* value) {
  // Address kind + anycast flag.
  Serialize_Unsigned_Impl(4u, 3);
  // TODO: Serialize 0b110 if we need it.
  Serialize_Unsigned_Impl(value->workchain_id, 8);
  Serialize_Unsigned_Impl(value->address, 256);
}

CommonMsgInfo Deserialize_CommonMsgInfo_Impl() {
  CommonMsgInfo value;
  Deserialize_Unsigned_Impl(1);
  value.ihr_disabled = Deserialize_Unsigned_Impl(1);
  value.bounce = Deserialize_Unsigned_Impl(1);
  value.bounced = Deserialize_Unsigned_Impl(1);
  value.src = Deserialize_MsgAddressInt_Impl();
  value.dst = Deserialize_MsgAddressInt_Impl();
  value.value = Deserialize_CurrencyCollection_Impl();
  value.ihr_fee = Deserialize_Grams_Impl();
  value.fwd_fee = Deserialize_Grams_Impl();
  value.created_lt = Deserialize_Unsigned_Impl(64);
  value.created_at = Deserialize_Unsigned_Impl(32);
  value.amount = Deserialize_Unsigned_Impl(16);
  __builtin_tvm_setglobal(7, value.src.workchain_id);
  __builtin_tvm_setglobal(8, value.src.address);
  return value;
}

void Serialize_CommonMsgInfo_Impl(CommonMsgInfo* info) {
  Serialize_Unsigned_Impl(0, 1);
  Serialize_Unsigned_Impl(info->ihr_disabled, 1);
  Serialize_Unsigned_Impl(info->bounce, 1);
  Serialize_Unsigned_Impl(info->bounced, 1);
  Serialize_MsgAddressInt_Impl(&info->src);
  Serialize_MsgAddressInt_Impl(&info->dst);
  Serialize_CurrencyCollection_Impl(&info->value);
  Serialize_Grams_Impl(&info->ihr_fee);
  Serialize_Grams_Impl(&info->fwd_fee);
  Serialize_Unsigned_Impl(info->created_lt, 64);
  Serialize_Unsigned_Impl(info->created_at, 32);
  Serialize_Unsigned_Impl(info->amount, 16);
}

#include "messages.inc"
#undef HEADER_OR_C

MsgAddressInt build_msg_address_int (int workchain, unsigned account) {
    MsgAddressInt addr;

    addr.workchain_id = workchain;
    addr.address = account;

    return addr;
}

void build_internal_message (MsgAddressInt* dest, unsigned value) {
    Grams val_grams;

    val_grams.amount.len = tonstdlib_log_8 (value);
    val_grams.amount.value = value;

    CurrencyCollection val;
    val.grams = val_grams;
    val.other = 0;

    Grams zero_grams;
    zero_grams.amount.len = 0;
    zero_grams.amount.value = 0;
    EmptyMessage msg;

    msg.info.ihr_disabled = 0;
    msg.info.bounce = 0;
    msg.info.bounced = 0;
    msg.info.src = *dest; // will be rewirtten by Node
    msg.info.dst = *dest;
    msg.info.value = val;
    msg.info.ihr_fee = zero_grams;
    msg.info.fwd_fee = zero_grams;
    msg.info.created_lt = 0; // will be rewirtten by Node
    msg.info.created_at = 0; // will be rewirtten by Node
    msg.init = 0;
    msg.info.amount = 0;

    Serialize_EmptyMessage (&msg);
}

void build_external_output_int256_message (int value) {
    Message_ExtOut_int256 msg;
    msg.info.one_one = 3;

    msg.info.src.workchain_id = -1;
    msg.info.src.address = 1;
    msg.info.dst.zero_zero = 0;
    msg.info.created_lt = 0;
    msg.info.created_at = 0;
    msg.init = 0;
    msg.body = 0;
    msg.value = value;

    Serialize_Message_ExtOut_int256 (&msg);
}

void build_external_output_common_message_header () {
    Message_ExtOut_common msg;
    msg.info.one_one = 3;

    msg.info.src.workchain_id = 0;
    msg.info.src.address = 1;
    msg.info.dst.zero_zero = 0;
    msg.info.created_lt = 0;
    msg.info.created_at = 0;
    msg.init = 0;
    msg.body = 0;

    Serialize_Message_ExtOut_common (&msg);
}

void send_raw_message (int flags) {
    tonstdlib_send_work_slice_as_rawmsg (flags);
}

unsigned sender_workchain_id() {
  return __builtin_tvm_getglobal(7);
}

unsigned sender_address() {
  return __builtin_tvm_getglobal(8);
}
