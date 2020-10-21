// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/
// REQUIRES: tvm-registered-target

#include <tvm/contract.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/contract_handle.hpp>
#include <tvm/string.hpp>
#include <tvm/replay_attack_protection/timestamp.hpp>
#include <tvm/default_support_functions.hpp>
#include <tvm/debot.hpp>

using namespace tvm;
using namespace schema;
using namespace debot;

__interface [[no_pubkey]] ISMV_Debot {

  [[external, dyn_chain_parse]]
  void constructor(uint8 options, string debotAbi, string targetAbi, address targetAddr);

  [[external]]
  [[return_name("contexts")]] dict_array<Context> fetch();

  [[external]]
  void start();

  [[external]]
  void quit();

  [[external]]
  DebotVersion getVersion();

  [[external]]
  DebotOptions getDebotOptions();
};

static_assert(get_func_id<&ISMV_Debot::constructor>() == 0x172daa29);
static_assert(get_func_id<ISMV_Debot, 0>() == 0x172daa29);
static_assert(get_func_id<&ISMV_Debot::fetch>() == 0x68c4f887);
static_assert(get_func_id<ISMV_Debot, 1>() == 0x68c4f887);
static_assert(get_func_id<&ISMV_Debot::start>() == 0x059c0d6f);
static_assert(get_func_id<ISMV_Debot, 2>() == 0x059c0d6f);
static_assert(get_func_id<&ISMV_Debot::quit>() == 0x30025d94);
static_assert(get_func_id<ISMV_Debot, 3>() == 0x30025d94);
static_assert(get_func_id<&ISMV_Debot::getVersion>() == 0x3541576f);
static_assert(get_func_id<ISMV_Debot, 4>() == 0x3541576f);
static_assert(get_func_id<&ISMV_Debot::getDebotOptions>() == 0x7decd0db);
static_assert(get_func_id<ISMV_Debot, 5>() == 0x7decd0db);

