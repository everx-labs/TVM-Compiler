#ifndef TVM_TYPES_H
#define TVM_TYPES_H

#define JOIN(a,b) a##b
#define JOIN3(a,b,c) a##b##c
#define XJOIN(a,b) JOIN(a,b)
#define XJOIN3(a,b,c) JOIN3(a,b,c)

#define TVM_CUSTOM_EXCEPTION(id,val) enum { id = val };

typedef struct Slice {
    int body;
} Slice;

typedef struct Cell {
    int body;
} Cell;

typedef struct CellBuilder {
    int body;
} CellBuilder;

int __builtin_tvm_newc();
int __builtin_tvm_endc(int);
int __builtin_tvm_stu(int,int,int);
int __builtin_tvm_sti(int,int,int);
int __builtin_tvm_ctos(int);
int __builtin_tvm_sendrawmsg(int msg_slice,int flags);
int __builtin_tvm_throwif(int condition,int exc);

#define tvm_assert(condition,exc) (__builtin_tvm_throwif(!(condition),(exc)))

CellBuilder Serialize_Unsigned_Impl (CellBuilder builder, size_t width, unsigned int);
CellBuilder Serialize_Signed_Impl (CellBuilder builder, size_t width, signed int);
Cell Serialize_Unsigned (size_t width, unsigned int);
Cell Serialize_Signed (size_t width, signed int);

unsigned int Deserialize_Unsigned_Impl (Slice*, size_t width);
signed int Deserialize_Signed_Impl (Slice*, size_t width);
unsigned int Deserialize_Unsigned (Cell, size_t width);
signed int Deserialize_Signed (Cell, size_t width);

#endif