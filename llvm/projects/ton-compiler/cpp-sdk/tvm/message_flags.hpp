#pragma once

// Message flags

namespace tvm {

constexpr unsigned SEND_ALL_GAS = 128;
constexpr unsigned SEND_REST_GAS_FROM_INCOMING = 64;
constexpr unsigned DELETE_ME_IF_I_AM_EMPTY = 32;
constexpr unsigned IGNORE_ACTION_ERRORS = 2;
constexpr unsigned SENDER_WANTS_TO_PAY_FEES_SEPARATELY = 1;

constexpr unsigned DEFAULT_MSG_FLAGS = SENDER_WANTS_TO_PAY_FEES_SEPARATELY | IGNORE_ACTION_ERRORS;

// RAWRESERVE flags
// x to −x before performing any further actions
constexpr unsigned RESERVE_MINUS_VALUE = 8;
// x is increased by the original balance of the current account (before the
//  compute phase), including all extra currencies, before performing any
//  other checks and actions.
constexpr unsigned RESERVE_ADD_BALANCE = 4;
// external action does not fail if the specified amount cannot be
//  reserved; instead, all remaining balance is reserved
constexpr unsigned RESERVE_UP_TO = 2;

constexpr unsigned RESERVE_ALL_EXCEPT = 1; 

} // namespace tvm

