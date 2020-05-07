#include "tvm.h"

unsigned tonstdlib_log_8 (unsigned value) {
    unsigned result = 0;
    while (value > 0) {
        result ++;
        value = value / 256;
    }
    return result;
}

unsigned tonstdlib_ubytesize (unsigned value) {
  return (__builtin_tvm_ubitsize(value) + 7) >> 3;
}

void Serialize_Unsigned_Impl (unsigned width, unsigned val) {
	tonstdlib_work_slice_store_uint(width,val);
}

void Serialize_Signed_Impl (unsigned width, signed val) {
	tonstdlib_work_slice_store_int(width,val);
}

void Serialize_Unsigned (unsigned width, unsigned val) {
	tonstdlib_create_empty_work_slice();
	Serialize_Unsigned_Impl (width, val);
}

void Serialize_Signed (unsigned width, signed val) {
	tonstdlib_create_empty_work_slice();
	Serialize_Signed_Impl (width, val);
}

unsigned Deserialize_Unsigned_Impl (unsigned width) {
	return tonstdlib_work_slice_load_uint(width);
}

int Deserialize_Signed_Impl (unsigned width) {
	return tonstdlib_work_slice_load_int(width);
}

unsigned Deserialize_Unsigned (unsigned width) {
	return Deserialize_Unsigned_Impl (width);
}

int Deserialize_Signed (unsigned width) {
	return Deserialize_Signed_Impl (width);
}

struct __attribute__((tvm_tuple)) S { int v; __tvm_slice s; };

__tvm_slice __tvm_ldu(__tvm_slice slice, int width, int *value) {
  struct S s = __builtin_tvm_ldu(slice, width);
  *value = s.v;
  return s.s;
}

__tvm_slice __tvm_ldi(__tvm_slice slice, int width, int *value) {
  struct S s = __builtin_tvm_ldi(slice, width);
  *value = s.v;
  return s.s;
}

Cell Deserialize_Cell() {
  int s = __builtin_tvm_getglobal(3);
  struct __attribute__((tvm_tuple)) { __tvm_cell c; __tvm_slice s; } st =
    __builtin_tvm_ldref(__builtin_tvm_cast_to_slice(s));
  __builtin_tvm_setglobal(3, __builtin_tvm_cast_from_slice(st.s));
  return st.c;
}

void tvm_accept(void) {
  __builtin_tvm_accept();
}
