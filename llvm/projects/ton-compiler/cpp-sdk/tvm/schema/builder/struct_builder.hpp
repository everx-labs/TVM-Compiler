#pragma once

#include <tvm/to_std_tuple.hpp>
#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

template<class _Tp>
struct make_builder_impl {
  static_assert(std::is_class_v<_Tp>);

  using fmt_tuple = to_std_tuple_t<_Tp>;
  using value_type = _Tp;

  __always_inline static builder build(builder b, value_type v) {
    return make_builder_impl<fmt_tuple>::build(b, to_std_tuple(v));
  }
};

template<class _ValueT, size_t Index, class _Tp, class... _RestTypes>
struct sequence_builder {
  __always_inline static builder build(builder b, _ValueT v) {
    b = make_builder_impl<_Tp>::build(b, std::get<Index>(v));
    return sequence_builder<_ValueT, Index + 1, _RestTypes...>::build(b, v);
  }
};

template<class _ValueT, size_t Index, class _Tp>
struct sequence_builder<_ValueT, Index, _Tp> {
  __always_inline static builder build(builder b, _ValueT v) {
    return make_builder_impl<_Tp>::build(b, std::get<Index>(v));
  }
};

template<class... Types>
struct make_builder_impl<std::tuple<Types...>> {
  using value_type = std::tuple<Types...>;
  __always_inline static builder build(builder b, value_type v) {
    return sequence_builder<value_type, 0, Types...>::build(b, v);
  }
};

}} // namespace tvm::schema
