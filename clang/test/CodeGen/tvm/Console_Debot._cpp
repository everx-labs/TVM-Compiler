// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/contract.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/contract_handle.hpp>
#include <tvm/string.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/debot.hpp>
#include <tvm/default_support_functions.hpp>
#include <tvm/Console.hpp>
#include "SuperRoot.hpp"
#include "ProposalRoot.hpp"
#include "MultiBallot.hpp"
#include "MultisigWallet.hpp"

using namespace tvm;
using namespace schema;
using namespace debot;

using ISuperRootPtr = handle<ISuperRoot>;
using IProposalRootPtr = handle<IProposalRoot>;
using IMultiBallotPtr = handle<IMultiBallot>;
using IMultisigWalletPtr = handle<IMultisigWallet>;

__interface [[no_pubkey]] ISMV_Debot {

  [[external]]
  void constructor() = 1;

  [[external]]
  resumable<void> start() = 2;
};

struct DSMV_Debot {
  std::optional<ISuperRootPtr> super_root_;
  std::optional<IMultiBallotPtr> ballot_;
};

__interface ESMV_Debot {
};

static constexpr unsigned TIMESTAMP_DELAY = 1800;
using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

class SMV_Debot final : public smart_interface<ISMV_Debot>, public DSMV_Debot {
  IConsolePtr cout_ptr_ = IConsolePtr(address::make_std(schema::int8{0}, schema::uint256{0}));

  __always_inline IConsolePtr::proxy cout() {
    return cout_ptr_();
  }
  __always_inline
  auto printf(string fmt) {
    cell params = builder().endc();
    return cout().printf(fmt, params);
  }
  template<typename... Args>
  __always_inline
  auto printf(string fmt, Args... args) {
    auto tup = std::make_tuple(args...);
    auto chain = make_chain_tuple(tup);
    cell params = build(chain).endc();
    return cout().printf(fmt, params);
  }
public:
  __always_inline
  void constructor() override {
  }

