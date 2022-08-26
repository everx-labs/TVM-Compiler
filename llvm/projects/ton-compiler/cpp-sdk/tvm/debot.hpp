#pragma once

#include <tvm/sequence.hpp>
#include <tvm/schema/basics.hpp>

namespace tvm { namespace debot {

using namespace schema;

static constexpr uint8 ACTION_EMPTY        {0u};
static constexpr uint8 ACTION_RUN_ACTION   {1u};    // Call debot function associated with action
static constexpr uint8 ACTION_RUN_METHOD   {2u};    // Call get-method of smart contract controlled by debot.
static constexpr uint8 ACTION_SEND_MSG     {3u};    // Send a message to smart contract controlled by debot.
static constexpr uint8 ACTION_INVOKE_DEBOT {4u};    // Call action from another debot
static constexpr uint8 ACTION_PRINT		     {5u};    // Print string to user
// set of helper actions that must be performed by debot engine
static constexpr uint8 ACTION_CALL_ENGINE  {10u};    // Parse integer from string.

static constexpr uint8 DEBOT_ABI           {1u};
static constexpr uint8 DEBOT_TARGET_ABI    {2u};
static constexpr uint8 DEBOT_TARGET_ADDR   {4u};

static constexpr uint8 STATE_ZERO      {0u};   // initial state, before we start
static constexpr uint8 STATE_STARTED   {1u};// "home" - after the start command
static constexpr uint8 STATE_EXIT      {255u}; // we're done

struct Action {
  // String that describes action step, should be printed to user
  string desc;
  // Name of debot function that runs this action
  string name;
  // Action type
  uint8 actionType;
  // Action attributes.
  // Syntax: "attr1,attr2,attr3=value,...".
  // Example: "instant,fargs=fooFunc,sign=by-user,func=foo"
  string attrs;
  // Context to transit to
	uint8 to;
  // Action Context
  cell misc;
};

struct Context {
  string desc;      // message to be printed to the user
  uint8 id;		      // Context ordinal
  dict_array<Action> actions;	// list of actions
};

struct DebotVersion {
  string name;
  uint_t<24> semver;
};

struct DebotOptions {
  uint8 options;
  string debotAbi;
  string targetAbi;
  address targetAddr;
};

}} // namespace tvm::debot

