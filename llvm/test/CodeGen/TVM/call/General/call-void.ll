; RUN: llc < %s -march=tvm

define void @foo() {
        ret void
}

define i257 @main() {
        call void @foo( )
        ret i257 0
}

