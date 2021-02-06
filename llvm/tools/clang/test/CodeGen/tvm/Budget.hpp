#pragma once

#include <tvm/schema/message.hpp>

namespace tvm { namespace schema {

__interface [[no_pubkey]] IBudget {

  [[internal, external, dyn_chain_parse]]
  void constructor(address SMV_root);

  [[internal, noaccept]]
  void request(uint256 proposalId, address contestAddr, uint256 requestValue);
};

struct DBudget {
  address SMV_root_;
};

struct EBudget {};

}} // namespace tvm::schema

