// Auxiliary file, defining three sets of C symbols for a structure:
// - enumeration with fields offsets and lengths
// - serialization methods
// - deserialization methods
//
// Two macroses should be specified to use this file: TON_STRUCT_NAME
// and TON_STRUCT. The structure should be specified in TON_STRUCT macro,
// it may use the following macros:

// Structure serialization
#define FIELD_CONSTANT_UNSIGNED(name, value, size) { builder = __builtin_tvm_stu(value, builder, size); }
#define FIELD_UNSIGNED(name, size)                 { builder = __builtin_tvm_stu(value->name, builder, size); }
#define FIELD_SIGNED(name, size)                   { builder = __builtin_tvm_sti(value->name, builder, size); }
#define FIELD_VAR_UNSIGNED(name, size)             { builder = __builtin_tvm_stu(value->name.len, builder, size); \
                                                     builder = __builtin_tvm_stu(value->name.value, builder, value->name.len * 8); }
#define FIELD_VAR_SIGNED(name, size)               { builder = __builtin_tvm_stu(value->name.len, builder, size); \
                                                     builder = __builtin_tvm_sti(value->name.value, builder, value->name.len * 8); }
#define FIELD_COMPLEX(name, type)                  { builder = tvm_serialize_##type(builder, &(value->name)); }
#define FIELD_MAYBE(name, type)                    { if (value->name.just) { \
                                                       builder = __builtin_tvm_stu(1, builder, 1); \
                                                       builder = tvm_serialize_##type(builder, &(value->name.value)); \
                                                     } else { \
                                                       builder = __builtin_tvm_stu(0, builder, 1); \
                                                     } \
                                                   }
#define FIELD_ARRAY(name, actual_size, max_size, type) { /* TODO */ }

__tvm_builder XJOIN(tvm_serialize_, TON_STRUCT_NAME)(__tvm_builder builder, TON_STRUCT_NAME* value) {
    TON_STRUCT
    return builder;
}

#undef FIELD_CONSTANT_UNSIGNED
#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_VAR_UNSIGNED
#undef FIELD_VAR_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

// Structure deserialization
#define FIELD_CONSTANT_UNSIGNED(name, value, size) { unsigned  v; *slice = __tvm_ldu(*slice, size, &v); tvm_assert (v == value, 59); }
#define FIELD_UNSIGNED(name, size)                 { unsigned  v; *slice = __tvm_ldu(*slice, size, &v); res.name = v; }
#define FIELD_SIGNED(name, size)                   { int  v; *slice = __tvm_ldi(*slice, size, &v); res.name = v; }
#define FIELD_VAR_UNSIGNED(name, size)             { unsigned v1; *slice = __tvm_ldu(*slice, size, &v1); res.name.len = v1; \
                                                     unsigned v2; *slice = __tvm_ldu(*slice, res.name.len * 8, &v2); res.name.value = v2; }
#define FIELD_VAR_SIGNED(name, size)               { int v1; *slice = __tvm_ldu(*slice, size, &v1); res.name.len = v1; \
                                                     int v2; *slice = __tvm_ldi(*slice, res.name.len * 8, &v2); res.name.value = v2; }
#define FIELD_COMPLEX(name, type)                  { res.name = tvm_deserialize_##type(slice); }
#define FIELD_MAYBE(name, type)                    { int v;  *slice = __tvm_ldu(*slice, 1, &v); res.name.just = v; \
                                                     if (res.name.just) res.name.value = tvm_deserialize_##type(slice); }
#define FIELD_ARRAY(name, actual_size, max_size, type) { /* TODO*/ }

TON_STRUCT_NAME XJOIN(tvm_deserialize_, TON_STRUCT_NAME)(__tvm_slice* slice) {
    TON_STRUCT_NAME res;
    TON_STRUCT
    return res;
}

#undef FIELD_CONSTANT_UNSIGNED
#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_VAR_UNSIGNED
#undef FIELD_VAR_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

#undef TON_STRUCT_NAME
#undef TON_STRUCT
