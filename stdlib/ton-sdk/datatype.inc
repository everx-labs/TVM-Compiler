#define TON_STRUCT_NAME Grams
#define TON_STRUCT \
    FIELD_VAR_UNSIGNED(amount, 4)
#include HEADER_OR_C

#define TON_STRUCT_NAME CurrencyCollection
#define TON_STRUCT \
    FIELD_COMPLEX(grams, Grams) \
    FIELD_UNSIGNED(other, 1)
 /* FIELD_COMPLEX(other, ExtraCurrencyCollection) */
#include HEADER_OR_C

#define TON_STRUCT_NAME Bit
#define TON_STRUCT \
    FIELD_UNSIGNED(bit, 1)
#include HEADER_OR_C
