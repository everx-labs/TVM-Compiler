typedef long long int64_t;

typedef struct Pair {
  int64_t first;
  int64_t second;
} Pair;

Pair tvm_dictuget(int64_t key, int64_t dict_id, int64_t keylen);
Pair tvm_ldrefrtos(int64_t slice);
Pair tvm_ldu(int64_t slice);
int64_t tvm_pdload(int64_t index);
int64_t tvm_pdstore(int64_t index, int64_t value);
