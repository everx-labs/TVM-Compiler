#pragma once

#include <tvm/parser.hpp>
#include <tvm/schema/basics.hpp>

// Parsing of variant

namespace tvm { namespace schema {

template<class _ValueT, class _Ctx, class _Tp, class... _RestTypes>
struct variant_parser {
  inline static std::tuple<optional<_ValueT>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    parser copy_parser(p);
    auto [opt_cur, new_p, new_ctx] = make_parser_impl<_Tp>::parse(p, ctx);
    if (opt_cur)
      return std::tuple(_ValueT(*opt_cur), new_p, new_ctx);
    return variant_parser<_ValueT, _Ctx, _RestTypes...>::parse(copy_parser, ctx);
  }
};

template<class _ValueT, class _Ctx, class _Tp>
struct variant_parser<_ValueT, _Ctx, _Tp> {
  inline static std::tuple<optional<_ValueT>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto [opt_cur, new_p, new_ctx] = make_parser_impl<_Tp>::parse(p, ctx);
    if (opt_cur)
      return std::tuple(_ValueT(*opt_cur), new_p, new_ctx);
    return std::tuple(optional<_ValueT>{}, new_p, new_ctx);
  }
};

template<class... Types>
struct make_parser_impl<std::variant<Types...>> {
  using value_type = std::variant<Types...>;

  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    return variant_parser<value_type, _Ctx, Types...>::parse(p, ctx);
  }
};

template<class X, class Y>
struct make_parser_impl<Either<X, Y>> {
  using value_type = Either<X, Y>;
  using base_t = typename value_type::base_t;

  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto [opt_rv, new_p, new_ctx] = make_parser_impl<base_t>::parse(p, ctx);
    if (opt_rv) {
      value_type rv;
      rv.val_ = *opt_rv;
      return { rv, new_p, new_ctx };
    }
    return { {}, p, ctx };
  }
};

}} // namespace tvm::schema
