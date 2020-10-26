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
#include "MultisigWallet.hpp"

using namespace tvm;
using namespace schema;
using namespace debot;

using ISuperRootPtr = handle<ISuperRoot>;
using IMultisigWalletPtr = handle<IMultisigWallet>;

__interface [[no_pubkey]] ISMV_Debot {

  [[external]]
  void constructor();

  [[external]]
  [[return_name("contexts")]] dict_array<Context> fetch();

  [[external]]
  resumable<void> start();

  [[external]]
  void quit();

  [[external]]
  DebotVersion getVersion();

  [[external]]
  DebotOptions getDebotOptions();
};

struct DSMV_Debot {
  std::optional<address> root_addr_;
  std::optional<address> multisig_addr_;
};

__interface ESMV_Debot {
};

static constexpr unsigned TIMESTAMP_DELAY = 1800;
using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

class SMV_Debot final : public smart_interface<ISMV_Debot>, public DSMV_Debot {
public:
  __always_inline
  void constructor() override {
  }

  __always_inline
  dict_array<Context> fetch() override {
    return {};
  }
  __always_inline
  resumable<void> start() override {
    co_await cout().print("Hello, I am SMV Debot!\n");

    if (!root_addr_)
      requestRootAddress();
    if (!multisig_addr_)
      requestMultisigAddress();

    while(true) {
      uint32 select = co_await cout().requestUint32("1). Create new proposal\n"
                                                    "2). Create ballot for voting\n"
                                                    "3). Change root address\n"
                                                    "4). Change multisig address\n"
                                                    "5). Exit\n");
      switch (select.get()) {
        case 1:
          createNewProposal();
          break;
        case 2:
          createBallot();
          break;
        case 3:
          // TODO: Fix rematerialization for contract field pointers phi-nodes
          // requestRootAddress();
          break;
        case 4:
          // TODO: Fix rematerialization for contract field pointers phi-nodes
          // requestMultisigAddress();
          break;
        case 5: co_return;
      }
    }
  }
  __always_inline
  void quit() override {
  }
  __always_inline
  DebotVersion getVersion() override {
    return { .name = "SMV DeBot v0.1.1", .semver = uint_t<24>{(1u << 8) | 1} };
  }
  __always_inline
  DebotOptions getDebotOptions() override {
    return {
      .options = uint8{0},
      .debotAbi = "",
      .targetAbi = "",
      .targetAddr = address::make_std(int8{0}, uint256{0})
    };
  }
  DEFAULT_SUPPORT_FUNCTIONS(ISMV_Debot, replay_protection_t)
private:
  IConsolePtr cout_ptr_ = IConsolePtr(address::make_std(schema::int8{0}, schema::uint256{0}));
  __always_inline IConsolePtr::proxy cout() {
    return cout_ptr_();
  }
  __always_inline
  resumable<void> createNewProposal() {
    co_return;
  }
  __always_inline
  resumable<void> createBallot() {
    address depool_addr = address::make_std(int8{0}, uint256{0});
    uint256 pubkey = co_await cout().requestPubkey("Specify pubkey for ballot:\n");
    uint256 tonsToBallot = co_await cout().requestTONs("TONs to ballot:\n");
    uint256 tonsToProcess = co_await cout().requestTONs("Processing TONs (extra TONs will be returned back to your multisig):\n");
    cell msg = ISuperRootPtr(*root_addr_).prepare_internal(Grams(tonsToProcess.get())).
      createMultiBallot(pubkey, depool_addr, tonsToBallot);
    cell external_msg = IMultisigWalletPtr(*multisig_addr_).prepare_external().
      sendTransaction(*multisig_addr_, uint128{tonsToProcess.get()}, bool_t{true}, uint8{3}, msg);
    co_await cout().print("Processing message...");
    co_await cout().signAndSendExternal(external_msg);
    co_await cout().print("Done.");
  }

  __always_inline
  resumable<void> requestRootAddress() {
    root_addr_ = co_await cout().requestAddress("Please, enter SMV root address:\n");
  }
  __always_inline
  resumable<void> requestMultisigAddress() {
    multisig_addr_ = co_await cout().requestAddress("Specify your multisig address:\n");
  }
};

DEFINE_JSON_ABI(ISMV_Debot, DSMV_Debot, ESMV_Debot);

// ----------------------------- Main entry functions ---------------------- //
DEFAULT_MAIN_ENTRY_FUNCTIONS(SMV_Debot, ISMV_Debot, DSMV_Debot, TIMESTAMP_DELAY)

