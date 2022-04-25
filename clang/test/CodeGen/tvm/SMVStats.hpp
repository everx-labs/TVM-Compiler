#pragma once

#include <tvm/schema/message.hpp>
#include <tvm/dict_array.hpp>
#include <tvm/log_sequence.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/smart_switcher.hpp>

namespace tvm { namespace schema {

// For internal purposes (fixed-size address)
struct TransferRecord {
  uint256 proposalId;
  addr_std_fixed contestAddr;
  uint256 requestValue;
};
// For getter (reconstructed address)
struct Transfer {
  uint256 proposalId;
  address contestAddr;
  uint256 requestValue;
};

__interface [[no_pubkey]] ISMVStats {

  [[internal, external, dyn_chain_parse]]
  void constructor(address SMV_root);

  [[internal, noaccept, answer_id]]
  bool_t registerTransfer(uint256 proposalId, address contestAddr, uint256 requestValue);

  // ============== getters ==============
  [[getter]]
  dict_array<Transfer> getTransfers();
};

struct DSMVStats {
  address SMV_root_;
  log_sequence<TransferRecord> transfers_;
  Grams start_balance_;
};

struct ESMVStats {};

static constexpr unsigned STATS_TIMESTAMP_DELAY = 1800;
using stats_replay_protection_t = replay_attack_protection::timestamp<STATS_TIMESTAMP_DELAY>;

inline
std::pair<StateInit, uint256> prepare_stats_state_init_and_addr(DSMVStats data, cell code) {
  cell data_cl =
    prepare_persistent_data<ISMVStats, stats_replay_protection_t, DSMVStats>(
      { stats_replay_protection_t::init() }, data);
  StateInit init {
    /*split_depth*/{}, /*special*/{},
    code, data_cl, /*library*/{}
  };
  cell init_cl = build(init).make_cell();
  return { init, uint256(tvm_hash(init_cl)) };
}

}} // namespace tvm::schema

