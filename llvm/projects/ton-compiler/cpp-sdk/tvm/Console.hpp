#pragma once

#include <tvm/contract_handle.hpp>

namespace tvm {

__interface IConsole {
  [[internal, answer_id]]
  schema::bool_t print(string message);

  [[internal, answer_id]]
  string requestInput(string message);
  [[internal, answer_id]]
  address requestAddress(string message);
  [[internal, answer_id]]
  schema::uint32 requestUint32(string message);
  [[internal, answer_id]]
  schema::uint256 requestUint256(string message);
  [[internal, answer_id]]
  schema::uint256 requestPubkey(string message);
  [[internal, answer_id]]
  schema::uint256 requestTONs(string message);
  [[internal, answer_id]]
  schema::bool_t requestYesOrNo(string message);
  [[internal, answer_id]]
  schema::uint32 requestDateTime(string message);
  [[internal, answer_id]]
  cell requestCell(string message);

  [[internal, answer_id]]
  schema::bool_t signAndSendExternal(cell msg);
};

using IConsolePtr = handle<IConsole>;

} // namespace tvm

