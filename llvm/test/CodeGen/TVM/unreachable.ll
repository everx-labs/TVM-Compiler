; RUN: llc -march=tvm < %s | FileCheck %s
; CHECK-LABEL: foo
define void @foo() {
  unreachable
}
; CHECK-LABEL: bar
define void @bar() {
entry:
  br label %exit
exit:
  unreachable
}
; CHECK-LABEL: baz
define void @baz(i257 %v) {
  unreachable
}
