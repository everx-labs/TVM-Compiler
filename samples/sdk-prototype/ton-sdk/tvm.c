#include "tvm.h"

CellBuilder Serialize_Unsigned_Impl (CellBuilder builder, size_t width, unsigned val) {
	builder.body = __builtin_tvm_stu(builder.body,width,val);
	return builder;
}

CellBuilder Serialize_Signed_Impl (CellBuilder builder, size_t width, signed val) {
	builder.body = __builtin_tvm_sti(builder.body,width,val);
	return builder;
}

Cell Serialize_Unsigned (size_t width, unsigned val) {
	CellBuilder builder;
	Cell res;

	builder.body = __builtin_tvm_newc();
	Serialize_Unsigned_Impl (builder, width, val);
	res.body = __builtin_tvm_endc(builder.body);
	return res;
}

Cell Serialize_Signed (size_t width, signed val) {
	CellBuilder builder;
	Cell res;

	builder.body = __builtin_tvm_newc();
	Serialize_Signed_Impl (builder, width, val);
	res.body = __builtin_tvm_endc(builder.body);
	return res;
}
