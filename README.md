# C and C++ compiler for TVM
`Clang for TVM 0.2.0 based on LLVM 7.0.0`.
This repository stores source code of Clang for TON Virtual Machine and a runtime library that will later be moved to a separate repository. TON VM (https://test.ton.org/tvm.pdf) is designed to run smart contracts in TON blockchain (https://test.ton.org/ton.pdf). The compiler and the runtime library (`stdlib` subdirectory) are components of the toolchain that also includes Assembler/Linker/Emulator tool, `tvm_linker` (the source code will be open later).
All relevant binaries are delivered within TON Labs Node SE at https://ton.dev/.
## Prerequesites
To build the toolchain, you need a recent C++ toolchain supporting C++17:
- MSVC 2017 or newer
- Clang 6.0.0 or newer
- GCC 7.3.0 or newer
Stable operation of older toolchains is not guaranteed.
You also need zlib 1.2.3.4 or newer. Python 2.7 is required to run tests. Optionally, you can use ninja-build.
For more info about LLVM software requirements visit: https://llvm.org/docs/GettingStarted.html.

## Supported operation systems
We expect the project to build successfully on
- Ubuntu 16.04, 18.04
- Mac OS Mojave 10.14.3 or higher
- Windows 10

## Building
At the moment TVM toolchain contains no specific flags and you can build it as an ordinary LLVM-based project except for 
* `LLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM` that enables TVM backend in LLVM
* `LLVM_BYTE_SIZE_IN_BITS=257` that defines size of byte the compiler assumes.
Sample build scenario:
```
$ git clone git@github.com:tonlabs/TON-Compiler.git
$ mkdir build
$ cd build
$ cmake -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_BYTE_SIZE_IN_BITS=257 -GNinja ../TON-Compiler/llvm
$ ninja
```
Note that 257 bits byte is incompatible with other targets Clang supports. `LLVM_BYTE_SIZE_IN_BITS` is a temporary solution introduced to support TVM, later on we plan to switch to non-breaking method to support TVM target (see `Upstreaming` section).
To learn more about available CMake options, please refer to https://llvm.org/docs/CMake.html

## Running tests
To run tests for TVM, execute
```
$ ninja check-llvm-codegen-tvm
```

To run tests for other platforms (to ensure that LLVM itself is not broken), Clang build with `-DLLVM_BYTE_SIZE_IN_BITS=8` is required.
```
$ ninja check-all
```

For more details see [testing.md](https://github.com/tonlabs/TON-Compiler/blob/readme/testing.md).

## Example of usage
`samples/sdk-prototype/piggybank.c` contains a sample contract in C.
To compile it to TVM assembly run
```
clang -S -O3 -target tvm -I/path/to/stdlib/ton-sdk piggybank.c
```
TVM target is currently only supports assembly emission, so `clang` invocation without `-S` will return an error.
`-O3` is recommended optimization level, the compiler is tested with O3 enabled, so O0 is less reliable.

## Language limitations and performance issues
* A contract has a limited gas supply. Once it is exceeded, the contract terminates with an error.
* TVM doesn't have float point arithmetic, so float point operations results in an error.
* Contrary to the C specification, unsigned integers overflow can be expected resulting in an exception.
* TVM has stack, but it doesn't have memory. The runtime currently emulates it via dictionaries. Accessing dictionaries incurs high gas costs, so we strongly discourage the use of globals and getting variable addresses.
* TVM uses 257-bits wide numbers. All numberic types are 257-bit wide, and 1 byte = 257 bit. So we encourage to not to make assumption about behavior of implementation-defined features of C and C++.

## Unsupported features
* C and C++ standard libraries (partial support is planned).
* Exception handling.
* Pointers to functions and virtual methods.
* Free store memory allocation / deallocation.

## Getting support
Samples, demonstrating contracts is C are available at [https://github.com/tonlabs/samples/tree/master/c](https://github.com/tonlabs/samples/tree/master/c).
Texts, videos and samples illustrating how to use the compiler will soon appear at https://ton.dev/ and https://www.youtube.com/channel/UC9kJ6DKaxSxk6T3lEGdq-Gg. Stay tuned.
You can also get support in TON Dev Telegram chanel: https://t.me/tondev_en.
In case you found a bug, raise an issue in the repository. Please attach the source file, the command to reproduce the failure and your machine description.

## Contribution policy
The project strives to follow LLVM coding standards and policies as well as C++ Core Guidelines, so before contributing, we recommend you to familiarize with the following documents:
- [LLVM Developer Policy](https://llvm.org/docs/DeveloperPolicy.html)
- [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- [LLVM Programmer’s Manual](http://llvm.org/docs/ProgrammersManual.html)
- [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)

Note: Since TVM backend uses C++17 which is not yet fully supported in LLVM, the guidelines could be reasonably adjusted for cases of C++17 usages. C++17 data structures are preferred over LLVM counterparts, for instance, `std::optional<T>` is better to use w.r.t. `llvm::Optional<T>`.

All changes in LLVM (excluding `lib/Target/TVM` subdirectory) must be marked as local changes in the following way:
```
// TVM local begin
<changed LLVM code>
// TVM local end
```
The reason is to help resolving merge conflicts when updating LLVM to a new version.

All removals from LLVM must be commented out instead:
```
#if 0
<removed LLVM code>
#endif
```
The reason is to minimize number of merge conflicts when updating LLVM to a new version.
To learn more about development a downstream LLVM project, refer to [https://llvm.org/devmtg/2015-10/slides/RobinsonEdwards-LivingDownstreamWithoutDrowning.pdf](https://llvm.org/devmtg/2015-10/slides/RobinsonEdwards-LivingDownstreamWithoutDrowning.pdf).

## Upstreaming
We believe that LLVM community would benefit from getting TVM backend upstream. It's a very distinct architecture, that break several assumptions. For instance, Clang front-end and the optimizer relies on 8-bit bytes byte which is not true for TVM as well as for some non-mainstream processors. Furthermore, target independent code generator is designed for a register machine, so stack machine support is benefitial at least for WebAssembly. So, with some constraints removed, LLVM will be better placed to downstream development and we believe it is feasible without damaging existing targets.
We would like to implement, upstream and maintain the following features:
* Byte size specified in data layout, removing magical number 8 from the optimizer.
* memset, memcopy, memmove configured with the byte size.
* DAG scheduler for a stack machine.
* Generic analysis and transformation passes to optimize for stack machine on MIR level.
The current version of Clang for TVM is based on LLVM 7.0.0, it's planned update LLVM, remove hardcoded byte size and adopt opaque types insted of types we introduced to LLVM.
