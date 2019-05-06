#define SMC_REMAINING_BALANCE_START 64
#define SMC_REMAINING_BALANCE_LEN 32

#define SMC_BLOCK_UT_START 96
#define SMC_BLOCK_UT_LEN 32

long long get_smc_first_slice() {
  long long smc_slice = __builtin_tvm_getreg(5);
  long long smc_first_cell = __builtin_tvm_retrieve_ref(smc_slice);
  return __builtin_tvm_ctos(smc_first_cell);
}

long long get_smc_info_remaining_balance() {
  long long smc_first_slice = get_smc_first_slice();
  return __builtin_tvm_retrieve_unsigned(
      smc_first_slice, SMC_REMAINING_BALANCE_START, SMC_REMAINING_BALANCE_LEN);
}

long long get_smc_info_block_ut() {
  long long smc_first_slice = get_smc_first_slice();
  return __builtin_tvm_retrieve_unsigned(smc_first_slice, SMC_BLOCK_UT_START,
                                         SMC_BLOCK_UT_LEN);
}

#define PERSISTENT_DATA_TOTAL_VALUE_START 0
#define PERSISTENT_DATA_TOTAL_VALUE_LEN 32

#define PERSISTENT_DATA_PERIOD_START 32
#define PERSISTENT_DATA_PERIOD_LEN 32

#define PERSISTENT_DATA_STARTTIME_START 64
#define PERSISTENT_DATA_STARTTIME_LEN 32

void init_persistent_data(long long total_value, long long period) {
  long long slice = __builtin_tvm_newc();
  long long block_ut = get_smc_info_block_ut();
  slice =
      __builtin_tvm_stu(slice, total_value, PERSISTENT_DATA_TOTAL_VALUE_LEN);
  slice = __builtin_tvm_stu(slice, period, PERSISTENT_DATA_PERIOD_LEN);
  slice = __builtin_tvm_stu(slice, block_ut, PERSISTENT_DATA_STARTTIME_LEN);
  long long cell = __builtin_tvm_endc(slice);
  __builtin_tvm_set_persistent_data(cell);
}

long long get_persistent_total_value() {
  long long root_slice = __builtin_tvm_get_persistent_data();
  return __builtin_tvm_retrieve_unsigned(root_slice,
                                         PERSISTENT_DATA_TOTAL_VALUE_START,
                                         PERSISTENT_DATA_TOTAL_VALUE_LEN);
}

#define INTERNAL_MSG_ANYCAST_START 0
#define INTERNAL_MSG_ANYCAST_LEN 5

#define INTERNAL_MSG_WORKCHAIN_START 5
#define INTERNAL_MSG_WORKCHAIN_LEN 8

#define INTERNAL_MSG_TOADDR_START 13
#define INTERNAL_MSG_TOADDR_LEN 256

long long make_internal_msg_cell(long long to_addr, long long value) {
  long long slice = __builtin_tvm_newc();
  slice = __builtin_tvm_stu(slice, 0, INTERNAL_MSG_ANYCAST_LEN);
  slice = __builtin_tvm_stu(slice, -1, INTERNAL_MSG_WORKCHAIN_LEN);
  slice = __builtin_tvm_stu(slice, to_addr, INTERNAL_MSG_TOADDR_LEN);
  slice = __builtin_tvm_stu(slice, value, 32);
  long long cell = __builtin_tvm_endc(slice);
  return cell;
}

void execute_transaction(long long to_addr, long long value) {
  long long cell = make_internal_msg_cell(to_addr, value);
  __builtin_tvm_sendrawmsg(cell, 0);
}

void transfer(long long to_addr) {
  long long remaining_balance = get_smc_info_remaining_balance();
  __builtin_tvm_throwif(remaining_balance < get_persistent_total_value(), 61);
  execute_transaction(to_addr, remaining_balance);
}
