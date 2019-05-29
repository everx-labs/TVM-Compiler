#include "tvm_std.h"

#define SIGNATURE_LEN 64
typedef uint8_t Signature[SIGNATURE_LEN];
#define PUBLIC_KEY_LEN 32
typedef uint8_t PubKey[PUBLIC_KEY_LEN];
typedef void* Anycast;

typedef struct _IntAddress {
    Anycast anycast;
    uint8_t workchain_id;
    uint8_t address[32];
} IntAddress;

typedef struct _ExtAddress {
    uint8_t len;
    uint8_t buf[32];
} ExtAddress;

typedef struct _Message {
    Slice body;
} Message;

Slice sdk_serialize_key(PubKey* key);
Slice new_slice();
void slice_write_u64(Slice* slice, int64_t value);

void send_int_msg(IntAddress to, int64_t value, Slice body);
void send_ex_msg(Slice body);

Message tvm_getmsg();

