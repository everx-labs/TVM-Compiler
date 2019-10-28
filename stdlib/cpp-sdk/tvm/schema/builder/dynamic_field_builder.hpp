#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

template<class _GetField>
struct make_builder_impl<dynamic_bitfield<_GetField>> {
  using value_type = dynamic_bitfield<_GetField>;
  inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<class _GetField>
struct make_builder_impl<dynamic_uint<_GetField>> {
  using value_type = dynamic_uint<_GetField>;
  inline static builder build(builder b, value_type v) {
    return b.stu(v.val_, v.bitlen_);
  }
};

template<class _GetField>
struct make_builder_impl<dynamic_int<_GetField>> {
  using value_type = dynamic_int<_GetField>;
  inline static builder build(builder b, value_type v) {
    return b.sti(v.val_, v.bitlen_);
  }
};

}} // namespace tvm::schema
