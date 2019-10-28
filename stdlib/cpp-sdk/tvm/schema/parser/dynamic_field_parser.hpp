#pragma once

#include <tvm/parser.hpp>
#include <tvm/schema/basics.hpp>
#include <tvm/to_struct.hpp>

// Parsing of dynamic_bitfield

namespace tvm { namespace schema {

template<class _GetField, class _ParentStruct, class _ParentTuple>
__attribute__((alwaysinline))
unsigned get_dynfield_size(ParseContext<_ParentStruct, _ParentTuple> ctx) {
  // To get len from already processed field,
  //  we copy current tuple to struct and access its field
  auto s = to_struct<_ParentStruct>(ctx.get_tuple());
  constexpr auto FieldPtr = _GetField{}();
  auto field_size = (&s)->*FieldPtr;
  return field_size();
}

template<class _GetField>
struct make_parser_impl<dynamic_bitfield<_GetField>> {
  using value_type = dynamic_bitfield<_GetField>;

  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_GetField>(ctx);
    auto rv = value_type{ p.ldslice(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

template<class _GetField>
struct make_parser_impl<dynamic_uint<_GetField>> {
  using value_type = dynamic_uint<_GetField>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_GetField>(ctx);
    auto rv = value_type{ p.ldu(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

template<class _GetField>
struct make_parser_impl<dynamic_int<_GetField>> {
  using value_type = dynamic_int<_GetField>;
  template<class _Ctx>
  inline static std::tuple<optional<value_type>, parser, _Ctx> parse(parser p, _Ctx ctx) {
    auto bitlen = get_dynfield_size<_GetField>(ctx);
    auto rv = value_type{ p.ldi(bitlen), bitlen };
    return std::tuple(rv, p, ctx);
  }
};

}} // namespace tvm::schema
