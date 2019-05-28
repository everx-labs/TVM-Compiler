typedef long long int64_t;


#define EXCEPT_NOT_FOUND          ((int64_t)40)
#define EXCEPT_ACCESS_DENIED      ((int64_t)41)
#define EXCEPT_INVALID_ADDRESS    ((int64_t)42)
#define EXCEPT_CUSTOM_START       ((int64_t)100)

typedef struct Pair {
  int64_t first;
  int64_t second;
} Pair;

typedef struct _Slice {
  uint8_t buf[1023];
} Slice;

typedef struct DictINext {
  int64_t key;
  Slice value;
  bool_t result;
} DictINext;

typedef struct _Hashmap {
  int64_t keylen;
} Hashmap;

//Dictionaries
Dict tvm_new_dict();
bool_t tvm_dictadd(Dict dict, int64_t key, Slice data);
Pair tvm_dictuget(int64_t key, Dict dict_id, int64_t keylen);
Pair tvm_dictudel(Dict dict_id, int64_t key, int64_t keylen);
DictINext tvm_dictugetnext(Dict dict, int64_t start_key, int64_t keylen);
//Slices
Pair tvm_ldrefrtos(int64_t slice);
Slice tvm_ldref(Slice* data);
Pair tvm_ldu(int64_t slice);
void tvm_ldslice(Slice* slice, void* buf, int64_t len);
//Memory
int64_t tvm_pdload(int64_t index);
int64_t tvm_pdstore(int64_t index, int64_t value);
