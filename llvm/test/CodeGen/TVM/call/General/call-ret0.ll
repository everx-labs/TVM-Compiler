; RUN: llc < %s -march=tvm
define i257 @foo(i257 %x) {
        ret i257 %x
}

define i257 @main() {
        %r = call i257 @foo( i257 0 )             ; <i257> [#uses=1]
        ret i257 %r
}

