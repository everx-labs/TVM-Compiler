#pragma once

#include <tvm/error_code.hpp>
#include <tvm/parser.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/schema/message.hpp>
#include <tvm/schema/parser/struct_parser.hpp>
#include <tvm/schema/parser/variant_parser.hpp>
#include <tvm/schema/parser/dynamic_field_parser.hpp>

namespace tvm { namespace schema {

template<unsigned _bitlen, unsigned _code>
struct make_parser_impl<bitconst<_bitlen, _code>> {
  using value_type = bitconst<_bitlen, _code>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    if (p.ldu(_bitlen) == _code)
      return std::tuple(value_type{}, p, ctx);
    return std::tuple(optional<value_type>{}, p, ctx);
  }
};

template<unsigned _bitlen>
struct make_parser_impl<bitfield<_bitlen>> {
  using value_type = bitfield<_bitlen>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldslice(_bitlen) };
    return std::tuple(rv, p, ctx);
  }
};

template<unsigned _bitlen>
struct make_parser_impl<uint_t<_bitlen>> {
  using value_type = uint_t<_bitlen>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldu(_bitlen) };
    return std::tuple(rv, p, ctx);
  }
};

template<unsigned _bitlen>
struct make_parser_impl<int_t<_bitlen>> {
  using value_type = int_t<_bitlen>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldi(_bitlen) };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<varuint<16>> {
  using value_type = varuint<16>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldvaruint16() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<varuint<32>> {
  using value_type = varuint<32>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldvaruint32() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<varint<16>> {
  using value_type = varint<16>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldvarint16() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<varint<32>> {
  using value_type = varint<32>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldvarint32() };
    return std::tuple(rv, p, ctx);
  }
};

template<unsigned _keylen, class _element_type>
struct make_parser_impl<HashmapE<_keylen, _element_type>> {
  using value_type = HashmapE<_keylen, _element_type>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.lddict() };
    return std::tuple(rv, p, ctx);
  }
};
template<>
struct make_parser_impl<anydict> {
  using value_type = anydict;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.lddict() };
    return std::tuple(rv, p, ctx);
  }
};

template<class _Tp>
struct make_parser_impl<ref<_Tp>> {
  using value_type = ref<_Tp>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto inner_slice = p.ldrefrtos();
    parser inner_parser(inner_slice);
    auto [rv, =inner_parser, new_ctx] = make_parser_impl<_Tp>::parse(inner_parser, ctx);
    inner_parser.ends();
    return std::tuple(value_type{*rv}, p, new_ctx);
  }
};
template<>
struct make_parser_impl<cell> {
  using value_type = cell;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldref() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<anyval> {
  using value_type = anyval;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    return { anyval{p.get_cur_slice()}, parser(slice::create_empty()), ctx };
  }
};

template<>
struct make_parser_impl<MsgAddressSlice> {
  using value_type = MsgAddressSlice;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldmsgaddr() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<MsgAddress> {
  using value_type = MsgAddress;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto parsed = p.parsemsgaddr();
    auto rv = parsed.unpack();
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<lazy<MsgAddress>> {
  using value_type = lazy<MsgAddress>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldmsgaddr() };
    return std::tuple(rv, p, ctx);
  }
};
template<>
struct make_parser_impl<lazy<MsgAddressInt>> {
  using value_type = lazy<MsgAddressInt>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldmsgaddr() };
    return std::tuple(rv, p, ctx);
  }
};
template<>
struct make_parser_impl<lazy<MsgAddressExt>> {
  using value_type = lazy<MsgAddressExt>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto rv = value_type{ p.ldmsgaddr() };
    return std::tuple(rv, p, ctx);
  }
};

template<>
struct make_parser_impl<empty> {
  using value_type = empty;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    return std::tuple(value_type{}, p, ctx);
  }
};

template<class _Tp>
struct make_parser_impl<optional<_Tp>> {
  using value_type = optional<_Tp>;
  // Tricky situation here, value_type = optional<_Tp>,
  //  ret = optional<optional<_Tp>>
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    if (p.ldu(1)) {
      auto [parsed, new_p, new_ctx] = make_parser_impl<_Tp>::parse(p, ctx);
      if (parsed)
        return std::tuple(parsed, new_p, new_ctx);
      return std::tuple(optional<value_type>{}, p, ctx);
    } else {
      return std::tuple(value_type{}, p, ctx);
    }
    return std::tuple(optional<value_type>{}, p, ctx);
  }
};

template<class _Tp>
using make_parser = make_parser_impl<_Tp>;

// Returns parsed value if succeeded or nullopt if not
template<class _Tp>
inline optional<_Tp> parse_if(parser p) {
  ParseContext<empty, std::tuple<>> ctx;
  auto [rv, new_p, new_ctx] = make_parser<_Tp>::parse(p, ctx);
  return rv;
}

// Returns parsed optional value and new parser (the rest of slice)
template<class _Tp>
inline auto parse_continue(parser p) {
  ParseContext<empty, std::tuple<>> ctx;
  auto [rv, new_p, new_ctx] = make_parser<_Tp>::parse(p, ctx);
  return std::tuple(rv, new_p);
}

template<class _Tp>
inline auto parse(parser p, unsigned err_code = error_code::custom_data_parse_error,
    bool full = false) {
  ParseContext<empty, std::tuple<>> ctx;
  auto [opt_val, new_p, new_ctx] = make_parser<_Tp>::parse(p, ctx);
  if (!opt_val)
      __builtin_tvm_throw(err_code);
  if (full) new_p.ends();
  return *opt_val;
}

template<class _Tp>
inline auto parse(slice sl, unsigned err_code = error_code::custom_data_parse_error,
    bool full = false) {
  return parse<_Tp>(parser(sl));
}

template<typename _Tp>
__always_inline _Tp lazy<_Tp>::operator()() const {
  return val();
}

template<typename _Tp>
__always_inline _Tp lazy<_Tp>::val() const {
  return parse<_Tp>(sl_, error_code::custom_data_parse_error, true);
}

}} // namespace tvm::schema
