#include "tvm_std.h"

#define SIGNATURE_LEN 512
typedef uint8_t Signature[SIGNATURE_LEN];
#define PUBLIC_KEY_LEN 256
typedef uint8_t PubKey[PUBLIC_KEY_LEN];
typedef void* Anycast;

typedef struct _IntAddress {
    Anycast anycast;
    uint8_t workchain_id;
    uint8_t address[256];
} IntAddress;

typedef struct _ExtAddress {
    uint8_t len;
    uint8_t buf[256];
} ExtAddress;

typedef struct _Slice {
} Slice;

typedef struct _Message {
    Slice body;
} Message;


Slice new_slice();
void slice_write_u64(Slice* slice, int64_t value);

void send_int_msg(IntAddress to, int64_t value, Slice body);
void send_ex_msg(Slice body);


Message tvm_getmsg();

