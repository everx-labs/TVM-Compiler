// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s -o -
// REQUIRES: tvm-registered-target

template<class Ty, Ty val>
struct integral_constant {
  static constexpr unsigned value = val;
};

template<class T>
struct fields_count {
  static constexpr unsigned value = __reflect_fields_count<integral_constant, unsigned, T>::value;
};

template<class T>
static constexpr unsigned fields_count_v = fields_count<T>::value;

struct test_struct_a {
  int a;
  unsigned b;
  struct internal { int x; int y; } c;
  int d;
};

struct test_struct_b {
  int a;
  int b;
};

struct test_struct_c {
};

int foo() {
  static_assert(fields_count_v<test_struct_a> == 4);
  static_assert(fields_count_v<test_struct_b> == 2);
  static_assert(fields_count_v<test_struct_c> == 0);
  return 0;
}

