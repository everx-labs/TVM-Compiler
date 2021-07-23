#pragma once

#include <boost/hana/string.hpp>
#include <boost/hana/value.hpp>
#include <boost/hana/power.hpp>
#include <boost/hana/div.hpp>

namespace tvm {

namespace hana = boost::hana;

template<unsigned radix>
constexpr size_t get_magnitude(size_t num) {
  unsigned i = 0;
  while (num > 0) {
    num /= radix;
    ++i;
  }
  return i;
}

constexpr char hex_sym(unsigned num) {
  constexpr char arr[] = "0123456789abcdef";
  return arr[std::min(num, 16u)];
}

template <unsigned radix, class X, size_t ...i>
constexpr auto to_string(X x, std::index_sequence<i...>) {
  constexpr size_t mag = get_magnitude<radix>(X::value);
  return hana::string<
    hana::size_c<hex_sym(
      hana::value(x / hana::power(hana::size_c<radix>, hana::size_c<mag - i - 1>)
                    % hana::size_c<radix>))
      >...>{};
}

template <unsigned radix, class X>
constexpr auto to_string(X) {
  using namespace hana::literals;
  if constexpr (X::value == 0)
    return "0"_s;
  else
    return to_string<radix>(hana::size_c<static_cast<size_t>(X::value)>,
                            std::make_index_sequence<get_magnitude<radix>(X::value)>());
}

} // namespace tvm

