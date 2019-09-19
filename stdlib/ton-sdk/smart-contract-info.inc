#define TON_STRUCT_NAME SmartContractInfo
#define TON_STRUCT \
    FIELD_CONSTANT_UNSIGNED(smc_info, 0x076ef1ea, 32) \
    FIELD_UNSIGNED(actions, 16) \
    FIELD_UNSIGNED(msgs_sent, 16) \
    FIELD_UNSIGNED(unixtime, 32) \
    FIELD_UNSIGNED(block_lt, 64) \
    FIELD_UNSIGNED(trans_lt, 64) \
    FIELD_UNSIGNED(rand_seed, 256) \
    FIELD_COMPLEX(balance_remaining, CurrencyCollection) \
    FIELD_COMPLEX(myself, MsgAddressInt)
#include HEADER_OR_C

#undef HEADER_OR_C