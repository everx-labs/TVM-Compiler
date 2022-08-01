#pragma once

#include <tvm/is_tuple.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/schema/chain_fold.hpp>

namespace tvm { inline namespace schema {

template<class Data, unsigned BitsOffset = 0, unsigned RefsOffset = 0>
auto parse_chain_static(parser p) {
  if constexpr (is_tuple<Data>()) {
    using data_tup_t = Data;
    using LinearTup = decltype(make_chain_tuple<BitsOffset, RefsOffset>(data_tup_t{}));
    auto linear_tup = parse<LinearTup>(p);
    return chain_fold_tup<data_tup_t>(linear_tup);
  } else {
    using data_tup_t = to_std_tuple_t<Data>;
    using LinearTup = decltype(make_chain_tuple<BitsOffset, RefsOffset>(data_tup_t{}));
    auto linear_tup = parse<LinearTup>(p);
    return to_struct<Data>(chain_fold_tup<data_tup_t>(linear_tup));
  }
}

}} // namespace tvm::schema

