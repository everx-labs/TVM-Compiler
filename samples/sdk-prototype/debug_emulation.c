#include "ton-sdk/tvm.h"
#include <stdio.h>
#include <string.h>

char buffer [10000];
char sc_info_buffer [10000];
int write_ptr = 0;
int read_ptr = 0;

int tonstdlib_work_slice_load_int (unsigned width) {
    int res = 0;
    for (int i = 0; i < width; i++) {
        if (buffer[read_ptr] == 0) printf ("????");
        res = res * 2 + buffer[read_ptr++]-'0';
    }
    return res;
}

unsigned tonstdlib_work_slice_load_uint (unsigned width) {
    unsigned res = 0;
    for (int i = 0; i < width; i++) {
        if (buffer[read_ptr] == 0) printf ("????");
        res = res * 2 + buffer[read_ptr++]-'0';
    }
    return res;
}

void tonstdlib_work_slice_store_int (int value, unsigned width) {
    for (int i = 0; i < width; i++) {
        buffer[write_ptr++] = (width-i-1 > 63 ? 0 : (value >> (width-i-1)) & 1) ? '1' : '0';
    }
}

void tonstdlib_work_slice_store_uint (unsigned value, unsigned width) {
    for (int i = 0; i < width; i++) {
        buffer[write_ptr++] = (width-i-1 > 63 ? 0 : (value >> (width-i-1)) & 1) ? '1' : '0';
    }
}

void tonstdlib_send_work_slice_as_rawmsg (int flags) {
    buffer [write_ptr] = 0;
    printf ("Message: %s", buffer);
}

int __builtin_tvm_throwif(int condition,int exc) {
    if (condition) {
        printf ("throw: %d\n", exc);
    }
    return 0;
}

void tonstdlib_create_empty_work_slice () {
    read_ptr = write_ptr = 0;
}

void tonstdlib_get_smart_contract_info () {
    strcpy (buffer, sc_info_buffer);
    read_ptr = write_ptr = 0;
}
#if 0
void set_sc_info () {
    strcpy (sc_info_buffer, buffer);
}

void print_buffer () {
    buffer [write_ptr] = 0;
    printf ("Buffer: %s\n", buffer);
}
#endif