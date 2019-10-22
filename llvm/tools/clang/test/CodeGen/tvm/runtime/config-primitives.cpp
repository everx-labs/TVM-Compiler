// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -I%S/../../../../../../../stdlib/cpp-sdk/ -o - | tvm-testrun --no-trace --entry main | FileCheck %s

#include "smart_contract_info.hpp"

// CHECK-NOT: Unhandled exception
// CHECK: TVM terminated with exit code 0
// CHECK: Contract persistent data updated

int main() {
  // Comparing unpacked as a full tuple config parameters with
  //  personal parameter requests.

  // Unpacked as a full tuple
  auto full_info = tvm::smart_contract_info::get().unpack();

  auto now1 = full_info.get_now();
  auto blocklt1 = full_info.get_blocklt();
  auto ltime1 = full_info.get_ltime();
  auto myaddr1 = full_info.get_myaddr();
  auto configroot1 = full_info.get_configroot();

  __builtin_tvm_throwif(now1 != tvm::smart_contract_info::now(), 101);
  __builtin_tvm_throwif(blocklt1 != tvm::smart_contract_info::blocklt(), 102);
  __builtin_tvm_throwif(ltime1 != tvm::smart_contract_info::ltime(), 103);
  bool addr_eq = __builtin_tvm_sdeq(myaddr1, tvm::smart_contract_info::myaddr());
  __builtin_tvm_throwif(!addr_eq, 104);
  auto configroot2 = tvm::smart_contract_info::configroot();
  if (!__builtin_tvm_isnull_cell(configroot1) && !__builtin_tvm_isnull_cell(configroot2)) {
    bool config_eq = __builtin_tvm_sdeq(__builtin_tvm_ctos(configroot1),
                                        __builtin_tvm_ctos(configroot2));
    __builtin_tvm_throwif(!config_eq, 105);
  }
  
  auto configdict2 = tvm::smart_contract_info::configdict();
  if (!__builtin_tvm_isnull_cell(configroot1) && !__builtin_tvm_isnull_cell(configdict2.v0)) {
    bool config_eq2 = __builtin_tvm_sdeq(__builtin_tvm_ctos(configroot1),
                                         __builtin_tvm_ctos(configdict2.v0));
    __builtin_tvm_throwif(!config_eq2, 106);
  }
  __builtin_tvm_throwif(configdict2.v1 != 32, 107);
 
  return 0;
}

