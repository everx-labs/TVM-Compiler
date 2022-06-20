#pragma once

#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

template<auto _Field>
struct make_builder_impl<dynamic_bitfield<_Field>> {
  using value_type = dynamic_bitfield<_Field>;
  inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<auto _Field>
struct make_builder_impl<dynamic_uint<_Field>> {
  using value_type = dynamic_uint<_Field>;
  inline static builder build(builder b, value_type v) {
    return b.stu(v.val_, v.bitlen_);
  }
};

template<auto _Field>
struct make_builder_impl<dynamic_int<_Field>> {
  using value_type = dynamic_int<_Field>;
  inline static builder build(builder b, value_type v) {
    return b.sti(v.val_, v.bitlen_);
  }
};

}} // namespace tvm::schema
