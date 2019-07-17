#include "ton-sdk/tvm.h"
#include "ton-sdk/messages.h"
#include "ton-sdk/constructor.h"
#include "ton-sdk/arguments.h"
#include "ton-sdk/smart-contract-info.h"

// Transfer statuses
// It will be returned by a contract as an output parameter
enum TransferStatus
{
  TransferStatus_Success,
  TransferStatus_NotEnoughMoney,
  TransferStatus_TxLimitExceeded,
};

// Transaction limit which is persistent between contract method calls
unsigned tx_limit_persistent;

// Function which is invoked by standard library at the moment of the contract
// initialization.
void constructor_Impl()
{
  tx_limit_persistent = 0;
}

// Set transaction limit
// Transactions with amount larger than the limit will be declined
// Zero limit allows to send any amount of money in one transaction
void set_tx_limit_Impl(unsigned limit)
{
  tx_limit_persistent = limit;
}

// Make a transfer to the destination account
unsigned transfer_Impl(unsigned destination_account, unsigned amount)
{
  MsgAddressInt destination = build_msg_address_int(0, destination_account);

  SmartContractInfo sc_info = get_SmartContractInfo();
  unsigned balance = (unsigned)sc_info.balance_remaining.grams.amount.value;

  // The current balance is kept in balance_remaining field of
  // the SmartContractInfo structure.
  // If there are no enough money, return error status code
  if (balance < amount)
    return TransferStatus_NotEnoughMoney;

  // Check if transaction amount does not exceed limit
  if (tx_limit_persistent != 0 && amount > tx_limit_persistent)
    return TransferStatus_TxLimitExceeded;

  // We have enough money and can send them to the destination address. Build
  // the empty message with no payload, just destination address; all money will
  // be transferred to this address.
  build_internal_message(&destination, amount);

  // Send message to the destination address
  tonstdlib_send_work_slice_as_rawmsg(0);

  return TransferStatus_Success;
}
