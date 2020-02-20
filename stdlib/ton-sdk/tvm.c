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

__tvm_builder tvm_serialize_unsigned (__tvm_builder builder, unsigned value, unsigned size) {
  return __builtin_tvm_stu(value, builder, size);
}

__tvm_builder tvm_serialize_int (__tvm_builder builder, int value, unsigned size) {
  return __builtin_tvm_sti(value, builder, size);
}

unsigned tvm_deserialize_unsigned (__tvm_slice *slice, unsigned size) {
  unsigned value;
  *slice = __tvm_ldu(*slice, size, &value);
  return value;
}

int tvm_deserialize_int (__tvm_slice *slice, unsigned size) {
  int value;
  *slice = __tvm_ldi(*slice, size, &value);
  return value;
}

struct __attribute__((tvm_tuple)) S { int v; __tvm_slice s; };

__tvm_slice __tvm_ldu(__tvm_slice slice, int width, unsigned *value) {
  struct S s = __builtin_tvm_ldu(slice, width);
  *value = s.v;
  return s.s;
}

__tvm_slice __tvm_ldi(__tvm_slice slice, int width, int *value) {
  struct S s = __builtin_tvm_ldi(slice, width);
  *value = s.v;
  return s.s;
}

Cell tvm_deserialize_Cell() {
  int s = __builtin_tvm_getglobal(3);
  struct __attribute__((tvm_tuple)) { __tvm_cell c; __tvm_slice s; } st =
    __builtin_tvm_ldref(__builtin_tvm_cast_to_slice(s));
  __builtin_tvm_setglobal(3, __builtin_tvm_cast_from_slice(st.s));
  return st.c;
}

void tvm_accept(void) {
  __builtin_tvm_accept();
}
