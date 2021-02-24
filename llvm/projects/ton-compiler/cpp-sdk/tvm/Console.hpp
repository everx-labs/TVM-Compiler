#pragma once

#include <tvm/contract_handle.hpp>

namespace tvm { namespace schema {

__interface IConsole {
  [[internal, answer_id]]
  bool_t print(string message);

  [[internal, answer_id]]
  bool_t printf(string message, cell params);

  [[internal, answer_id]]
  string inputStr(string message);
  [[internal, answer_id]]
  address inputAddress(string message, string key_hint);
  [[internal, answer_id]]
  uint256 inputUint256(string message);
  [[internal, answer_id]]
  uint256 inputPubkey(string message);
  [[internal, answer_id]]
  uint256 inputTONs(string message);
  [[internal, answer_id]]
  bool_t inputYesOrNo(string message);
  [[internal, answer_id]]
  uint32 inputDateTime(string message);
  [[internal, answer_id]]
  cell inputDeployMessage(string message);
  [[internal, answer_id]]
  cell inputCell(string message);
  [[internal, answer_id]]
  cell inputCode(string message);

  [[internal, answer_id]]
  int8 getAccountType(address addr);

  [[internal, answer_id]]
  bool_t iAmHome();
};

using IConsolePtr = handle<IConsole>;

}} // namespace tvm::schema

