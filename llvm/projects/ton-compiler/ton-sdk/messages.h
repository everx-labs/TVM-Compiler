#ifndef TON_SDK_MESSAGES_H
#define TON_SDK_MESSAGES_H

#include "tvm.h"

#define HEADER_OR_C "define-ton-struct-header.inc"
#include "messages.inc"

enum SendMsgFlags {
  MSG_NO_FLAGS            = 0,
  MSG_PAY_FEES_SEPARATELY = 1,
  MSG_IGNORE_ERRORS       = 2
};

MsgAddressInt build_msg_address_int (int workchain, unsigned account);

void build_internal_message (MsgAddressInt* dest, unsigned value);
void build_internal_message_bounce(MsgAddressInt *dest, unsigned value,
                                   unsigned bounce);
void send_raw_message (int flags);
void build_external_output_int256_message (int value);
void build_external_output_common_message_header ();

MsgAddressInt get_sender_address ();

#endif
