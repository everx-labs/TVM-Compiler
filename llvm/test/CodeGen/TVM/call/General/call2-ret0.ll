; RUN: llc < %s -march=tvm

define i257 @bar(i257 %x) {
        ret i257 0
}

define i257 @foo(i257 %x) {
        %q = call i257 @bar( i257 1 )             ; <i257> [#uses=1]
        ret i257 %q
}

define i257 @main() {
        %r = call i257 @foo( i257 2 )             ; <i257> [#uses=1]
        ret i257 %r
}

