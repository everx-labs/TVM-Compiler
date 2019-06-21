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

// Target amount of money: piggy bank ensures that the contract
// owner accumulates this amount before he/she can spend the 
// money.
//
// The value is kept in persistent data storage: the values 
// that have "_persistent" suffix will be saved between contract 
// invocations
int target_persistent;

// Function with "constructor" name is invoked by the standard
// library at the moment of the contract initialization.
// The additional data (target, owner) are to be passed to the
// contract in the initialization message.
void constructor () {
    target_persistent = DESERIALIZE_ARGUMENT_UNSIGNED (256);
}

// Main piggybank method: withdraw money from the piggybank
// account. May be activated if the account accumulated 
// "target" amount of money. If not, the method throws
// NOT_ENOUGH_MONEY exception.
void transfer () {
    MsgAddressInt destination = DESERIALIZE_ARGUMENT_COMPLEX (MsgAddressInt);

    // The current balance is kept in balance_remaining field of
    // the SmartContractInfo structure.
    SmartContractInfo sc_info = get_SmartContractInfo();
    int balance = sc_info.balance_remaining.grams.amount;

    // Check that we have collected enough money to transfer them.
    // If the assertion fails, the contract stops working and
    // nothing happens.
    tvm_assert (balance >= target_persistent, NOT_ENOUGH_MONEY);

    // Ok, great, we collected enough money and can retrieve
    // them! In this case, send the message to the contract to
    // transfer the money to the destination address.

    // At first, let us build the message -- emtpy message with no
    // payload, just destination address; all money are transferred
    // to that address.
    build_internal_message (destination, balance);

    // And now we are sending it
    tonstdlib_send_work_slice_as_rawmsg (0);
}

#ifdef DEBUG_EMULATION
#include <stdio.h>
#include <memory.h>
extern void set_sc_info ();
extern void print_buffer ();

int main () {
    MsgAddressInt adr;
    memset (&adr, 0, sizeof(adr));
    adr.anycast.just = 0;
    adr.workchain_id = -1;
    adr.address = 0x87654321;

    tonstdlib_create_empty_work_slice ();
    Serialize_MsgAddressInt (&adr);
    printf ("Address: "); print_buffer();

    tonstdlib_create_empty_work_slice ();
    tonstdlib_work_slice_store_int (11, 256);
    constructor();
    printf ("Target: %d\n", target_persistent);

    SmartContractInfo sc_info; 
    memset (&sc_info, 0, sizeof(sc_info));
    sc_info.balance_remaining.grams.amount = 1234;
    sc_info.myself = adr;
    printf ("Smart contract info\n");
    tonstdlib_create_empty_work_slice ();
    Serialize_SmartContractInfo (&sc_info);
    set_sc_info ();

    SmartContractInfo sc_info1 = get_SmartContractInfo();
    printf ("Deserialized: %d\n", sc_info1.balance_remaining.grams.amount);
    fflush (stdout);

    tonstdlib_create_empty_work_slice ();
    Serialize_MsgAddressInt (&adr);

    printf ("Transfer\n");

    transfer ();
}
#endif
