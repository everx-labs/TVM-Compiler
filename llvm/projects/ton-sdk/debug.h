// This header file provides primitives for TVM runtime debugging
#ifndef TVM_DEBUG_H
#define TVM_DEBUG_H

#define TON_DEBUG_GET_ARG_COUNT(...)                                           \
  TON_DEBUG_GET_ARG_COUNT_IMPL(0, ##__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define TON_DEBUG_GET_ARG_COUNT_IMPL(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_,    \
                                     _8_, _9_, count, ...)                     \
  count

#define TON_DEBUG_JOIN_AGAIN(x, y) x##y
#define TON_DEBUG_JOIN(x, y) TON_DEBUG_JOIN_AGAIN(x, y)

#define TVM_DEBUG_PRINT(S, ...)                                                \
  {                                                                            \
    __builtin_tvm_printstr(S);                                                   \
    TON_DEBUG_JOIN(tonstdlib_debug_print,                                      \
                   TON_DEBUG_GET_ARG_COUNT(__VA_ARGS__))                       \
    (__VA_ARGS__);                                                             \
    __builtin_tvm_logflush();                                                  \
  }

inline void tonstdlib_debug_print0() { }

inline void tonstdlib_debug_print1(int a1) { __builtin_tvm_print_value(a1); }

inline void tonstdlib_debug_print2(int a1, int a2) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
}

inline void tonstdlib_debug_print3(int a1, int a2, int a3) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
}

inline void tonstdlib_debug_print4(int a1, int a2, int a3, int a4) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
}

inline void tonstdlib_debug_print5(int a1, int a2, int a3, int a4, int a5) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
  __builtin_tvm_print_value(a5);
}

inline void tonstdlib_debug_print6(int a1, int a2, int a3, int a4, int a5,
                                   int a6) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
  __builtin_tvm_print_value(a5);
  __builtin_tvm_print_value(a6);
}

inline void tonstdlib_debug_print7(int a1, int a2, int a3, int a4, int a5,
                                   int a6, int a7) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
  __builtin_tvm_print_value(a5);
  __builtin_tvm_print_value(a6);
  __builtin_tvm_print_value(a7);
}

inline void tonstdlib_debug_print8(int a1, int a2, int a3, int a4, int a5,
                                   int a6, int a7, int a8) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
  __builtin_tvm_print_value(a5);
  __builtin_tvm_print_value(a6);
  __builtin_tvm_print_value(a7);
  __builtin_tvm_print_value(a8);
}

inline void tonstdlib_debug_print9(int a1, int a2, int a3, int a4, int a5,
                                   int a6, int a7, int a8, int a9) {
  __builtin_tvm_print_value(a1);
  __builtin_tvm_print_value(a2);
  __builtin_tvm_print_value(a3);
  __builtin_tvm_print_value(a4);
  __builtin_tvm_print_value(a5);
  __builtin_tvm_print_value(a6);
  __builtin_tvm_print_value(a7);
  __builtin_tvm_print_value(a8);
  __builtin_tvm_print_value(a9);
}

#endif
