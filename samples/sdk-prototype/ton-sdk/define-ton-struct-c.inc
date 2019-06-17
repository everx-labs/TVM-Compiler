// Auxiliary file, defining three sets of C symbols for a structure:
// - enumeration with fields offsets and lengths
// - serialization methods
// - deserialization methods
//
// Two macroses should be specified to use this file: TON_STRUCT_NAME
// and TON_STRUCT. The structure should be specified in TON_STRUCT macro, 
// it may use the following macros: 

// Structure serialization
#define FIELD_UNSIGNED(name, size) builder = Serialize_Unsigned_Impl (builder, value->name, size);
#define FIELD_SIGNED(name, size) builder = Serialize_Signed_Impl (builder, value->name, size);
#define FIELD_COMPLEX(name, type) builder = Serialize_##type##_Impl (builder, &(value->name));
#define FIELD_MAYBE(name, type) { if (value->name.just) { \
    builder = Serialize_Unsigned_Impl (builder, 1, 1); /* serialize Just */ \
    builder = Serialize_##type##_Impl (builder, &(value->name.value)); \
} else { builder = Serialize_Unsigned_Impl (builder, 0, 1); }}
#define FIELD_ARRAY(name, actual_size, max_size, type) { for (int _count = 0; _count < value->actual_size; _count++) \
    builder = Serialize_##type##_Impl (builder, &(value->name[_count])); }


CellBuilder XJOIN3(Serialize_,TON_STRUCT_NAME,_Impl) (CellBuilder builder, TON_STRUCT_NAME* value) {
    TON_STRUCT
    return builder;
}

Cell XJOIN(Serialize_,TON_STRUCT_NAME) (TON_STRUCT_NAME* value) {
    Cell res;
    CellBuilder builder; builder.body = __builtin_tvm_newc();
    builder = XJOIN3(Serialize_,TON_STRUCT_NAME,_Impl) (builder, value);
    res.body = __builtin_tvm_endc(builder.body);
    return res;
}

#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

// Structure deserialization
#define FIELD_UNSIGNED(name, size) res.name = Deserialize_Unsigned_Impl (slice, size);
#define FIELD_SIGNED(name, size) res.name = Deserialize_Signed_Impl (slice, size);
#define FIELD_COMPLEX(name, type) res.name = Deserialize_##type##_Impl (slice);
#define FIELD_MAYBE(name, type) res.name.just = Deserialize_Unsigned_Impl (slice, 1); \
    if (res.name.just) { res.name.value = Deserialize_##type##_Impl (slice); }
#define FIELD_ARRAY(name, actual_size, max_size, type) { for (int _count = 0; _count < res.actual_size; _count++) \
    res.name[_count] = Deserialize_##type##_Impl (slice); }

TON_STRUCT_NAME XJOIN3(Deserialize_,TON_STRUCT_NAME,_Impl) (Slice* slice) {
    TON_STRUCT_NAME res;
    TON_STRUCT
    return res;
}

TON_STRUCT_NAME XJOIN(Deserialize_,TON_STRUCT_NAME) (Cell cell) {
    Slice slice; slice.body = __builtin_tvm_ctos (cell.body);
    return XJOIN3(Deserialize_,TON_STRUCT_NAME,_Impl) (&slice);
}

#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

#undef TON_STRUCT_NAME
#undef TON_STRUCT
