#include "messages.h"

#define HEADER_OR_C "define-ton-struct-c.inc"
#include "messages.inc"

void build_internal_message (MsgAddressInt dest, unsigned value) {
    Grams val_grams;
    val_grams.amount = value;

    CurrencyCollection val;
    val.grams = val_grams;

    Grams zero_grams;
    zero_grams.amount = 0;

    EmptyMessage msg;
    msg.info.ihr_disabled = 0;
    msg.info.bounce = 0;
    msg.info.src = dest; // will be rewirtten by Node
    msg.info.dst = dest;
    msg.info.value = val;
    msg.info.ihr_fee = zero_grams;
    msg.info.fwd_fee = zero_grams;
    msg.info.created_lt = 0; // will be rewirtten by Node
    msg.info.created_at = 0; // will be rewirtten by Node
    msg.init = 0;
    Serialize_EmptyMessage (&msg);
}

void send_raw_message (int flags) {
    tonstdlib_send_work_slice_as_rawmsg (flags);
}

void send_external_int256_message (int value) {
    ;
}
