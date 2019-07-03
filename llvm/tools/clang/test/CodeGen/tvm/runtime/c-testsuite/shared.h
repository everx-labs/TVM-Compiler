void test_entry_point()
{
// CHECK-NOT: custom error
  if (main())
    __builtin_tvm_throwif(1, 13);
}
