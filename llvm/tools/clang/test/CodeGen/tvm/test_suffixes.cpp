// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

#include <tvm/suffixes.hpp>

using namespace tvm;
using namespace schema;

static_assert(1nanoton == Grams(1));
static_assert(2nano    == Grams(2));
static_assert(3nTon    == Grams(3));
static_assert(4nT      == Grams(4));

static_assert(1microton   == Grams(1000));
static_assert(2micro      == Grams(2000));
static_assert(3mcT        == Grams(3000));
static_assert(1.2microton == Grams(1200));
static_assert(1.3micro    == Grams(1300));
static_assert(1.4mcT      == Grams(1400));

static_assert(1milliton   == Grams(1000000));
static_assert(2milli      == Grams(2000000));
static_assert(3mT         == Grams(3000000));
static_assert(2.1milliton == Grams(2100000));
static_assert(3.2milli    == Grams(3200000));
static_assert(4.3mT       == Grams(4300000));

static_assert(1ton    == Grams(1000000000));
static_assert(1Ton    == Grams(1000000000));
static_assert(2.7ton  == Grams(2700000000));
static_assert(3.92Ton == Grams(3920000000));
static_assert(1T      == Grams(1000000000));
static_assert(1.2T    == Grams(1200000000));
static_assert(1UT     == uint128(1000000000));
static_assert(1.3UT   == uint128(1300000000));

static_assert(3kiloton == Grams(3000000000000));
static_assert(4kTon    == Grams(4000000000000));
static_assert(9.123kiloton == Grams(9123000000000));
static_assert(3.1415kTon   == Grams(3141500000000));
static_assert(1KT    == Grams(1000000000000));
static_assert(1.2KT  == Grams(1200000000000));
static_assert(1UKT   == uint128(1000000000000));
static_assert(1.3UKT == uint128(1300000000000));

static_assert(9megaton    == Grams(9000000000000000));
static_assert(3.14megaton == Grams(3140000000000000));
static_assert(122MTon     == Grams(122000000000000000));
static_assert(12.34MTon   == Grams(12340000000000000));
static_assert(1MT    == Grams(1000000000000000));
static_assert(1.7MT  == Grams(1700000000000000));
static_assert(1UMT   == uint128(1000000000000000));
static_assert(1.5UMT == uint128(1500000000000000));

static_assert(2gigaton   == Grams(2000000000000000000));
static_assert(2.2gigaton == Grams(2200000000000000000));
static_assert(3GTon      == Grams(3000000000000000000));
static_assert(3.3GTon    == Grams(3300000000000000000));
static_assert(4GT        == Grams(4000000000000000000));
static_assert(4.4GT      == Grams(4400000000000000000));
static_assert(1UGT     == uint128(1000000000000000000));
static_assert(1.5UGT   == uint128(1500000000000000000));

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

