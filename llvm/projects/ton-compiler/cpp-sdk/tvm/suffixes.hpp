#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm {

static constexpr unsigned one_full_ton_size = 1000000000;

constexpr Grams operator "" nanoton(unsigned long long v) {
  return Grams(v);
}
constexpr Grams operator "" nano(unsigned long long v) {
  return Grams(v);
}
constexpr Grams operator "" nTon(unsigned long long v) {
  return Grams(v);
}
constexpr Grams operator "" nT(unsigned long long v) {
  return Grams(v);
}

constexpr Grams operator "" microton(long double v) {
  return Grams(static_cast<unsigned>(v * 1000));
}
constexpr Grams operator "" micro(long double v) {
  return Grams(static_cast<unsigned>(v * 1000));
}
constexpr Grams operator "" mcT(long double v) {
  return Grams(static_cast<unsigned>(v * 1000));
}
constexpr Grams operator "" microton(unsigned long long v) {
  return Grams(v * 1000);
}
constexpr Grams operator "" micro(unsigned long long v) {
  return Grams(v * 1000);
}
constexpr Grams operator "" mcT(unsigned long long v) {
  return Grams(v * 1000);
}

constexpr Grams operator "" milliton(long double v) {
  return Grams(static_cast<unsigned>(v * 1000000));
}
constexpr Grams operator "" milli(long double v) {
  return Grams(static_cast<unsigned>(v * 1000000));
}
constexpr Grams operator "" mT(long double v) {
  return Grams(static_cast<unsigned>(v * 1000000));
}
constexpr Grams operator "" milliton(unsigned long long v) {
  return Grams(v * 1000000);
}
constexpr Grams operator "" milli(unsigned long long v) {
  return Grams(v * 1000000);
}
constexpr Grams operator "" mT(unsigned long long v) {
  return Grams(v * 1000000);
}

constexpr Grams operator "" ton(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size));
}
constexpr Grams operator "" Ton(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size));
}
constexpr Grams operator "" ton(unsigned long long v) {
  return Grams(v * one_full_ton_size);
}
constexpr Grams operator "" Ton(unsigned long long v) {
  return Grams(v * one_full_ton_size);
}
constexpr Grams operator "" T(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size));
}
constexpr Grams operator "" T(unsigned long long v) {
  return Grams(v * one_full_ton_size);
}
constexpr uint128 operator "" UT(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ton_size));
}
constexpr uint128 operator "" UT(unsigned long long v) {
  return uint128(v * one_full_ton_size);
}

constexpr Grams operator "" kiloton(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000));
}
constexpr Grams operator "" kiloton(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000);
}
constexpr Grams operator "" kTon(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000));
}
constexpr Grams operator "" kTon(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000);
}
constexpr Grams operator "" KT(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000));
}
constexpr Grams operator "" KT(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000);
}
constexpr uint128 operator "" UKT(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ton_size * 1000));
}
constexpr uint128 operator "" UKT(unsigned long long v) {
  return uint128(v * one_full_ton_size * 1000);
}

constexpr Grams operator "" megaton(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000));
}
constexpr Grams operator "" megaton(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000);
}
constexpr Grams operator "" MTon(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000));
}
constexpr Grams operator "" MTon(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000);
}
constexpr Grams operator "" MT(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000));
}
constexpr Grams operator "" MT(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000);
}
constexpr uint128 operator "" UMT(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ton_size * 1000000));
}
constexpr uint128 operator "" UMT(unsigned long long v) {
  return uint128(v * one_full_ton_size * 1000000);
}

constexpr Grams operator "" gigaton(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000000));
}
constexpr Grams operator "" gigaton(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000000);
}
constexpr Grams operator "" GTon(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000000));
}
constexpr Grams operator "" GTon(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000000);
}
constexpr Grams operator "" GT(long double v) {
  return Grams(static_cast<unsigned>(v * one_full_ton_size * 1000000000));
}
constexpr Grams operator "" GT(unsigned long long v) {
  return Grams(v * one_full_ton_size * 1000000000);
}
constexpr uint128 operator "" UGT(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ton_size * 1000000000));
}
constexpr uint128 operator "" UGT(unsigned long long v) {
  return uint128(v * one_full_ton_size * 1000000000);
}

constexpr uint8 operator "" u8(unsigned long long v) {
  return uint8(v);
}
constexpr uint16 operator "" u16(unsigned long long v) {
  return uint16(v);
}
constexpr uint32 operator "" u32(unsigned long long v) {
  return uint32(v);
}
constexpr uint64 operator "" u64(unsigned long long v) {
  return uint64(v);
}
constexpr uint128 operator "" u128(unsigned long long v) {
  return uint128(v);
}
constexpr uint256 operator "" u256(unsigned long long v) {
  return uint256(v);
}

constexpr int8 operator "" i8(unsigned long long v) {
  return int8(v);
}
constexpr int16 operator "" i16(unsigned long long v) {
  return int16(v);
}
constexpr int32 operator "" i32(unsigned long long v) {
  return int32(v);
}
constexpr int64 operator "" i64(unsigned long long v) {
  return int64(v);
}
constexpr int128 operator "" i128(unsigned long long v) {
  return int128(v);
}
constexpr int256 operator "" i256(unsigned long long v) {
  return int256(v);
}

} // namespace tvm

