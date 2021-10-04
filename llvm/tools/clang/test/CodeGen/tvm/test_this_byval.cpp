// RUN: %clang -O3 -S -c -emit-llvm -target tvm %s --sysroot=%S/../../../../../projects/ton-compiler/cpp-sdk/ -o -
// REQUIRES: tvm-registered-target

//using namespace tvm;
//using namespace schema;

__interface ITest {
  [[internal, noaccept]]
  void foo();
};

struct DTest {
  unsigned b_;
};

struct ETest {
};

static constexpr unsigned TIMESTAMP_DELAY = 1800;

class Test final : public ITest, public DTest {
public:
  //using replay_protection_t = replay_attack_protection::timestamp<TIMESTAMP_DELAY>;

  __attribute__((noinline))
  void foo() {
    ++b_;
  }
  //DEFAULT_SUPPORT_FUNCTIONS(ITest, replay_protection_t);
};

__attribute__((tvm_raw_func)) int main_internal(__tvm_cell msg, __tvm_slice msg_body) {
  Test t;
  t.foo();
  return 0;
}

//DEFINE_JSON_ABI(ITest, DTest, ETest);

// ----------------------------- Main entry functions ---------------------- //
//DEFAULT_MAIN_ENTRY_FUNCTIONS(Test, ITest, DTest, TIMESTAMP_DELAY)

