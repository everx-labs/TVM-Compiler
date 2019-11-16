#define TON_STRUCT_NAME CommonMsgInfo
#define TON_STRUCT \
    FIELD_UNSIGNED(zero, 1) \
    FIELD_UNSIGNED(ihr_disabled, 1) \
    FIELD_UNSIGNED(bounce, 1) \
    FIELD_UNSIGNED(bounced, 1) \
    FIELD_COMPLEX(src, MsgAddressInt) \
    FIELD_COMPLEX(dst, MsgAddressInt) \
    FIELD_COMPLEX(value, CurrencyCollection) \
    FIELD_COMPLEX(ihr_fee, Grams) \
    FIELD_COMPLEX(fwd_fee, Grams) \
    FIELD_UNSIGNED(created_lt, 64) \
    FIELD_UNSIGNED(created_at, 32) \
    FIELD_UNSIGNED(amount, 16)
#include HEADER_OR_C

#define TON_STRUCT_NAME CommonMsgInfo_ExtOut
#define TON_STRUCT \
    FIELD_UNSIGNED(one_one, 2) \
    FIELD_COMPLEX(src, MsgAddressInt) \
    FIELD_COMPLEX(dst, MsgAddressExt) \
    FIELD_UNSIGNED(created_lt, 64) \
    FIELD_UNSIGNED(created_at, 32)
#include HEADER_OR_C

#define TON_STRUCT_NAME EmptyMessage
#define TON_STRUCT \
    FIELD_COMPLEX(info, CommonMsgInfo) \
    FIELD_UNSIGNED(init, 1)
#include HEADER_OR_C

#define TON_STRUCT_NAME Message_ExtOut_int256
#define TON_STRUCT \
    FIELD_COMPLEX(info, CommonMsgInfo_ExtOut) \
    FIELD_UNSIGNED(init, 1) \
    FIELD_UNSIGNED(body, 1) \
    FIELD_SIGNED(value, 256)
#include HEADER_OR_C

#define TON_STRUCT_NAME Message_ExtOut_common
#define TON_STRUCT \
    FIELD_COMPLEX(info, CommonMsgInfo_ExtOut) \
    FIELD_UNSIGNED(init, 1) \
    FIELD_UNSIGNED(body, 1) 
#include HEADER_OR_C
