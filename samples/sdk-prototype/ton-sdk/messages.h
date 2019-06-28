#ifndef TON_SDK_MESSAGES_H
#define TON_SDK_MESSAGES_H

#include "tvm.h"

#define HEADER_OR_C "define-ton-struct-header.inc"
#include "messages.inc"

void build_internal_message (MsgAddressInt* dest, unsigned value);
void send_raw_message (int flags);
void build_external_output_int256_message (int value);
void build_external_output_common_message ();

#endif