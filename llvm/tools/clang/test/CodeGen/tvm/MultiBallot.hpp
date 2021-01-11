#pragma once

#include <tvm/schema/message.hpp>

#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/dict_array.hpp>
#include <tvm/dict_set.hpp>

namespace tvm { namespace schema {

using address = lazy<MsgAddressInt>;
using VotesType = uint128;
using DepositType = uint256;

#define GASTOGRAM_WORKAROUND

__always_inline unsigned gastogram(unsigned gas) {
  // TODO: solve gastogram problem
#ifdef GASTOGRAM_WORKAROUND
  return gas * 10000;
#else
  return __builtin_tvm_gastogram(gas);
#endif
}

static constexpr unsigned MULTI_BALLOT_TIMESTAMP_DELAY = 1800;
using multi_ballot_replay_protection_t = replay_attack_protection::timestamp<MULTI_BALLOT_TIMESTAMP_DELAY>;

struct sendVotesResult {
  DepositType has_deposit;
  DepositType already_sent_deposit;
  DepositType new_sent_deposit;
};

struct requestDepositResult {
  bool_t succeeded;
  uint256 unfinished_proposal;
};

__interface IMultiBallot {

  __attribute__((internal, noaccept))
  void constructor() = 1;

  // Initializes ballot variables
  __attribute__((internal, noaccept))
  void deployBallot() = 2;

  // Receive native funds transfer and keep it in deposit
  __attribute__((internal, noaccept))
  void receiveNativeTransfer(DepositType amount) = 3;

  // Receive stake transfer notify (from solidity IParticipant::onTransfer(address source, uint128 amount))
  __attribute__((internal, noaccept))
  void receiveStakeTransfer(address source, uint128 amount) = 0x23c4771d; // = hash_v<"onTransfer(address,uint128)()v2">

  // ======= externals =======

  // Sends all votes (for deposit) to Proposal Root contract
  __attribute__((external, noaccept))
  sendVotesResult sendVote(address proposal, bool_t yesOrNo) = 5;

  // The function must request affected proposals the voting is finished,
  // set to zero deposit variables and return stake and native deposits to user_wallet
  __attribute__((external, noaccept))
  resumable<void> requestDeposit(address user_wallet) = 6;

  // send all remaining tons to user_wallet
  __attribute__((external, noaccept))
  void finalize(address user_wallet) = 7;

  // ======= getters =======

  __attribute__((getter))
  DepositType getNativeDeposit() = 8;

  __attribute__((getter))
  DepositType getStakeDeposit() = 9;

  __attribute__((getter, no_persistent))
  DepositType getMultiBallot_receiveNativeTransfer_GasPrice() = 10;

  __attribute__((getter, no_persistent))
  DepositType getMultiBallot_receiveStakeTransfer_GasPrice() = 11;
};

struct DMultiBallot {
  uint256 ballot_public_key_;
  int8 workchain_id_;
  uint256 super_root_;
  address depool_;
  DepositType native_deposit_;
  DepositType stake_deposit_;

  // map of voted proposal addresses to sent deposit
  dict_map<uint256, DepositType> proposals_;
};

struct EMultiBallot {
};

// Prepare MultiBallot StateInit structure and expected contract address (hash from StateInit)
inline
std::pair<StateInit, uint256> prepare_ballot_state_init_and_addr(DMultiBallot ballot_data, cell ballot_code) {
  cell ballot_data_cl =
    prepare_persistent_data<IMultiBallot, multi_ballot_replay_protection_t, DMultiBallot>(
      { multi_ballot_replay_protection_t::init(), {} }, ballot_data);
  StateInit ballot_init {
    /*split_depth*/{}, /*special*/{},
    ballot_code, ballot_data_cl, /*library*/{}
  };
  cell ballot_init_cl = build(ballot_init).make_cell();
  return { ballot_init, uint256(tvm_hash(ballot_init_cl)) };
}

}} // namespace tvm::schema

