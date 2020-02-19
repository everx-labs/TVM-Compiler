#ifndef TVM_TYPES_H
#define TVM_TYPES_H

#define JOIN(a,b) a##b
#define JOIN3(a,b,c) a##b##c
#define XJOIN(a,b) JOIN(a,b)
#define XJOIN3(a,b,c) JOIN3(a,b,c)

#define TVM_CUSTOM_EXCEPTION(id,val) enum { id = val };

typedef __tvm_cell Cell;

unsigned tonstdlib_log_8 (unsigned value);
unsigned tonstdlib_ubytesize (unsigned value);

void tonstdlib_create_empty_work_slice (void);
int tonstdlib_work_slice_load_int (unsigned width);
unsigned tonstdlib_work_slice_load_uint (unsigned width);
void tonstdlib_work_slice_store_int (int value, unsigned width);
void tonstdlib_work_slice_store_uint (unsigned value, unsigned width);
void tonstdlib_send_work_slice_as_rawmsg (int flags);
unsigned tvm_sender_pubkey (void);
void tvm_accept(void);

#define tvm_assert(condition,exc) if (!(condition)) __builtin_tvm_throw((exc))

void Serialize_Unsigned_Impl (unsigned width, unsigned int);
void Serialize_Signed_Impl (unsigned width, signed int);
void Serialize_Unsigned (unsigned width, unsigned int);
void Serialize_Signed (unsigned width, signed int);

unsigned int Deserialize_Unsigned_Impl (unsigned width);
signed int Deserialize_Signed_Impl (unsigned width);
unsigned int Deserialize_Unsigned (unsigned width);
signed int Deserialize_Signed (unsigned width);
Cell Deserialize_Cell ();

__tvm_slice __tvm_ldu(__tvm_slice slice, int width, int *value);
__tvm_slice __tvm_ldi(__tvm_slice slice, int width, int *value);

#endif
