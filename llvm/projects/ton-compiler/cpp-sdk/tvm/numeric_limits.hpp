#pragma once

#include <tvm/schema/basics.hpp>

namespace std {

template<unsigned _bitlen> class numeric_limits<tvm::schema::uint_t<_bitlen>> {
  using T = tvm::schema::uint_t<_bitlen>;
public:
  static constexpr bool is_specialized = true;
  static constexpr T min() noexcept { return T(0); }
  static constexpr T max() noexcept {
    return T((1u << _bitlen) - 1);
  }
  static constexpr T lowest() noexcept { return T(0); }
  static constexpr int  digits = _bitlen;
  static constexpr int  digits10 = 0;
  static constexpr bool is_signed = false;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr int radix = 2;
  static constexpr T epsilon() noexcept { return T(0); }
  static constexpr T round_error() noexcept { return T(0); }

  static constexpr int  min_exponent = 0;
  static constexpr int  min_exponent10 = 0;
  static constexpr int  max_exponent = 0;
  static constexpr int  max_exponent10 = 0;

  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr T infinity() noexcept { return T(); }
  static constexpr T quiet_NaN() noexcept { return T(); }
  static constexpr T signaling_NaN() noexcept { return T(); }
  static constexpr T denorm_min() noexcept { return min(); }

  static constexpr bool is_iec559 = false;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = false;

  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;
  static constexpr float_round_style round_style = round_toward_zero;
};

template<unsigned _bitlen> class numeric_limits<tvm::schema::int_t<_bitlen>> {
  using T = tvm::schema::int_t<_bitlen>;
public:
  static constexpr bool is_specialized = true;
  static constexpr T min() noexcept {
    return T(-static_cast<int>((1u << (_bitlen - 1)) - 1));
  }
  static constexpr T max() noexcept {
    return T((1u << (_bitlen - 1)) - 1);
  }
  static constexpr T lowest() noexcept { return min(); }
  static constexpr int  digits = _bitlen - 1;
  static constexpr int  digits10 = 0;
  static constexpr bool is_signed = true;
  static constexpr bool is_integer = true;
  static constexpr bool is_exact = true;
  static constexpr int radix = 2;
  static constexpr T epsilon() noexcept { return T(0); }
  static constexpr T round_error() noexcept { return T(0); }

  static constexpr int  min_exponent = 0;
  static constexpr int  min_exponent10 = 0;
  static constexpr int  max_exponent = 0;
  static constexpr int  max_exponent10 = 0;

  static constexpr bool has_infinity = false;
  static constexpr bool has_quiet_NaN = false;
  static constexpr bool has_signaling_NaN = false;
  static constexpr float_denorm_style has_denorm = denorm_absent;
  static constexpr bool has_denorm_loss = false;
  static constexpr T infinity() noexcept { return T(); }
  static constexpr T quiet_NaN() noexcept { return T(); }
  static constexpr T signaling_NaN() noexcept { return T(); }
  static constexpr T denorm_min() noexcept { return min(); }

  static constexpr bool is_iec559 = false;
  static constexpr bool is_bounded = true;
  static constexpr bool is_modulo = false;

  static constexpr bool traps = false;
  static constexpr bool tinyness_before = false;
  static constexpr float_round_style round_style = round_toward_zero;
};
  
} // namespace std

