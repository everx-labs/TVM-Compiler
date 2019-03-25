# LLVM-based Toolchain for TVM
`Toolchain for TVM 0.1.0 based on LLVM 7.0.0`.
Note: this is an alpha version of the toolchain which is under active development. Many C and C++ features are not yet supported.
To learn more about the architecture of TVM, please refer to TBD.
## Prerequesites
To build the toolchain, you need a recent C++ toolchain supporting C++17:
- MSVC 2017 or newer
- Clang 6.0.0 or newer
- GCC 7.3.0 or newer
Older versions of toolchains might work, but it's not guaranteed.
You also need zlib 1.2.3.4 or newer. Python 2.7 is required to run tests. We recommend using ninja-build, though it's not necessary. You could find more about software requirements to build LLVM at https://llvm.org/docs/GettingStarted.html.
## Supported operation systems
The project expects to be buildable on any Linux distribution, FreeBSD, Mac OS X and Windows platforms supporting the required software. The following configurations were tested:
- Ubuntu 16.04, 18.04
- Mac OS Mojave 10.14.3
- Windows 10
## Building
At the moment TVM toolchain contains no specific flags and you can build it as an ordinary LLVM-based project except for `LLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM` which enables TVM backend in LLVM.
Sample build scenario:
```
$ git clone git@github.com:tonlabs/TON-Compiler.git
$ mkdir build
$ cd build
$ cmake -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM -DLLVM_TARGETS_TO_BUILD=X86 -GNinja ../TON-Compiler/llvm
$ ninja
```
To learn more about available CMake options, please refer to https://llvm.org/docs/CMake.html
## Running tests
To run tests, execute
```
$ ninja check
```
Tests are described in more detail in [testing.md](https://github.com/tonlabs/TON-Compiler/blob/readme/testing.md).
## Example of usage
TBD
## Known issues
At the moment C specification support is limited. In particular, there is no support for:
- Loops, goto
- Memory operations
- Non-integer types including pointers
- Recursive calls
Aside from that, TVM specific functionality is limited to:
- Creation of a dictionary
- Getting and setting a value in a dictionary
- Minimal manipulations with slices, builders, and cells
Samples of the current state of the compiler are available at TBD.
## Getting support
TBD
## Contribution policy
The project strives to follow LLVM coding standards and policies as well as C++ Core Guidelines, so before contributing, we recommend you to familiarize with the following documents:
- [LLVM Developer Policy](https://llvm.org/docs/DeveloperPolicy.html)
- [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
- [LLVM Programmer’s Manual](http://llvm.org/docs/ProgrammersManual.html)
- [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md)

Note: Since TVM backend uses C++17 which is not yet fully supported in LLVM, the guidelines could be reasonably adjusted for cases of C++17 usages. C++17 data structures are preferred over LLVM counterparts, for instance, `std::optional<T>` is better to use w.r.t. `llvm::Optional<T>`.
