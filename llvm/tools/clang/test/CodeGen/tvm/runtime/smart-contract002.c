// RUN: %clang -O3 -S -c -target tvm %s -o %t
// RUN: %clang -O3 -S -c -target tvm %S/../../../../../../../samples/sdk-prototype/transfer-80000001.c -o %t.transfer-80000001
// RUN: %clang -O3 -S -c -target tvm %S/../../../../../../../samples/sdk-prototype/ton-sdk/messages.c -o %t.messages
// RUN: %clang -O3 -S -c -target tvm %S/../../../../../../../samples/sdk-prototype/ton-sdk/smart-contract-info.c -o %t.smart-contract-info
// RUN: %clang -O3 -S -c -target tvm %S/../../../../../../../samples/sdk-prototype/ton-sdk/tvm.c -o %t.tvm
// RUN: cat %t %t.transfer-80000001 %t.messages %t.smart-contract-info %t.tvm > %t.combined
// RUN: tvm-testrun --no-trace --entry test --stdlibc-path %S/../../../../../../../samples/sdk-prototype/stdlib_c.tvm < %t.combined | FileCheck %s
// XFAIL: *

void produce_output();

void test()
{
//CHECK: Output actions:
//CHECK-NEXT: ----------------
//CHECK-NEXT: Action(SendMsg):
//CHECK-NEXT: message header
//CHECK-NEXT: ihr_disabled: false
//CHECK-NEXT: bounce      : false
//CHECK-NEXT: bounced     : false
//CHECK-NEXT: source      : 0:0000000000000000000000000000000000000000000000000000000080000001
//CHECK-NEXT: destination : 0:0000000000000000000000000000000000000000000000000000000080000001
//CHECK-NEXT: value       : CurrencyCollection: Grams vui16[len = 2, value = 43690], other curencies:
//CHECK-NEXT: count: 0
//CHECK-NEXT: ihr_fee     : vui16[len = 0, value = 0]
//CHECK-NEXT: fwd_fee     : vui16[len = 0, value = 0]
//CHECK-NEXT: created_lt  : 0
//CHECK-NEXT: created_at  : 0
//CHECK-NEXT: init  : None
//CHECK-NEXT: body : Some(SliceData { cell: CellData { references: [], data: [8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 74, 170, 168, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128], cell_type: Ordinary, level_mask: LevelMask(0), store_hashes: false, hashes: Some([UInt256([93, 226, 55, 232, 5, 84, 246, 27, 233, 180, 33, 151, 74, 70, 136, 132, 118, 213, 143, 212, 127, 192, 143, 94, 68, 1, 63, 116, 88, 131, 79, 56])]), depths: Some([0]) }, data_window: 665..680, references_window: 0..0 })
//CHECK-NEXT: body_hex: 0000
//CHECK-EMPTY:
//CHECK-NEXT: TEST COMPLETED
  produce_output();
}
