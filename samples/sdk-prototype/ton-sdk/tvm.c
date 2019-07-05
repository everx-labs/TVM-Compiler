#include "tvm.h"

unsigned tonstdlib_log_8 (unsigned value) {
#if 0
// TODO: fix problems in complex if!
    if (value >= 65536) return 3;
    else if (value >= 256) return 2;
    else if (value >= 1) return 1;
    else 
    if (value >= 1) return 2;
    return 0;
#else
    return (!!value) + (!!(value >> 8)) + (!!(value >> 16)) + (!!(value >> 24)) + (!!(value >> 32));
#endif
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
