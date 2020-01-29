#pragma once

#include <tvm/parser.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/to_struct.hpp>

// Parsing of dynamic_bitfield

namespace tvm { namespace schema {

template<auto _Field, class _ParentStruct, class _ParentTuple>
__attribute__((alwaysinline))
unsigned get_dynfield_size(ParseContext<_ParentStruct, _ParentTuple> ctx) {
  // To get len from already processed field,
  //  we copy current tuple to struct and access its field
  auto s = to_struct<_ParentStruct>(ctx.get_tuple());
  auto field_size = (&s)->*_Field;
  return field_size();
}

template<auto _Field>
struct make_parser_impl<dynamic_bitfield<_Field>> {
  using value_type = dynamic_bitfield<_Field>;

  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_Field>(ctx);
    auto rv = value_type{ p.ldslice(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

template<auto _Field>
struct make_parser_impl<dynamic_uint<_Field>> {
  using value_type = dynamic_uint<_Field>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_Field>(ctx);
    auto rv = value_type{ p.ldu(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

template<auto _Field>
struct make_parser_impl<dynamic_int<_Field>> {
  using value_type = dynamic_int<_Field>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_Field>(ctx);
    auto rv = value_type{ p.ldi(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

}} // namespace tvm::schema
