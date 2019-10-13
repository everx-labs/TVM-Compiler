// RUN: %clang -O3 -S -c -target tvm %s -std=c++17 -I%S/../../../../../../../stdlib/cpp-sdk/ -o - | tvm-testrun --no-trace --entry main | FileCheck %s

#include "untuple_caller.hpp"
#include "builder.hpp"
#include "parser.hpp"
#include "tuple.hpp"

struct __attribute__((tvm_tuple)) tuple_struct_XY {
  int x;
  int y;
};

tvm::tuple<tuple_struct_XY> make2() {
  tuple_struct_XY tp = { 100, 200 };
  return tvm::tuple<tuple_struct_XY>(tp);
}

int sum2(tvm::tuple<tuple_struct_XY> tpHandle) {
  tuple_struct_XY tp = tpHandle.unpack();
  return tp.x + tp.y;
}

class __attribute__((tvm_tuple)) Message {
public:
  Message(int x, int y, int z) : x(x), y(y), z(z) {}
  
  __tvm_cell build() const {
    tvm::builder b;
    b.stu(x, 12);
    b.stu(y, 24);
    b.stu(z, 28);
    return b.make_cell();
  }
  static Message parse(__tvm_cell cell) {
    tvm::parser p(cell);
    auto x = p.ldu(12);
    auto y = p.ldu(24);
    auto z = p.ldu(28);
    return Message(x, y, z);
  }
  void send() const {
    __builtin_tvm_sendrawmsg(build(), 0);
  }
  tvm::tuple<Message> pack() const { return tvm::tuple<Message>(*this); }

  int x;
  int y;
  int z;
};

tvm::tuple<Message> make_msg() {
  return Message(190, 50, 70).pack();
}

// CHECK-NOT: Unhandled exception
int main() {
  auto msg = make_msg();
  auto msg_cell = msg.unpack().build();
  auto parsed = Message::parse(msg_cell);
  __builtin_tvm_throwif(parsed.x != 190, 11);
  __builtin_tvm_throwif(parsed.y != 50, 12);
  __builtin_tvm_throwif(parsed.z != 70, 13);
  return sum2(make2());
}

