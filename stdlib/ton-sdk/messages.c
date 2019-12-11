#include "messages.h"

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "datatype.inc"
#include "address.inc"
#include "messages.inc"
#undef HEADER_OR_C

MsgAddressInt build_msg_address_int (int workchain, unsigned account) {
    MsgAddressInt addr;

    addr.anycast = 0;
    addr.workchain_id = workchain;
    addr.address = account;

    return addr;
}

void build_internal_message (MsgAddressInt* dest, unsigned value) {
  build_internal_message_bounce(dest, value, 0);
}

void build_internal_message_bounce(MsgAddressInt *dest, unsigned value,
                                   unsigned bounce) {
  Grams val_grams;

  val_grams.amount.len = tonstdlib_ubytesize(value);
  val_grams.amount.value = value;

  CurrencyCollection val;
  val.grams = val_grams;
  val.other = 0;

  Grams zero_grams;
  zero_grams.amount.len = 0;
  zero_grams.amount.value = 0;
  EmptyMessage msg;

  msg.info.ihr_disabled = 0;
  msg.info.bounce = bounce;
  msg.info.bounced = 0;
  msg.info.src = *dest; // will be rewritten by Node
  msg.info.dst = *dest;
  msg.info.value = val;
  msg.info.ihr_fee = zero_grams;
  msg.info.fwd_fee = zero_grams;
  msg.info.created_lt = 0; // will be rewritten by Node
  msg.info.created_at = 0; // will be rewritten by Node
  msg.init = 0;
  msg.info.amount = 0;

  Serialize_EmptyMessage(&msg);
}

void build_external_output_int256_message (int value) {
    Message_ExtOut_int256 msg;
    msg.info.one_one = 3;

    msg.info.src.anycast = 0;
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

    msg.info.src.anycast = 0;
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

CommonMsgInfo Deserialize_CommonMsgInfo_Impl(__tvm_slice* slice) {
  CommonMsgInfo value;
  unsigned unused;
  *slice = __tvm_ldu(*slice, 1, &unused);
  *slice = __tvm_ldu(*slice, 1, &value.ihr_disabled);
  *slice = __tvm_ldu(*slice, 1, &value.bounce);
  *slice = __tvm_ldu(*slice, 1, &value.bounced);
  value.src = Deserialize_MsgAddressInt_Impl(slice);
  value.dst = Deserialize_MsgAddressInt_Impl(slice);
  value.value = Deserialize_CurrencyCollection_Impl(slice);
  value.ihr_fee = Deserialize_Grams_Impl(slice);
  value.fwd_fee = Deserialize_Grams_Impl(slice);
  *slice = __tvm_ldu(*slice, 64, &value.created_lt);
  *slice = __tvm_ldu(*slice, 32, &value.created_at);
  *slice = __tvm_ldu(*slice, 16, &value.amount);
  __builtin_tvm_setglobal(7, value.src.workchain_id);
  __builtin_tvm_setglobal(8, value.src.address);
  return value;
}

void Serialize_CommonMsgInfo_Impl(__tvm_builder *builder, CommonMsgInfo* value) {
  *builder = __builtin_tvm_stu(0, *builder, 1);
  *builder = __builtin_tvm_stu(value->ihr_disabled, *builder, 1);
  *builder = __builtin_tvm_stu(value->bounce, *builder, 1);
  *builder = __builtin_tvm_stu(value->bounced, *builder, 1);
  Serialize_MsgAddressInt_Impl(builder, &value->src);
  Serialize_MsgAddressInt_Impl(builder, &value->dst);
  Serialize_CurrencyCollection_Impl(builder, &value->value);
  Serialize_Grams_Impl(builder, &value->ihr_fee);
  Serialize_Grams_Impl(builder, &value->fwd_fee);
  *builder = __builtin_tvm_stu(value->created_lt, *builder, 64);
  *builder = __builtin_tvm_stu(value->created_at, *builder, 32);
  *builder = __builtin_tvm_stu(value->amount, *builder, 16);
}

unsigned sender_workchain_id() {
  return __builtin_tvm_getglobal(7);
}

unsigned sender_address() {
  return __builtin_tvm_getglobal(8);
}
