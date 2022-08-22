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
  build_internal_message_bounce(dest, value, 0);
}

__tvm_builder build_currency_collection(__tvm_builder builder,
                                        unsigned value) {
  //TODO: enable stu-based serialization once store optimization is implemented.
  /*
  unsigned val_size_in_bits = (__builtin_tvm_ubitsize(value) + 7) & -8;
  unsigned val_size_in_bytes = val_size_in_bits / 8;
  builder = __builtin_tvm_stu(val_size_in_bytes, builder, 4);
  builder = __builtin_tvm_stu(value, builder, val_size_in_bits);
  */
  builder = __builtin_tvm_stvaruint16(builder, value);
  builder = __builtin_tvm_stu(0, builder, 1);
  return builder;
}

void build_internal_message_bounce(MsgAddressInt *dest, unsigned value,
                                   unsigned bounce) {
  __tvm_builder builder = __builtin_tvm_newc();
  // Serialize: leading 0, ihr_disabled
  builder = __builtin_tvm_stu(0b00, builder, 2);
  builder = __builtin_tvm_stu(bounce, builder, 1);
  // Serialize: bounced, src addr = addr_none
  builder = __builtin_tvm_stu(0b000, builder, 3);
  Serialize_MsgAddressInt_Impl(&builder, dest);
  builder = build_currency_collection(builder, value);
  // Serialize: ihr_fee, fwd_fee, created_lt, created_at, init, body
  //            4 bits,  4 bits,  64 bits,    32 bits,    1 bit 1 bit
  builder = __builtin_tvm_stu(0, builder, 4 + 4 + 64 + 32 + 1 + 1);
  __builtin_tvm_setglobal(6, __builtin_tvm_cast_from_builder(builder));
}

void build_external_output_common_message_header () {
  __tvm_builder builder = __builtin_tvm_newc();
  // Serialize: leading 0b11, src addr = addr_none, dst addr = addr_none
  builder = __builtin_tvm_stu(0b110000, builder, 6);
  // Serialize: created_lt, created_at, init, body
  //            64 bits,    32 bits,    1 bit 1 bit
  builder = __builtin_tvm_stu(0, builder, 64 + 32 + 1 + 1);
  __builtin_tvm_setglobal(6, __builtin_tvm_cast_from_builder(builder));
}

void send_raw_message (int flags) {
    tonstdlib_send_work_slice_as_rawmsg (flags);
}

MsgAddressInt get_sender_address() {
    __tvm_cell c = __builtin_tvm_cast_to_cell(__builtin_tvm_getglobal(7));
    __tvm_slice s = __builtin_tvm_ctos(c);
    int unused;
    s = __tvm_ldu(s, 4, &unused);
    return Deserialize_MsgAddressInt_Impl(&s);
}