  __always_inline
  resumable<void> start() override {
    co_await cout().print("Hello, I am SMV Debot!");

    if (!super_root_)
      super_root_ = ISuperRootPtr(co_await cout().inputAddress("SMV root address:", "SMV.root"));

    auto root_initialized = co_await super_root_->run().isFullyInitialized();
    if (!root_initialized) {
      co_await printf("Root is not initialized, exiting");
      co_return;
    }
    co_await printf("SMV root details \\{\n"
                    "  budget:     {address};\n"
                    "  statistics: {address};\n"
                    "  depool:     {address};\n"
                    "  proposals:  {uint32}\n"
                    "\\}",
      co_await super_root_->run().getBudget(),
      co_await super_root_->run().getStats(),
      co_await super_root_->run().getDepool(),
      (co_await super_root_->run().getProposalIds()).size()
    );

    while (true) {
      co_await cout().iAmHome();
      co_await cout().print("Choose a command:");
      if (ballot_) {
        auto ballot = *ballot_;
        auto cur_deposit = co_await ballot.run().getNativeDeposit() + co_await ballot.run().getStakeDeposit();
        co_await printf("Current ballot deposit: {value}", cur_deposit);
        co_await printf("1). New proposal\n"
                        "2). Change ballot for voting ({address})\n"
                        "3). Change root address ({address})\n"
                        "4). Increase ballot native deposit\n"
                        "5). Vote for proposal\n"
                        "6). Close ballot and return stakes\n"
                        "7). Exit",
                        ballot_->get(), super_root_->get());
      } else {
        co_await printf("1). New proposal\n"
                        "2). Create or set ballot for voting\n"
                        "3). Change root address ({address})\n"
                        "4). ---\n"
                        "5). ---\n"
                        "6). ---\n"
                        "7). Exit",
                        super_root_->get());
      }

      auto select = co_await cout().inputUint256("");
      switch (select.get()) {
      case 7:
        co_await cout().print("You choose to exit, bye");
        co_return;
      case 1: {
        co_await cout().print("Creating new proposal...");
        uint256 proposal_id = co_await cout().inputUint256("Proposal id:");
        uint128 totalVotes((co_await cout().inputUint256("Total votes:")).get());
        uint32 startime = co_await cout().inputDateTime("Start date:");
        uint32 endtime = co_await cout().inputDateTime("End date:");
        auto desc = co_await cout().inputStr("Description:");
        auto superMajority = co_await cout().inputYesOrNo("Super majority (y/n)?");
        auto votePrice = co_await cout().inputTONs("Vote price:");
        auto finalMsgEnabled = co_await cout().inputYesOrNo("Enable success message to deploy contest (y/n)?");
        cell finalMsg = builder().endc();
        uint256 finalMsgValue(0), finalMsgRequestValue(0);
        if (finalMsgEnabled) {
          finalMsg = co_await cout().inputDeployMessage("Final message code:");
          finalMsgValue = co_await cout().inputTONs("TONs to success message:");
          finalMsgRequestValue = co_await cout().inputTONs("TONs to request from budget in case of approved contest:");
        }
        auto whiteListEnabled = co_await cout().inputYesOrNo("Enable white pubkeys list (only able to vote) (y/n)?");
        dict_array<uint256> whitePubkeys;
        if (whiteListEnabled) {
          do {
            uint256 pubkey = co_await cout().inputPubkey("White pubkey:");
            whitePubkeys.push_back(pubkey);
          } while (co_await cout().inputYesOrNo("Add next white pubkey (y/n)?"));
        }

        auto processVal = co_await cout().inputTONs("Processing TONs (extra TONs will be returned back to your multisig):");
        Grams tonsToProcess(processVal.get());

        co_await printf("Proposal #{uint256} with total votes {uint128}, super majority: {bool}, vote price: {value},",
          proposal_id, totalVotes, superMajority, votePrice);
        co_await printf("success message enabled: {bool}, white list enabled: {bool},", finalMsgEnabled, whiteListEnabled);
        co_await printf("will starts at {datetime} and ends at {datetime},",
          startime, endtime);
        co_await printf("description:");
        co_await cout().print(desc);
        
        auto sendMessage = co_await cout().inputYesOrNo("Create proposal with the defined parameters (y/n)?");
        if (!sendMessage)
          break;

        auto root = *super_root_;
        co_await root(tonsToProcess).createProposal(proposal_id, totalVotes,
          startime, endtime, desc, superMajority, votePrice,
          finalMsgEnabled, finalMsg, finalMsgValue, finalMsgRequestValue,
          whiteListEnabled, whitePubkeys);
        auto proposal_addr = co_await root.run().getProposalAddress(proposal_id);
        co_await printf("Proposal created: {address}", proposal_addr);
        break;
      }
      case 2: {
        auto setup_existing = co_await cout().inputYesOrNo("Setup existing ballot (y/n)?");
        if (setup_existing) {
          ballot_ = IMultiBallotPtr(co_await cout().inputAddress("MultiBallot address:", "SMV.ballot"));
        } else {
          auto root = *super_root_;
          uint256 pubkey = co_await cout().inputPubkey("Pubkey for ballot:");
          auto expected_ballot_addr = co_await root.run().getMultiBallotAddress(pubkey);
          co_await printf("Expected multiballot address = {address}", expected_ballot_addr);

          auto tonsToBallot = co_await cout().inputTONs("TONs to ballot:");
          uint256 processVal;
          while (true) {
            processVal = co_await cout().inputTONs("Processing TONs (extra TONs will be returned back to your multisig):");
            if (processVal <= tonsToBallot) {
              co_await printf("Processing TONs ({value}) must be greater than TONs to ballot ({value})",
                processVal, tonsToBallot);
              continue;
            }
            break;
          }
          Grams tonsToProcess(processVal.get());
          co_await cout().print("Processing message...");
          ballot_ = IMultiBallotPtr(co_await root(tonsToProcess).createMultiBallot(pubkey, tonsToBallot));
          co_await printf("Real multiballot address = {address}", *ballot_);
        }
        break;
      }
      case 3: {
        super_root_ = ISuperRootPtr(co_await cout().inputAddress("SMV root address:", "SMV.root"));
        auto root_initialized = co_await super_root_->run().isFullyInitialized();
        if (!root_initialized) {
          co_await printf("Root is not initialized, exiting");
          co_return;
        }
        co_await printf("SMV root details \\{\n"
                        "  budget:     {address};\n"
                        "  statistics: {address};\n"
                        "  depool:     {address};\n"
                        "  proposals:  {uint32}\n"
                        "\\}",
          co_await super_root_->run().getBudget(),
          co_await super_root_->run().getStats(),
          co_await super_root_->run().getDepool(),
          (co_await super_root_->run().getProposalIds()).size()
        );
        break;
      }
      case 4: {
        if (!ballot_) {
          co_await cout().print("Setup your ballot for voting first");
          break;
        }
        auto ballot = *ballot_;
        auto tonsToDeposit = co_await cout().inputTONs("TONs to ballot deposit:");
        uint256 processVal;
        while (true) {
          processVal = co_await cout().inputTONs("Processing TONs:");
          if (processVal <= tonsToDeposit) {
            co_await printf("Processing TONs ({value}) must be greater than TONs to ballot deposit ({value})",
              processVal, tonsToDeposit);
            continue;
          }
          break;
        }
        uint128 tonsToProcess(processVal.get());
        cell body_payload = ballot.body_internal().receiveNativeTransfer(tonsToDeposit);
        auto multisig = IMultisigWalletPtr(co_await cout().inputAddress("Multisig address for deposit:", "big_multisig"));
        co_await multisig.debot_ext_in().sendTransaction(ballot.get(), tonsToProcess, bool_t{true}, uint8{3}, body_payload);
        break;
      }
      case 5: {
        if (!ballot_) {
          co_await cout().print("Setup your ballot for voting first");
          break;
        }
        auto ballot = *ballot_;
        co_await printf(
          "1). Enter proposal address\n"
          "2). Iterate active proposals");
        auto sel = co_await cout().inputUint256("");
        if (sel == 1) {
          auto proposal = IProposalRootPtr(co_await cout().inputAddress("Proposal address:", "SMV.proposal"));
          auto p = co_await proposal.run().getProposal();
          co_await printf("Proposal address: {address}", proposal.get());
          co_await printf("id: {uint256}, finished: {bool}, approved: {bool}, resultsSent: {bool}, earlyFinished: {bool}, whiteListEnabled: {bool}",
            p.id, p.finished, p.approved, p.resultsSent,
            p.earlyFinished, p.whiteListEnabled);
          co_await printf("Voting period: {datetime} - {datetime}", p.start, p.end);
          co_await printf("totalVotes: {uint128}, currentVotes: {uint128}, yesVotes: {uint128}, noVotes: {uint128}",
            p.totalVotes, p.currentVotes, p.yesVotes, p.noVotes);
          co_await printf("votePrice: {value}", p.votePrice);
          co_await printf("description:");
          co_await cout().print(p.desc);

          uint256 votePrice = co_await proposal.run().getVotePrice();
          auto nativeDeposit = co_await ballot.run().getNativeDeposit();
          auto stakeDeposit = co_await ballot.run().getStakeDeposit();
          auto commonDeposit = nativeDeposit + stakeDeposit;
          co_await printf("Your ballot has native deposit: {value}, and stake deposit: {value}, combined: {value}",
            nativeDeposit, stakeDeposit, commonDeposit);
          co_await printf("Vote price in this proposal: {value}", votePrice);
          if (votePrice > commonDeposit) {
            co_await printf("Not enough deposit for voting ({value} < {value})", commonDeposit, votePrice);
            break;
          }
          uint256 expectedVotes = commonDeposit / votePrice;
          if (expectedVotes > 1)
            co_await printf("Ballot may have {uint256} votes in this proposal", expectedVotes);
          else
            co_await printf("Ballot may have one vote in this proposal");

          auto support = co_await cout().inputYesOrNo("Support this proposal (y/n)?");
          auto vote_res = co_await ballot.debot_ext_in().sendVote(proposal.get(), support);
          co_await printf("has_deposit = {value}, already_sent_deposit = {value}, new_sent_deposit = {value}",
            vote_res.has_deposit, vote_res.already_sent_deposit, vote_res.new_sent_deposit);

          break;
        } else {
          const auto proposals = co_await super_root_->run().getProposalIds();
          for (uint256 prop_id : proposals) {
            auto prop = IProposalRootPtr((co_await super_root_->run().getProposalById(prop_id)).root);
            auto p = co_await prop.run().getProposal();
            if (p.finished) {
              co_await printf("#{uint256} is finished, skipping", prop_id);
              continue;
            }
            co_await printf("Proposal address: {address}", prop.get());
            co_await printf("id: {uint256}, finished: {bool}, approved: {bool}, resultsSent: {bool}, earlyFinished: {bool}, whiteListEnabled: {bool}",
              p.id, p.finished, p.approved, p.resultsSent,
              p.earlyFinished, p.whiteListEnabled);
            co_await printf("Voting period: {datetime} - {datetime}", p.start, p.end);
            co_await printf("totalVotes: {uint128}, currentVotes: {uint128}, yesVotes: {uint128}, noVotes: {uint128}",
              p.totalVotes, p.currentVotes, p.yesVotes, p.noVotes);
            co_await printf("votePrice: {value}", p.votePrice);
            co_await printf("description:");
            co_await cout().print(p.desc);

            auto nativeDeposit = co_await ballot.run().getNativeDeposit();
            auto stakeDeposit = co_await ballot.run().getStakeDeposit();
            auto commonDeposit = nativeDeposit + stakeDeposit;
            if (p.votePrice > commonDeposit) {
              co_await printf("Not enough deposit for voting ({value} < {value}), skipping", commonDeposit, p.votePrice);
            } else {
              co_await printf("Your ballot has native deposit: {value}, and stake deposit: {value}, combined: {value}",
                nativeDeposit, stakeDeposit, commonDeposit);
              uint256 expectedVotes = commonDeposit / p.votePrice;
              if (expectedVotes > 1)
                co_await printf("Ballot may have {uint256} votes in this proposal", expectedVotes);
              else
                co_await printf("Ballot may have one vote in this proposal");
              auto do_vote = co_await cout().inputYesOrNo("Vote for this proposal (y/n)?");
              if (do_vote) {
                auto support = co_await cout().inputYesOrNo("Support this proposal (y/n)?");
                auto vote_res = co_await ballot.debot_ext_in().sendVote(prop.get(), support);
                co_await printf("has_deposit = {uint256}, already_sent_deposit = {uint256}, new_sent_deposit = {uint256}",
                  vote_res.has_deposit, vote_res.already_sent_deposit, vote_res.new_sent_deposit);
              }
            }
            auto go_next = co_await cout().inputYesOrNo("Go to the next proposal (y/n)?");
            if (!go_next) break;
          }
          co_await cout().print("No more proposals");
        }
        break;
      }
      case 6: {
        if (!ballot_) {
          co_await cout().print("Setup your ballot for voting first");
          break;
        }
        auto ballot = *ballot_;
        auto cur_deposit = co_await ballot.run().getNativeDeposit() + co_await ballot.run().getStakeDeposit();
        if (cur_deposit) {
          auto multisig_addr = co_await cout().inputAddress("Multisig address, where to transfer deposit:", "big_multisig");
          co_await ballot.debot_ext_in().requestDeposit(multisig_addr);
          co_await cout().print("Transfer processed");
        } else {
          co_await cout().print("Ballot vote deposits are empty");
        }
        auto finalize = co_await cout().inputYesOrNo("Finalize the ballot to return the remaining native funds (y/n)?");
        if (finalize) {
          auto multisig_addr = co_await cout().inputAddress("Multisig address, where to transfer native funds:", "small_multisig");
          co_await ballot.debot_ext_in().finalize(multisig_addr);
          co_await cout().print("Transfer processed");
        }
        break;
      }
      }
    }
  }
  DEFAULT_SUPPORT_FUNCTIONS(ISMV_Debot, replay_protection_t)
  
  // default processing of unknown messages
  __always_inline static int _fallback(cell msg, slice msg_body) {
    return 0;
  }
};

DEFINE_JSON_ABI(ISMV_Debot, DSMV_Debot, ESMV_Debot);

// ----------------------------- Main entry functions ---------------------- //
DEFAULT_MAIN_ENTRY_FUNCTIONS(SMV_Debot, ISMV_Debot, DSMV_Debot, TIMESTAMP_DELAY)

