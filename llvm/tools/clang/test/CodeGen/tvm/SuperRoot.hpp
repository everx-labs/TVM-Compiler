#pragma once

#include "MultiBallot.hpp"
#include "Budget.hpp"
#include "SMVStats.hpp"

#include <tvm/contract_handle.hpp>

namespace tvm { inline namespace schema {

struct Proposal {
  address root;
};

__interface ISuperRoot {

  [[external, dyn_chain_parse]]
  void constructor(address budget, address stats, address depool);

  [[external, noaccept, dyn_chain_parse]]
  void setProposalRootCode(cell code);

  [[external, noaccept, dyn_chain_parse]]
  void setMultiBallotCode(cell code);

  // Adds new proposal to m_proposals, prepares initial data for new Proposal Root contract,
  //  calculates it address and deploys it
  [[internal, noaccept, dyn_chain_parse, answer_id]]
  bool_t createProposal(uint256 id, VotesType totalVotes,
                        uint32 startime, uint32 endtime, string desc,
                        bool_t superMajority, DepositType votePrice,
                        bool_t finalMsgEnabled,
                        cell finalMsg, uint256 finalMsgValue, uint256 finalMsgRequestValue,
                        bool_t whiteListEnabled, dict_array<uint256> whitePubkeys);

  // Create MultiBallot for user, to vote for proposals
  [[internal, noaccept, dyn_chain_parse, answer_id]]
  address createMultiBallot(uint256 pubkey, DepositType tonsToBallot);

  // Proposal notifies that contest is approved and funds requested
  [[internal, noaccept]]
  void contestApproved(uint256 id, address contest_addr, uint256 requestValue);

  // ============== getters ==============
  [[getter]]
  address getBudget();

  [[getter]]
  address getStats();

  [[getter]]
  address getDepool();

  [[getter]]
  address getMultiBallotAddress(uint256 pubkey);

  [[getter]]
  Proposal getProposalById(uint256 id);

  [[getter]]
  dict_array<uint256> getProposalIds();

  [[getter]]
  address getProposalAddress(uint256 id);

  [[getter]]
  bool_t isFullyInitialized();

  [[getter, no_persistent]]
  DepositType getCreateMultiBallotGasPrice();
};

struct DSuperRoot {
  handle<IBudget> budget_;
  handle<ISMVStats> stats_;
  address depool_;
  std::optional<cell> proposal_root_code_;
  std::optional<cell> multi_ballot_code_;
  int8 workchain_id_;
  DepositType start_balance_;
  dict_map<uint256, Proposal> proposals_;
  uint256 deployer_key_;
};

struct ESuperRoot {};

}} // namespace tvm::schema

