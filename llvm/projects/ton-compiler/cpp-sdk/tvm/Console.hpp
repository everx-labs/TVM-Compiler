#pragma once

#include <tvm/contract_handle.hpp>

namespace tvm { inline namespace schema {

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
  cell inputCode(string tvc_path);

  [[internal, answer_id]]
  int8 getAccountType(address addr);

  [[internal, answer_id]]
  bool_t iAmHome();

  [[internal, answer_id]]
  address requestProxy(string message);

  [[internal, answer_id]]
  bool_t setProxy(address addr, uint256 pubkey);

  [[internal, answer_id]]
  uint256 genkey();

  [[internal, answer_id]]
  uint256 loadkey(string path);
};

using IConsolePtr = handle<IConsole>;

}} // namespace tvm::schema

