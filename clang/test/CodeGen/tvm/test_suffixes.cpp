// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/suffixes.hpp>

using namespace tvm;

static_assert(1_nanoev    == Evers(1));
static_assert(2_nano      == Evers(2));
static_assert(3_nEv       == Evers(3));

static_assert(1_microev   == Evers(1000));
static_assert(2_micro     == Evers(2000));
static_assert(3_mcEv      == Evers(3000));
static_assert(1.2_microev == Evers(1200));
static_assert(1.3_micro   == Evers(1300));
static_assert(1.4_mcEv    == Evers(1400));

static_assert(1_milliev   == Evers(1000000));
static_assert(2_milli     == Evers(2000000));
static_assert(3_mEv       == Evers(3000000));
static_assert(2.1_milliev == Evers(2100000));
static_assert(3.2_milli   == Evers(3200000));
static_assert(4.3_mEv     == Evers(4300000));

static_assert(1_ev        == Evers(1000000000));
static_assert(1_Ev        == Evers(1000000000));
static_assert(2.7_ev      == Evers(2700000000));
static_assert(3.92_Ev     == Evers(3920000000));
static_assert(1_UEv       == uint128(1000000000));
static_assert(1.3_UEv     == uint128(1300000000));

static_assert(3_kiloev     == Evers(3000000000000));
static_assert(4_kEv        == Evers(4000000000000));
static_assert(9.123_kiloev == Evers(9123000000000));
static_assert(3.1415_kEv   == Evers(3141500000000));
static_assert(1_UKEv       == uint128(1000000000000));
static_assert(1.3_UKEv     == uint128(1300000000000));

static_assert(9_megaev     == Evers(9000000000000000));
static_assert(3.14_megaev  == Evers(3140000000000000));
static_assert(122_MEv      == Evers(122000000000000000));
static_assert(12.34_MEv    == Evers(12340000000000000));
static_assert(1_UMEv       == uint128(1000000000000000));
static_assert(1.5_UMEv     == uint128(1500000000000000));

static_assert(2_gigaev     == Evers(2000000000000000000));
static_assert(2.2_gigaev   == Evers(2200000000000000000));
static_assert(3_GEv        == Evers(3000000000000000000));
static_assert(3.3_GEv      == Evers(3300000000000000000));
static_assert(1_UGEv       == uint128(1000000000000000000));
static_assert(1.5_UGEv     == uint128(1500000000000000000));

static_assert(1u8   == uint8(1));
static_assert(2u16  == uint16(2));
static_assert(3u32  == uint32(3));
static_assert(4u64  == uint64(4));
static_assert(5u128 == uint128(5));
static_assert(6u256 == uint256(6));

static_assert(1i8   == int8(1));
static_assert(2i16  == int16(2));
static_assert(3i32  == int32(3));
static_assert(4i64  == int64(4));
static_assert(5i128 == int128(5));
static_assert(6i256 == int256(6));

