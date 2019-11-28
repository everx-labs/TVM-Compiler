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
