#ifndef TON_SDK_ADDRESS_H
#define TON_SDK_ADDRESS_H

#define HEADER_OR_C "define-ton-struct-header.inc"

// anycast_info$_ depth:(## 5) rewrite_pfx:(depth * Bit) = Anycast;
typedef struct Anycast {
  unsigned depth;
  unsigned rewrite_pfx;
} Anycast;

MsgAddressInt build_msg_address_int (int workchain, unsigned account);

void build_internal_message (MsgAddressInt* dest, unsigned value);
void send_raw_message (int flags);
void build_external_output_int256_message (int value);
void build_external_output_common_message_header ();

#endif
