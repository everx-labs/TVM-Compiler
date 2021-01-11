#pragma once

namespace tvm { namespace schema {

__interface IMultisigWallet {
  [[external]]
  void sendTransaction(address dest, uint128 value, bool_t bounce, uint8 flags, cell payload);
};

}} // namespace tvm::schema

