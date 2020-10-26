#pragma once

#include "MultiBallot.hpp"

namespace tvm { namespace schema {

struct Proposal {
  address root;
  // ...
};

struct GasPriceInfo {
  uint64 flat_gas_limit;
  uint64 flat_gas_price;
};

__interface ISuperRoot {

  __attribute__((external,dyn_chain_parse))
  void constructor(cell proposalRootCode, cell multiBallotCode) = 1;

  // Adds new proposal to m_proposals, prepares initial data for new Proposal Root contract,
  //  calculates it address and deploys it
  __attribute__((internal, noaccept, dyn_chain_parse))
  bool_t createProposal(uint256 id, address depool, VotesType totalVotes,
                        uint32 startime, uint32 endtime, bytes desc,
                        bool_t superMajority, DepositType votePrice,
                        bool_t finalMsgEnabled, cell finalMsg, uint256 finalMsgValue,
                        bool_t whiteListEnabled, dict_array<uint256> whitePubkeys) = 2;

  // Create MultiBallot for user, to vote for proposals
  __attribute__((internal, noaccept, dyn_chain_parse))
  address createMultiBallot(uint256 pubkey, address depool, DepositType tonsToBallot) = 3;

  // ============== getters ==============
  __attribute__((getter, dyn_chain_parse))
  address getMultiBallotAddress(uint256 pubkey, address depool) = 4;

  __attribute__((getter))
  Proposal getProposalById(uint256 id) = 5;

  __attribute__((getter))
  dict_array<uint256> getProposalIds() = 6;

  __attribute__((getter))
  address getProposalAddress(uint256 id) = 7;

  __attribute__((getter, no_persistent))
  DepositType getCreateMultiBallotGasPrice() = 8;
};

struct DSuperRoot {
  cell proposal_root_code_;
  cell multi_ballot_code_;
  int8 workchain_id_;
  DepositType start_balance_;
  dict_map<uint256, Proposal> proposals_;
};

struct ESuperRoot {};

}} // namespace tvm::schema

