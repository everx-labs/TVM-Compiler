#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm {

static constexpr unsigned one_full_ev_size = 1000000000;

constexpr Evers operator "" _nanoev(unsigned long long v) {
  return Evers(v);
}
constexpr Evers operator "" _nano(unsigned long long v) {
  return Evers(v);
}
constexpr Evers operator "" _nEv(unsigned long long v) {
  return Evers(v);
}

constexpr Evers operator "" _microev(long double v) {
  return Evers(static_cast<unsigned>(v * 1000));
}
constexpr Evers operator "" _micro(long double v) {
  return Evers(static_cast<unsigned>(v * 1000));
}
constexpr Evers operator "" _mcEv(long double v) {
  return Evers(static_cast<unsigned>(v * 1000));
}
constexpr Evers operator "" _microev(unsigned long long v) {
  return Evers(v * 1000);
}
constexpr Evers operator "" _micro(unsigned long long v) {
  return Evers(v * 1000);
}
constexpr Evers operator "" _mcEv(unsigned long long v) {
  return Evers(v * 1000);
}

constexpr Evers operator "" _milliev(long double v) {
  return Evers(static_cast<unsigned>(v * 1000000));
}
constexpr Evers operator "" _milli(long double v) {
  return Evers(static_cast<unsigned>(v * 1000000));
}
constexpr Evers operator "" _mEv(long double v) {
  return Evers(static_cast<unsigned>(v * 1000000));
}
constexpr Evers operator "" _milliev(unsigned long long v) {
  return Evers(v * 1000000);
}
constexpr Evers operator "" _milli(unsigned long long v) {
  return Evers(v * 1000000);
}
constexpr Evers operator "" _mEv(unsigned long long v) {
  return Evers(v * 1000000);
}

constexpr Evers operator "" _ev(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size));
}
constexpr Evers operator "" _Ev(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size));
}
constexpr Evers operator "" _ev(unsigned long long v) {
  return Evers(v * one_full_ev_size);
}
constexpr Evers operator "" _Ev(unsigned long long v) {
  return Evers(v * one_full_ev_size);
}
constexpr uint128 operator "" _UEv(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ev_size));
}
constexpr uint128 operator "" _UEv(unsigned long long v) {
  return uint128(v * one_full_ev_size);
}

constexpr Evers operator "" _kiloev(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000));
}
constexpr Evers operator "" _kiloev(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000);
}
constexpr Evers operator "" _kEv(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000));
}
constexpr Evers operator "" _kEv(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000);
}
constexpr uint128 operator "" _UKEv(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ev_size * 1000));
}
constexpr uint128 operator "" _UKEv(unsigned long long v) {
  return uint128(v * one_full_ev_size * 1000);
}

constexpr Evers operator "" _megaev(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000000));
}
constexpr Evers operator "" _megaev(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000000);
}
constexpr Evers operator "" _MEv(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000000));
}
constexpr Evers operator "" _MEv(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000000);
}
constexpr uint128 operator "" _UMEv(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ev_size * 1000000));
}
constexpr uint128 operator "" _UMEv(unsigned long long v) {
  return uint128(v * one_full_ev_size * 1000000);
}

constexpr Evers operator "" _gigaev(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000000000));
}
constexpr Evers operator "" _gigaev(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000000000);
}
constexpr Evers operator "" _GEv(long double v) {
  return Evers(static_cast<unsigned>(v * one_full_ev_size * 1000000000));
}
constexpr Evers operator "" _GEv(unsigned long long v) {
  return Evers(v * one_full_ev_size * 1000000000);
}
constexpr uint128 operator "" _UGEv(long double v) {
  return uint128(static_cast<unsigned>(v * one_full_ev_size * 1000000000));
}
constexpr uint128 operator "" _UGEv(unsigned long long v) {
  return uint128(v * one_full_ev_size * 1000000000);
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

