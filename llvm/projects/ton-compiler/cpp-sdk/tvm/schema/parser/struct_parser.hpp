#pragma once

#include <tvm/to_std_tuple.hpp>
#include <tvm/to_struct.hpp>
#include <tvm/parser.hpp>
#include <tvm/schema/basics.hpp>

// Parsing of struct/class and std tuples

namespace tvm { namespace schema {

template<class _Tp>
struct make_parser_impl {
  static_assert(std::is_class_v<_Tp>);

  using fmt_tuple = to_std_tuple_t<_Tp>;
  using value_type = _Tp;

  template<class _Ctx>
  inline static std::tuple<optional<_Tp>, parser, _Ctx> parse(parser p, _Ctx parent_ctx) {
    ParseContext<_Tp, fmt_tuple> ctx;
    auto [tup, new_p, new_ctx] = make_parser_impl<fmt_tuple>::parse_from_struct(p, ctx);
    if (tup)
      return std::tuple(to_struct<value_type>(*tup), new_p, parent_ctx);
    return std::tuple(optional<_Tp>{}, new_p, parent_ctx);
  }
};

template<class _Ctx, size_t Index, class _Tp, class... _RestTypes>
struct sequence_parser {
  inline static std::tuple<bool, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto [opt_cur, new_p, new_ctx] = make_parser_impl<_Tp>::parse(p, ctx);
    p = new_p;
    if (opt_cur) {
      std::get<Index>(ctx) = *opt_cur;
      return sequence_parser<_Ctx, Index + 1, _RestTypes...>::parse(p, ctx);
    }
    return std::tuple(false, p, ctx);
  }
};

template<class _Ctx, size_t Index, class _Tp>
struct sequence_parser<_Ctx, Index, _Tp> {
  inline static std::tuple<bool, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto [opt_cur, new_p, new_ctx] = make_parser_impl<_Tp>::parse(p, ctx);
    p = new_p;
    if (opt_cur) {
      std::get<Index>(ctx) = *opt_cur;
      return std::tuple(true, p, ctx);
    }
    return std::tuple(false, p, ctx);
  }
};

template<class... Types>
struct make_parser_impl<std::tuple<Types...>> {
  using value_type = std::tuple<Types...>;

  // For parsing structs
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse_from_struct(parser p, _Ctx ctx) {
    auto [succ, new_p, new_ctx] = sequence_parser<_Ctx, 0, Types...>::parse(p, ctx);
    if (succ)
      return std::tuple(new_ctx.get_tuple(), new_p, new_ctx);
    return std::tuple(optional<value_type>{}, p, ctx);
  }

  // For parsing std::tuple fields
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx parent_ctx) {
    if constexpr (std::tuple_size_v<value_type> == 0) {
      return { value_type{}, p, parent_ctx };
    } else {
      using CtxT = ParseContext<empty, value_type>;
      CtxT ctx;
      auto [succ, new_p, new_ctx] = sequence_parser<CtxT, 0, Types...>::parse(p, ctx);
      if (succ)
        return std::tuple(new_ctx.get_tuple(), new_p, parent_ctx);
      return std::tuple(optional<value_type>{}, p, parent_ctx);
    }
  }
};

}} // namespace tvm::schema
