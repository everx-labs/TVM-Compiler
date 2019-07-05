#include "ton-sdk/tvm.h"
#include "ton-sdk/messages.h"
#include "ton-sdk/constructor.h"
#include "ton-sdk/arguments.h"
#include "ton-sdk/smart-contract-info.h"

// Custom exception declaration
// This exception is thrown when the piggybank is required to
// transfer out some money, despite that piggybank did not
// accumulate "target" amount of money yet.
TVM_CUSTOM_EXCEPTION (NOT_ENOUGH_MONEY, 61);

// Simplest test for money transfer.
// Transfer 0xAAAA nanograms to contract 0x80000001.

void produce_output () {
    MsgAddressInt dest;
    dest.one = 1;
    dest.zero = 0;

    dest.anycast = 0;
    dest.workchain_id = 0;
    dest.address = 0x80000001;

    build_internal_message (&dest, 0xAAAA);

    send_raw_message (0);
}
