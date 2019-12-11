// Auxiliary file, defining three sets of C symbols for a structure:
// - enumeration with fields offsets and lengths
// - serialization methods
// - deserialization methods
//
// Two macroses should be specified to use this file: TON_STRUCT_NAME
// and TON_STRUCT. The structure should be specified in TON_STRUCT macro,
// it may use the following macros:

// Structure declaration
#define FIELD_CONSTANT_UNSIGNED(name, value, size)
#define FIELD_UNSIGNED(name, size) unsigned int name;
#define FIELD_SIGNED(name, size) signed int name;
#define FIELD_VAR_UNSIGNED(name, size) struct { int len; unsigned value; } name;
#define FIELD_VAR_SIGNED(name, size) struct { int len; int value; } name;
#define FIELD_COMPLEX(name, type) type name;
#define FIELD_MAYBE(name, type) struct { int just; type value; } name;
#define FIELD_ARRAY(name, actual_size, max_size, type) type name[max_size];

typedef struct TON_STRUCT_NAME {
    TON_STRUCT
} TON_STRUCT_NAME;

void XJOIN3(Serialize_, TON_STRUCT_NAME, _Impl)(__tvm_builder* builder, TON_STRUCT_NAME* value);
void XJOIN(Serialize_, TON_STRUCT_NAME)(TON_STRUCT_NAME* value);

#undef FIELD_CONSTANT_UNSIGNED
#undef FIELD_UNSIGNED
#undef FIELD_SIGNED
#undef FIELD_VAR_UNSIGNED
#undef FIELD_VAR_SIGNED
#undef FIELD_COMPLEX
#undef FIELD_MAYBE
#undef FIELD_ARRAY

TON_STRUCT_NAME XJOIN3(Deserialize_, TON_STRUCT_NAME, _Impl)(__tvm_slice* slice);
TON_STRUCT_NAME XJOIN(Deserialize_, TON_STRUCT_NAME)();

#undef TON_STRUCT_NAME
#undef TON_STRUCT
