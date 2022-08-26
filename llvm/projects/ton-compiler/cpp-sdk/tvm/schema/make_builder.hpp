#pragma once

#include <tvm/builder.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/builder/struct_builder.hpp>
#include <tvm/schema/builder/variant_builder.hpp>
#include <tvm/schema/builder/dynamic_field_builder.hpp>
#include <tvm/schema/message.hpp>
#include <experimental/type_traits>

namespace tvm { namespace schema {

template<unsigned _bitlen, unsigned _code>
struct make_builder_impl<bitconst<_bitlen, _code>> {
  using value_type = bitconst<_bitlen, _code>;
  __always_inline static builder build(builder b, value_type) {
    return b.stu(_code, _bitlen);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<bitfield<_bitlen>> {
  using value_type = bitfield<_bitlen>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<uint_t<_bitlen>> {
  using value_type = uint_t<_bitlen>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stu(v.val_, _bitlen);
  }
};

template<unsigned _bitlen>
struct make_builder_impl<int_t<_bitlen>> {
  using value_type = int_t<_bitlen>;
  __always_inline static builder build(builder b, value_type v) {
    return b.sti(v.val_, _bitlen);
  }
};

template<>
struct make_builder_impl<varuint<16>> {
  using value_type = varuint<16>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stvaruint16(v.val_);
  }
};

template<>
struct make_builder_impl<varuint<32>> {
  using value_type = varuint<32>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stvaruint32(v.val_);
  }
};

template<>
struct make_builder_impl<varint<16>> {
  using value_type = varint<16>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stvarint16(v.val_);
  }
};

template<>
struct make_builder_impl<varint<32>> {
  using value_type = varint<32>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stvarint32(v.val_);
  }
};

template<unsigned _keylen, class _element_type>
struct make_builder_impl<HashmapE<_keylen, _element_type>> {
  using value_type = HashmapE<_keylen, _element_type>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stdict(v.dict_);
  }
};
template<>
struct make_builder_impl<anydict> {
  using value_type = anydict;
  __always_inline static builder build(builder b, value_type v) {
    return b.stdict(v.dict_);
  }
};

template<class _Tp>
struct make_builder_impl<ref<_Tp>> {
  using value_type = ref<_Tp>;
  __always_inline static builder build(builder b, value_type v) {
    tvm::builder inner_builder;
    inner_builder = make_builder_impl<_Tp>::build(inner_builder, v.val_);
    return b.stref(inner_builder.endc());
  }
};
template<>
struct make_builder_impl<cell> {
  using value_type = cell;
  __always_inline static builder build(builder b, value_type v) {
    return b.stref(v);
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
  __always_inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl_);
  }
};

template<class _Tp>
struct make_builder_impl<lazy<_Tp>> {
  using value_type = lazy<_Tp>;
  __always_inline static builder build(builder b, value_type v) {
    return b.stslice(v.sl());
  }
};

template<>
struct make_builder_impl<empty> {
  using value_type = empty;
  __always_inline static builder build(builder b, value_type) {
    return b;
  }
};

template<class _Tp>
struct make_builder_impl<optional<_Tp>> {
  using value_type = optional<_Tp>;
  __always_inline static builder build(builder b, value_type v) {
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
__always_inline builder build(builder b, _Tp val) {
  return make_builder<_Tp>::build(b, val);
}

template<class _Tp>
__always_inline builder build(_Tp val) {
  return make_builder<_Tp>::build(builder(), val);
}

template<typename _Tp>
__always_inline lazy<_Tp>::lazy(_Tp val) {
  sl_ = build(val).make_slice();
}

template<typename _Tp>
__always_inline void lazy<_Tp>::operator=(_Tp val) {
  sl_ = build(val).make_slice();
}

template<typename _Tp>
__always_inline
lazy<_Tp> lazy<_Tp>::make_std(int8 workchain, uint256 addr) {
  lazy<_Tp> rv{ MsgAddressInt{ addr_std{ {}, {}, int8{workchain}, addr } } };
  return rv;
}

}} // namespace tvm::schema

