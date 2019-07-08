#include "messages.h"

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "messages.inc"

MsgAddressInt build_msg_address_int (int workchain, unsigned account) {
    MsgAddressInt addr;

    addr.anycast = 0;
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

    msg.info.zero = 0;
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

void send_raw_message (int flags) {
    tonstdlib_send_work_slice_as_rawmsg (flags);
}
