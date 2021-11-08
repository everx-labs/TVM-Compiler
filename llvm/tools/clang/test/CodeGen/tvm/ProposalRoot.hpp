#pragma once

#include "MultiBallot.hpp"
#include <tvm/contract_handle.hpp>

namespace tvm { inline namespace schema {

struct ProposalInfo {
  uint256 id;
  uint32 start;
  uint32 end;
  string desc;
  bool_t finished;
  bool_t approved;
  bool_t resultsSent;
  bool_t earlyFinished;
  bool_t whiteListEnabled;
  VotesType totalVotes;
  VotesType currentVotes;
  VotesType yesVotes;
  VotesType noVotes;
  DepositType votePrice;
};

struct VotesInfo {
  VotesType yes;
  VotesType no;
};

struct VoteResults {
  VotesType yes;
  VotesType no;
  bool_t finished;
  bool_t approved;
};

static constexpr unsigned PROPOSAL_ROOT_TIMESTAMP_DELAY = 1800;
using proposal_root_replay_protection_t = replay_attack_protection::timestamp<PROPOSAL_ROOT_TIMESTAMP_DELAY>;

__interface IProposalRoot {

  [[internal, noaccept]]
  void deployProposal();

  // Allows a multi ballot to vote for proposal
  [[internal, noaccept, answer_id]]
  VotesInfo vote(uint256 pubkey, DepositType deposit, bool_t yes);

  // Returns true if voting is already finished
  // Also generates external message with voting results at first request after voting finish
  [[internal, noaccept, answer_id]]
  bool_t checkFinished();

  // Returns true if voting is already finished and approved
  // Also generates external message with voting results at first request after voting finish
  [[internal, noaccept, answer_id]]
  bool_t checkApproved();

  // Returns VoteResults with details
  // Also generates external message with voting results at first request after voting finish
  [[internal, noaccept, answer_id]]
  VoteResults checkResults();

  // contest notifies that it is approved
  [[internal, noaccept]]
  void contestApproved();

  // ========== getters ==========
  [[getter]]
  address getDepool();

  // Returns a price value in tons that must be deposited for one vote
  [[getter]]
  DepositType getVotePrice();

  // Returns full information about proposal
  [[getter]]
  ProposalInfo getProposal();

  [[getter]]
  dict_array<uint256> getWhiteList();
};
using IProposalRootPtr = contract_handle<IProposalRoot>;

struct DProposalRoot {
  bool_t super_majority_;
  int8 workchain_id_;
  uint256 super_root_;
  address depool_;
  DepositType votePrice_;
  cell ballot_code_;
  std::optional<cell> final_msg_; // msg sent in case of successfull approval
  Grams final_msg_value_;
  // Value will be requested from super root to contest in case of approval
  Grams final_msg_request_value_;
  uint256 id_;
  uint32 start_;
  uint32 end_;
  string desc_;
  VotesType totalVotes_;
  VotesType yesVotes_;
  VotesType noVotes_;
  bool_t result_sent_;
  bool_t white_list_enabled_;
  dict_set<uint256> white_list_;
  // destination address of final message, when it sent
  std::optional<address> final_dest_;
};

__interface EProposalRoot {
  void VotesChanged(VotesType yes, VotesType no) = 30;
  void VotingFinished(VotesType yes, VotesType no, bool_t approved) = 31;
};

// Prepare ProposalRoot StateInit structure and expected contract address (hash from StateInit)
inline
std::pair<StateInit, uint256> prepare_proposal_state_init_and_addr(DProposalRoot proposal_data, cell proposal_code) {
  cell proposal_data_cl =
    prepare_persistent_data<IProposalRoot, proposal_root_replay_protection_t, DProposalRoot>(
      proposal_root_replay_protection_t::init(), proposal_data);
  StateInit proposal_init {
    /*split_depth*/{}, /*special*/{},
    proposal_code, proposal_data_cl, /*library*/{}
  };
  cell proposal_init_cl = build(proposal_init).make_cell();
  return { proposal_init, uint256(tvm_hash(proposal_init_cl)) };
}

}} // namespace tvm::schema

