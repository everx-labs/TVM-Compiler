#include "tvs_sdk.h"

__attribute__(("data"))
struct _PersistentData {
    int64_t total_value;
    int64_t starttime; //unixtime in seconds.
    int64_t period; //unixtime in seconds, for our case it will be one week.
} data;

SmartContractInfo get_smc_info() {
    return (SmartContractInfo)load reg c5.ref0;
}

__attribute__((run_once))
void constructor(int64_t total_value, int64_t period) {
    data.total_value = total_value;
    data.period = period;
    data.starttime = get_smc_info().block_ut;
}

__attribute__((signed))
void transfer(IntAddress to) {
    tvm_throwif(is_zero_address(to), 100);
    tvm_throwif(get_smc_info().remaining_balance < data.total_value);
    _execute_transaction(to, get_smc_info().remaining_balance);
}

void _execute_transaction(IntAddress to, int64_t value) {
    send_int_msg(to, value, new_slice());
}
