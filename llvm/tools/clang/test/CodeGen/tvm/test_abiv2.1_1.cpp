// RUN: %clang -O3 -export-json-abi %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o - | FileCheck %s
// REQUIRES: tvm-registered-target

#include <tvm/contract.hpp>
#include <tvm/smart_switcher.hpp>
#include <tvm/string.hpp>

using namespace tvm::schema;
using namespace tvm;

struct test_rv {
  varuint16 a;
  varuint32 b;
  varint16 c;
  varint32 d;
  std::optional<varint32> e;
};

__interface ITestAbi {
  void constructor();
  // CHECK: "name":"v1", "type":"optional(uint256)"
  void test_func1(std::optional<uint256> v1);
  // CHECK: "name":"value0", "type":"optional(uint64)"
  std::optional<uint64> test_func2();
  // CHECK: "name":"a", "type":"varuint16"
  // CHECK: "name":"b", "type":"varuint32"
  // CHECK: "name":"c", "type":"varint16"
  // CHECK: "name":"d", "type":"varint32"
  void test_func3(varuint16 a, varuint32 b, varint16 c, varint32 d);
  // CHECK: "name":"a", "type":"varuint16"
  // CHECK: "name":"b", "type":"varuint32"
  // CHECK: "name":"c", "type":"varint16"
  // CHECK: "name":"d", "type":"varint32"
  // CHECK: "name":"e", "type":"optional(varint32)"
  test_rv test_func4();
  // CHECK: "name":"x", "type":"string"
  // CHECK: "name":"y", "type":"string"
  // CHECK: "name":"z", "type":"string"
  // CHECK: "name":"value0", "type":"string"
  string test_func5(string x, string y, string z);
};

// CHECK: "fields":
// CHECK: "name":"__uninitialized", "type":"bool"
// CHECK: "name":"a", "type":"varuint16"
// CHECK: "name":"b", "type":"varuint32"
// CHECK: "name":"c", "type":"varint16"
// CHECK: "name":"d", "type":"varint32"

struct DTestAbi {
  varuint16 a;
  varuint32 b;
  varint16 c;
  varint32 d;
};

struct ETestAbi {};

DEFINE_JSON_ABI(ITestAbi, DTestAbi, ETestAbi);

