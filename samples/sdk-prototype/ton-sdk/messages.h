#ifndef TON_SDK_MESSAGES_H
#define TON_SDK_MESSAGES_H

#include "tvm.h"

#define HEADER_OR_C "define-ton-struct-header.inc"
#include "messages.inc"

Cell build_internal_message (MsgAddressInt dest, unsigned value);
void send_raw_message (Cell message, int flags);

#endif