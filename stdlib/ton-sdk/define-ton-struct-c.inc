// Auxiliary file, defining three sets of C symbols for a structure:
// - enumeration with fields offsets and lengths
// - serialization methods
// - deserialization methods
//
// Two macroses should be specified to use this file: TON_STRUCT_NAME
// and TON_STRUCT. The structure should be specified in TON_STRUCT macro, 
// it may use the following macros: 

// Structure serialization
#define FIELD_CONSTANT_UNSIGNED(name, value, size) Serialize_Unsigned_Impl (value, size);
#define FIELD_UNSIGNED(name, size) Serialize_Unsigned_Impl (value->name, size);
#define FIELD_SIGNED(name, size) Serialize_Signed_Impl (value->name, size);
#define FIELD_VAR_UNSIGNED(name, size) { Serialize_Unsigned_Impl (value->name.len, size); \
    Serialize_Unsigned_Impl (value->name.value, value->name.len*8); }
#define FIELD_VAR_SIGNED(name, size) { Serialize_Signed_Impl (value->name.len, size); \
    Serialize_Signed_Impl (value->name.value, value->name.len*8); }
#define FIELD_COMPLEX(name, type) Serialize_##type##_Impl (&(value->name));
#define FIELD_MAYBE(name, type) { if (value->name.just) { \
    Serialize_Unsigned_Impl (1, 1); /* serialize Just */ \
    Serialize_##type##_Impl (&(value->name.value)); \
} else { Serialize_Unsigned_Impl (0, 1); }}
#define FIELD_ARRAY(name, actual_size, max_size, type) //{ for (int _count = 0; _count < value->actual_size; _count++) \
//    Serialize_##type##_Impl (&(value->name[_count])); }


void XJOIN3(Serialize_,TON_STRUCT_NAME,_Impl) (TON_STRUCT_NAME* value) {
    TON_STRUCT
}

void XJOIN(Serialize_,TON_STRUCT_NAME) (TON_STRUCT_NAME* value) {
    tonstdlib_create_empty_work_slice();
    XJOIN3(Serialize_,TON_STRUCT_NAME,_Impl) (value);
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
#define FIELD_CONSTANT_UNSIGNED(name, value, size) tvm_assert (Deserialize_Unsigned_Impl (size) == value, 59);
#define FIELD_UNSIGNED(name, size) res.name = Deserialize_Unsigned_Impl (size);
#define FIELD_SIGNED(name, size) res.name = Deserialize_Signed_Impl (size);
#define FIELD_VAR_UNSIGNED(name, size) { res.name.len = Deserialize_Unsigned_Impl (size); \
    res.name.value = Deserialize_Unsigned_Impl (res.name.len*8); }
#define FIELD_VAR_SIGNED(name, size) { res.name.len = Deserialize_Unsigned_Impl (size); \
    res.name.value = Deserialize_Signed_Impl (res.name.len*8); }
#define FIELD_COMPLEX(name, type) res.name = Deserialize_##type##_Impl ();
#define FIELD_MAYBE(name, type) res.name.just = Deserialize_Unsigned_Impl (1); \
    if (res.name.just) { res.name.value = Deserialize_##type##_Impl (); }
#define FIELD_ARRAY(name, actual_size, max_size, type) // { for (int _count = 0; _count < res.actual_size; _count++) \
//    res.name[_count] = Deserialize_##type##_Impl (); }

TON_STRUCT_NAME XJOIN3(Deserialize_,TON_STRUCT_NAME,_Impl) () {
    TON_STRUCT_NAME res;
    TON_STRUCT
    return res;
}

TON_STRUCT_NAME XJOIN(Deserialize_,TON_STRUCT_NAME) () {
    TON_STRUCT_NAME s = XJOIN3(Deserialize_,TON_STRUCT_NAME,_Impl) ();
    tonstdlib_create_empty_work_slice();
    return s;
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
