#pragma once

#include <tvm/contract_handle.hpp>

namespace tvm {

__interface IConsole {
  [[internal]]
  schema::bool_t print(string message);

  [[internal]]
  string requestInput(string message);
  [[internal]]
  address requestAddress(string message);
  [[internal]]
  schema::uint32 requestUint32(string message);
  [[internal]]
  schema::uint256 requestPubkey(string message);
  [[internal]]
  schema::uint256 requestTONs(string message);

  [[internal]]
  schema::bool_t signAndSendExternal(cell msg);
};

using IConsolePtr = handle<IConsole>;

} // namespace tvm

