#pragma once

#include <type_traits>

// Message flags

namespace tvm {

constexpr unsigned SEND_ALL_GAS = 128;
constexpr unsigned SEND_REST_GAS_FROM_INCOMING = 64;
constexpr unsigned DELETE_ME_IF_I_AM_EMPTY = 32;
constexpr unsigned IGNORE_ACTION_ERRORS = 2;
constexpr unsigned SENDER_WANTS_TO_PAY_FEES_SEPARATELY = 1;

constexpr unsigned DEFAULT_MSG_FLAGS = 0;

// RAWRESERVE flags
enum class rawreserve_flag {
  none = 0x00,
  all_except = 0x01,
  // external action does not fail if the specified amount cannot be
  //  reserved; instead, all remaining balance is reserved
  up_to = 0x02,
  // x is increased by the original balance of the current account (before the
  //  compute phase), including all extra currencies, before performing any
  //  other checks and actions.
  add_balance = 0x04,
  // x to −x before performing any further actions
  minus_value = 0x08
};

__always_inline rawreserve_flag operator | (rawreserve_flag lhs, rawreserve_flag rhs) {
  using T = std::underlying_type_t<rawreserve_flag>;
  return static_cast<rawreserve_flag>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

__always_inline rawreserve_flag& operator |= (rawreserve_flag& lhs, rawreserve_flag rhs) {
  lhs = lhs | rhs;
  return lhs;
}

// x to −x before performing any further actions
[[deprecated]]
constexpr unsigned RESERVE_MINUS_VALUE = 8;
// x is increased by the original balance of the current account (before the
//  compute phase), including all extra currencies, before performing any
//  other checks and actions.
[[deprecated]]
constexpr unsigned RESERVE_ADD_BALANCE = 4;
// external action does not fail if the specified amount cannot be
//  reserved; instead, all remaining balance is reserved
[[deprecated]]
constexpr unsigned RESERVE_UP_TO = 2;

[[deprecated]]
constexpr unsigned RESERVE_ALL_EXCEPT = 1;

/// Modifier to send unspent evers from incoming message (SEND_REST_GAS_FROM_INCOMING flag).
/// \warning The flag doesn't work well when several messages is sent.
template<typename Contract>
class remaining_modifier {
public:
  explicit remaining_modifier(Contract& contract) : contract_(contract) {}
  void with_void() const {
    tvm_transfer(contract_.int_sender(), 0, true, SEND_REST_GAS_FROM_INCOMING | IGNORE_ACTION_ERRORS);
  }
  template<typename T>
  T operator & (T val) {
    contract_.set_int_return_flag(SEND_REST_GAS_FROM_INCOMING);
    return val;
  }
  Contract& contract_;
};

/// Modifier to send fixed amount of evers.
template<typename Contract>
class fixed_modifier {
public:
  explicit fixed_modifier(Contract& contract, uint128 evers) : contract_(contract), evers_(evers) {}
  void with_void() const {
    tvm_transfer(contract_.int_sender(), evers_.get(), true);
  }
  template<typename T>
  T operator & (T val) {
    contract_.set_int_return_value(evers_.get());
    return val;
  }
  Contract& contract_;
  uint128   evers_;
};

/// Modifier to send all contract evers except fixed amount.
template<typename Contract>
class all_except_modifier {
public:
  explicit all_except_modifier(Contract& contract, uint128 evers) : contract_(contract), evers_(evers) {}
  void with_void() const {
    tvm_rawreserve(evers_.get(), rawreserve_flag::up_to);
    tvm_transfer(contract_.int_sender(), 0, true, SEND_ALL_GAS | IGNORE_ACTION_ERRORS);
  }
  template<typename T>
  T operator & (T val) {
    tvm_rawreserve(evers_.get(), rawreserve_flag::up_to);
    contract_.set_int_return_flag(SEND_ALL_GAS);
    return val;
  }
  Contract& contract_;
  uint128   evers_;
};

} // namespace tvm

