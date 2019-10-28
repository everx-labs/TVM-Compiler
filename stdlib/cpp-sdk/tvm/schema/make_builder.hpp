#pragma once

#include <tvm/builder.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/builder/struct_builder.hpp>
#include <tvm/schema/builder/variant_builder.hpp>
#include <tvm/schema/builder/dynamic_field_builder.hpp>

namespace tvm { namespace schema {

template<unsigned _bitlen, unsigned _code>
struct make_builder_impl<bitconst<_bitlen, _code>> {
  using value_type = bitconst<_bitlen, _code>;
  inline static builder build(builder b, value_type) {
    return b.stu(_code, _bitlen);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<bitfield<_bitlen>> {
  using value_type = bitfield<_bitlen>;
  inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<uint_t<_bitlen>> {
  using value_type = uint_t<_bitlen>;
  inline static builder build(builder b, value_type v) {
    return b.stu(v.val_, _bitlen);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<int_t<_bitlen>> {
  using value_type = int_t<_bitlen>;
  inline static builder build(builder b, value_type v) {
    return b.sti(v.val_, _bitlen);
  }
};

template<>
struct make_builder_impl<varuint<16>> {
  using value_type = varuint<16>;
  inline static builder build(builder b, value_type v) {
    return b.stvaruint16(v.val_);
  }
};

template<>
struct make_builder_impl<varuint<32>> {
  using value_type = varuint<32>;
  inline static builder build(builder b, value_type v) {
    return b.stvaruint32(v.val_);
  }
};

template<>
struct make_builder_impl<varint<16>> {
  using value_type = varint<16>;
  inline static builder build(builder b, value_type v) {
    return b.stvarint16(v.val_);
  }
};

template<>
struct make_builder_impl<varint<32>> {
  using value_type = varint<32>;
  inline static builder build(builder b, value_type v) {
    return b.stvarint32(v.val_);
  }
};

template<unsigned _keylen, class _element_type>
struct make_builder_impl<HashmapE<_keylen, _element_type>> {
  using value_type = HashmapE<_keylen, _element_type>;
  inline static builder build(builder b, value_type v) {
    return b.stdict(v.dict_);
  }
};

template<class _Tp>
struct make_builder_impl<ref<_Tp>> {
  using value_type = ref<_Tp>;
  inline static builder build(builder b, value_type v) {
    tvm::builder inner_builder;
    inner_builder = make_builder_impl<_Tp>::build(inner_builder, v.val_);
    return b.stref(inner_builder.endc());
  }
};
template<>
struct make_builder_impl<anyref> {
  using value_type = anyref;
  inline static builder build(builder b, value_type v) {
    return b.stref(v.val_);
  }
};
template<>
struct make_builder_impl<anyval> {
  using value_type = anyval;
  inline static builder build(builder b, value_type v) {
    return b.stslice(v.val_);
  }
};

template<>
struct make_builder_impl<MsgAddressSlice> {
  using value_type = MsgAddressSlice;
  inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<>
struct make_builder_impl<empty> {
  using value_type = empty;
  inline static builder build(builder b, value_type) {
    return b;
  }
};

template<class _Tp>
struct make_builder_impl<optional<_Tp>> {
  using value_type = optional<_Tp>;
  inline static builder build(builder b, value_type v) {
    if (v) {
      b.stu(1, 1);
      return make_builder_impl<_Tp>::build(b, *v);
    } else {
      return b.stu(0, 1);
    }
  }
};

template<class _Tp>
using make_builder = make_builder_impl<_Tp>;

template<class _Tp>
inline builder build(builder b, _Tp val) {
  return make_builder<_Tp>::build(b, val);
}

template<class _Tp>
inline builder build(_Tp val) {
  return make_builder<_Tp>::build(builder(), val);
}

}} // namespace tvm::schema
