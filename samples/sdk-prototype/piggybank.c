#include "ton-sdk/tvm.h"
#include "ton-sdk/messages.h"
#include "ton-sdk/smart-contract-info.h"

// Custom exception declaration

// This exception is thrown when the acount is required to
// transfer out some money, despite that piggybank did not
// accumulate "target" amount of money yet.
TVM_CUSTOM_EXCEPTION (NOT_ENOUGH_MONEY, 61);

// Target amount initialization may be performed only once:
// otherwise one could easily evade "target" amount limitation.
TVM_CUSTOM_EXCEPTION (ALREADY_INITIALIZED, 62);

enum { MESSAGE_COST = 10000 };

// Target amount of money: piggy bank ensures that the contract
// owner accumulates this amount before he/she can spend the 
// money.
int target_persistent = MESSAGE_COST;

// Used to specify the target amount of money (in nanograms).
// The money cannot be decreased: if method initialize_limit 
// was invoked with larger value, its re-initialization with
// smaller value results in error.
void initialize_target_Impl (unsigned target) {
    tvm_assert (target_persistent > target, ALREADY_INITIALIZED);
    target_persistent = target;
}

// Main piggybank method: withdraw all money from the piggybank
// account. May be activated if the account accumulated 
// "target" amount of money. If not, the method throws
// NOT_ENOUGH_MONEY exception.
void transfer_Impl (unsigned destination_account) {
    // Check that we have collected enough money to transfer them.
    // Info about current account state (inluding current contract
    // gram balance) is kept in SmartContractInfo structure -
    // see TON blockchain docs for more info.
    SmartContractInfo sc_info = get_SmartContractInfo();
    int balance = sc_info.balance_remaining.grams.amount.value;
    tvm_assert (balance >= target_persistent, NOT_ENOUGH_MONEY);

    // Ok, great, we collected enough money and can retrieve
    // them! In this case, send the message to the contract to
    // transfer the money to the destination address.
    MsgAddressInt destination = 
        build_msg_address_int (0, destination_account);
    build_internal_message (&destination, balance - MESSAGE_COST);
    send_raw_message (0);
}
