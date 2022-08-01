#pragma once

#include <tvm/builder.hpp>
#include <tvm/schema/basics.hpp>

namespace tvm { namespace schema {

struct builder_visitor {
  template<class T>
  tvm::builder operator()(T v) const {
    return make_builder_impl<T>::build(b, v);
  }
  tvm::builder b;
};

template<class... Types>
struct make_builder_impl<std::variant<Types...>> {
  using value_type = std::variant<Types...>;

  static builder build(builder b, value_type v) {
    return std::visit(builder_visitor{ b }, v);
  }
};

}} // namespace tvm::schema
