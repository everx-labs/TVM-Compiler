; RUN: llc < %s -march=tvm
target datalayout = "E-S257-i1:257:257-i8:257:257-i16:257:257-i32:257:257-i64:257:257-i257:257:257-p:257:257-a:257:257"
target triple = "tvm"

define void @foo() {
        ret void
}

define i257 @main() {
        call void @foo( )
        ret i257 0
}

