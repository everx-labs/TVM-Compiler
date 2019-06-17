// Auxiliary file, defining three sets of C symbols for a structure:
// - enumeration with fields offsets and lengths
// - serialization methods
// - deserialization methods
//
// Two macroses should be specified to use this file: TON_STRUCT_NAME
// and TON_STRUCT. The structure should be specified in TON_STRUCT macro, 
// it may use the following macros: 

// Structure declaration
#define FIELD_UNSIGNED(name, size) unsigned int name;
#define FIELD_SIGNED(name, size) signed int name;
#define FIELD_COMPLEX(name, type) type name;
#define FIELD_MAYBE(name, type) struct { int just; type value; } name;
#define FIELD_ARRAY(name, maxsize, type) type name[maxsize];

typedef struct TON_STRUCT_NAME {
    TON_STRUCT
} TON_STRUCT_NAME;

#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

// Structure serialization
#define FIELD_UNSIGNED(name, size) builder = Serialize_Unsigned_Impl (builder, value->name, size);
#define FIELD_SIGNED(name, size) builder = Serialize_Signed_Impl (builder, value->name, size);
#define FIELD_COMPLEX(name, type) builder = Serialize_##type##_Impl (builder, &(value->name));
#define FIELD_MAYBE(name, type) { if (value->name.just) { \
    builder = Serialize_Unsigned_Impl (builder, 1, 1); /* serialize Just */ \
    builder = Serialize_##type##_Impl (builder, &(value->name.value)); \
} else { builder = Serialize_Unsigned_Impl (builder, 0, 1); }
#define FIELD_ARRAY(name, maxsize, type) { for (int _count = 0; _count < value->name.count; _count++) \
    builder = Serialize_##type##_Impl (builder, &(value->name.value[_count])); }


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
#define FIELD_UNSIGNED(name, size) Deserialize_Unsigned ();
#define FIELD_COMPLEX(name, type) Deserialize_##type ();

#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

#undef TON_STRUCT_NAME
#undef TON_STRUCT
