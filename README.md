# C and C++ compiler for TVM

`Getting C++ toolchain in binary form`
This README is mostly about building C++ for TVM which is the most appropriate way to get the compiler and the library for contributors. If your intent is only to use the compiler, we provide binary package you can simply download:
- [For Ubuntu](http://sdkbinaries-ws.tonlabs.io/clang-for-tvm/clang-for-tvm.tar.gz)
- [For Mac OS X](https://sdkbinaries.tonlabs.io/clang-for-tvm/clang-for-tvm-darwin.zip)

The binaries are updated on every commit in master, so they are always up to date.

`Clang for TVM based on LLVM 7.0.0`.
This repository contain
* Clang for TVM C and C++ compiler
* [C++ runtime and SDK headers-only library](https://github.com/tonlabs/TON-Compiler/tree/master/llvm/projects/ton-compiler)
* [C runtime and SDK library](https://github.com/tonlabs/TON-Compiler/tree/master/llvm/projects/ton-compiler)
The following guide is about building the compilers and installing them, to find a user guide and example contracts, please refer to [the samples repository](https://github.com/tonlabs/samples).

## Prerequisites
To build the toolchain, you need a recent C++ toolchain supporting C++17:
- MSVC 2017 or newer
- Clang 6.0.0 or newer
- GCC 7.3.0 or newer
- Rust 1.47.0 or newer
- Cargo

Stable operation of older toolchains is not guaranteed.
You also need zlib 1.2.3.4 or newer. Python 2.7 is required to run tests. Optionally, you can use ninja-build.
For more info about LLVM software requirements visit: [https://llvm.org/docs/GettingStarted.html](https://llvm.org/docs/GettingStarted.html).

## Supported operation systems
We expect the project to build successfully on
- Ubuntu 16.04, 18.04
- Mac OS Mojave 10.14.3 or higher
- Windows 10

## Building and installing
To build and to install the compiler use the following script:
```
$ git clone git@github.com:tonlabs/TON-Compiler.git && cd TON-Compiler
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/path/to/install -C /path/to/TON-Compiler/cmake/Cache/ton-compiler.cmake ../llvm
$ cmake --build . --target install-distribution
```
Notes:
* `/path/to/install` must be complete path to the installation folder, otherwise Clang might be unable to find all the libraries, headers and tools by default.
* We strongly recommend to use the installed version of the compiler, otherwise it might be unable to find the required headers and tools by itself, so they have to be specified by hands.
* A complete C and C++ toolchain require [tvm_linker](https://github.com/tonlabs/TVM-linker/) to be built. We recommend to put the liker binary to `/path/to/install/bin` directory. Otherwise you might need to use `-fuse-ld` option to spicify full name of the linker.
* `install-distribution` installs only required minimum of tools, while `install` target copies all the LLVM tools to the installation folder. These additional tools doesn't necessary work with TVM target properly.

For Windows / Microsoft Visual Studio 2017/2020 (clang Compiler without linker & other external tools)
```
> git clone git@github.com:tonlabs/TON-Compiler.git && cd TON-Compiler
> mkdir build
> cd build
> cmake -G "Visual Studio 15" -DLLVM_TARGETS_TO_BUILD="TVM" -C /path/to/TON-Compiler/cmake/Cache/ton-compiler-alone.cmake ../llvm
```

Then open generated solution file LLVM.sln with Visual Studio

### Troubleshooting and speeding up the build
Building Clang takes quite a bit of time, below we list some options to speed it up:
* Use faster build system via `-GXXX` option. We recommend to choose `ninja-build` (you might need to install it first) using `-GNinja`.
* Use a faster linker via `-DCMAKE_LINKER=<linker name>`. It's known that `lld` is faster than `gold`, and gold is faster than `ld`. On Linux, however, `lld` might not be a part of the default toolchain, so you might have to install it first.
* Use a faster compiler via `-DCMAKE_C_COMPILER=<compiler>` and `-DCMAKE_CXX_COMPILER=<compiler>`. Clang might be slightly faster than GCC.
* Build dynamically linked version of Clang via `-DBUILD_SHARED_LIB=On`.
Note that building Clang also require a tens of gigabytes of disk space (especially for a static build) and several gigabytes of RAM to be build. In case, you run out of memory `-DLLVM_PARALLEL_LINK_JOBS=N` might be of help. This option limits the number of link jobs running in parallel does the footpring. There is also a separate option `-DLLVM_PARALLEL_COMPILE_JOBS=N` to limit number of compilation jobs running in parallel.
To learn more about possible configurations of LLVM build, please refer to [LLVM documentation](https://llvm.org/docs/CMake.html). In case you are experiencing problems with build, we would appreciete raising an issue in this repository.

## Running tests
To run tests for TVM, execute
```
$ cmake --build . --target check-llvm-codegen-tvm
```

To run tests for other platforms (to ensure that LLVM itself is not broken), you have to create a separate build without using `/path/to/TON-Compiler/cmake/Cache/ton-compiler.cmake` config and run
```
$ cmake --build . --target check all
```

For more details see [testing.md](https://github.com/tonlabs/TON-Compiler/blob/readme/testing.md).

## Example of usage
You can learn more about C++ for TVM and find examples of usage of C++ toolchain [here](https://github.com/tonlabs/samples/tree/master/cpp). C toolchain is mostly for geeks who want to follow TVM assembly closely, but doesn't want to work with stack. C examples might be found [here](https://github.com/tonlabs/samples/tree/master/c).

## Getting support
C and C++ for TVM, being similar to conventional C and C++, has their own extensions and limitations, so if you are getting started with programming for TVM, we recommend to first refer to [the examples repository](https://github.com/tonlabs/samples).
Texts, videos and samples illustrating how to use the compiler will soon appear at https://ton.dev/ and https://www.youtube.com/channel/UC9kJ6DKaxSxk6T3lEGdq-Gg. Stay tuned.
You can also get support in [TON Dev Telegram channel](https://t.me/tondev_en).
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
