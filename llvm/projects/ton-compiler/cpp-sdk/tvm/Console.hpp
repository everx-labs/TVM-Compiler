#pragma once

#include <tvm/contract_handle.hpp>

namespace tvm {

__interface IConsole {
  [[internal, answer_id]]
  schema::bool_t print(string message);

  [[internal, answer_id]]
  schema::bool_t printf(string message, cell params);

  [[internal, answer_id]]
  string inputStr(string message);
  [[internal, answer_id]]
  address inputAddress(string message, string key_hint);
  [[internal, answer_id]]
  schema::uint256 inputUint256(string message);
  [[internal, answer_id]]
  schema::uint256 inputPubkey(string message);
  [[internal, answer_id]]
  schema::uint256 inputTONs(string message);
  [[internal, answer_id]]
  schema::bool_t inputYesOrNo(string message);
  [[internal, answer_id]]
  schema::uint32 inputDateTime(string message);
  [[internal, answer_id]]
  cell inputDeployMessage(string message);
  [[internal, answer_id]]
  cell inputCell(string message);
  [[internal, answer_id]]
  schema::bool_t iAmHome();
};

using IConsolePtr = handle<IConsole>;

} // namespace tvm

