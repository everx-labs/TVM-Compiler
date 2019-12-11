#define TON_STRUCT_NAME Anycast
#define TON_STRUCT \
    FIELD_UNSIGNED(depth, 5) \
    FIELD_ARRAY(rewrite_pfx, depth, 32, Bit)
#include HEADER_OR_C

#define TON_STRUCT_NAME MsgAddressInt
#define TON_STRUCT \
    FIELD_CONSTANT_UNSIGNED(one, 1, 1) \
    FIELD_CONSTANT_UNSIGNED(zero, 0, 1) \
    FIELD_UNSIGNED(anycast, 1) \
    /*FIELD_MAYBE(anycast, Anycast)*/ \
    FIELD_SIGNED(workchain_id, 8) \
    FIELD_UNSIGNED(address, 256)
#include HEADER_OR_C

#define TON_STRUCT_NAME MsgAddressExt
#define TON_STRUCT \
    FIELD_UNSIGNED(zero_zero, 2)
#include HEADER_OR_C

