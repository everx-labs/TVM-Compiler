typedef long long int64_t;


#define EXCEPT_NOT_FOUND          ((int64_t)40)
#define EXCEPT_ACCESS_DENIED      ((int64_t)41)
#define EXCEPT_INVALID_ADDRESS    ((int64_t)42)
#define EXCEPT_CUSTOM_START       ((int64_t)100)

typedef struct Pair {
  int64_t first;
  int64_t second;
} Pair;

Pair tvm_dictuget(int64_t key, int64_t dict_id, int64_t keylen);
Pair tvm_ldrefrtos(int64_t slice);
Slice tvm_ldref(Slice* data);
Pair tvm_ldu(int64_t slice);
int64_t tvm_pdload(int64_t index);
int64_t tvm_pdstore(int64_t index, int64_t value);
