#pragma once

#include <tvm/is_tuple.hpp>
#include <tvm/schema/chain_tuple.hpp>
#include <tvm/schema/chain_fold.hpp>

namespace tvm { inline namespace schema {

template<class Data, unsigned BitsOffset = 0, unsigned RefsOffset = 0>
cell build_chain_static(Data val) {
  if constexpr (is_tuple<Data>()) {
    return build(make_chain_tuple<BitsOffset, RefsOffset>(val)).make_cell();
  } else {
    return build(make_chain_tuple<BitsOffset, RefsOffset>(to_std_tuple(val))).make_cell();
  }
}

}} // namespace tvm::schema

