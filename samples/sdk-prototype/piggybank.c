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
void constructor (Slice arguments) {
    target_persistent = DESERIALIZE_SLICE_UNSIGNED (arguments, 256);
}

// Main piggybank method: withdraw money from the piggybank
// account. May be activated if the account accumulated 
// "target" amount of money. If not, the method throws
// NOT_ENOUGH_MONEY exception.
void transfer (Slice arguments) {
    MsgAddressInt destination = DESERIALIZE_SLICE_COMPLEX (arguments, MsgAddressInt);

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
    Cell x = build_internal_message (destination, balance);

    // And now we are sending it
    send_raw_message (x, 0);
}
