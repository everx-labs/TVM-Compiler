void test_entry_point()
{
// CHECK-NOT: custom error
  __builtin_tvm_throwif(main(), 13);
}