#include "tvm_std.h"
	
__attribute__((data))
struct _PersistentData {
    int64_t transactions;
    int64_t total_owners; //total number of castodians
    int64_t last_owner;   //ID of the last custodian for which there is a key 
    int64_t pubkeys;         //custodians public keys
} data;

__attribute__((run_once))
void constructor(int64_t total_owners) {
    data.total_owners = total_owners;
    data.transactions = __builtin_tvm_newdict();
    data.pubkeys = __builtin_tvm_newdict();
}
