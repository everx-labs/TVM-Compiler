Testing your code
To test the LLVM compiler (https://llvm.org/docs/TestingGuide.html), it is supposed to use integrated tools LIT (https://llvm.org/docs/CommandGuide/lit.html) and FileCheck (https://llvm.org /docs/CommandGuide/FileCheck.html).
Below is an example of the test, that can be executed by the LIT tool.

```
; RUN: llc < %s -march=tvm | FileCheck %s
target datalayout = "E-S1024-i256:256:256"
target triple = "tvm"

; CHECK-LABEL: icmpeq
define i64 @icmpeq(i64 %par1, i64 %par2) nounwind {
; CHECK: EQUAL
; CHECK-NEXT: PUSHINT 77
; CHECK-NEXT: PUSHINT 42
; CHECK-NEXT: XCHG s0, s2
; CHECK: CONDSEL
  %1 = icmp eq i64 %par1, %par2
  %2 = select i1 %1, i64 42, i64 77
  ret i64 %2
}
```

Analyzing the comment in the test file header, LIT preforms compilation of the test file into the target architecture and pass result into the FileCheck tool to check the compliance of the code generated in the target architecture with the expected one.

Executing your program.
To execute your compiled code a TVM installation is required.
At first you should clone the repository https://github.com/tonlabs/sdk-emulator to a preferred folder. Then go to the '<git clone directory>/tvm' and follow the installation instructions described in the README.md file.

To execute your code and see the execution information, run in console

```
cargo run --bin demo --features verbose -- <path to your TVM ASM file>
```

This command execute your code in the node emulator and writes all execution process messages into the standart output. It might be helpful to see execution flow and stack state before and after your code execution and also before and after each of TVM instruction in your code.
