#!/bin/sh

### Script for creating TON-Compiler tarball package
### targetting Ubuntu 18.04 LTS.

### Prerequisites:
# apt install cmake ninja-build g++ git python curl
# curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
# git clone https://github.com/tonlabs/TON-Compiler
# git clone https://github.com/tonlabs/TVM-Linker

### Building and installing the compiler

cd ~/TON-Compiler
REV=`git rev-parse --short HEAD`

mkdir build
cd build

cmake \
-G Ninja \
-DLLVM_ENABLE_ASSERTIONS=1 \
-DCMAKE_BUILD_TYPE="Release" \
-DLLVM_BUILD_LLVM_DYLIB=1 \
-DLLVM_LINK_LLVM_DYLIB=1 \
-DLLVM_TARGETS_TO_BUILD="" \
-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="TVM" \
-DLLVM_BYTE_SIZE_IN_BITS=257 \
-DLLVM_USE_LINKER=gold \
../llvm

ninja

mkdir -p ~/TON-Compiler-$REV/bin
mkdir -p ~/TON-Compiler-$REV/lib
mkdir -p ~/TON-Compiler-$REV/stdlib

cp ~/TON-Compiler/build/lib/libLLVM-7.so ~/TON-Compiler-$REV/lib/
cp ~/TON-Compiler/build/bin/clang        ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/build/bin/opt          ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/build/bin/llvm-link    ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/build/bin/llvm-as      ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/build/bin/llvm-dis     ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/build/bin/llc          ~/TON-Compiler-$REV/bin/

cp ~/TON-Compiler/stdlib/abi_parser.py              ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/llvm/tools/tvm-build/tvm-build.py ~/TON-Compiler-$REV/bin/
cp ~/TON-Compiler/llvm/tools/tvm-build/tvm-build++.py ~/TON-Compiler-$REV/bin/

cat >~/TON-Compiler-$REV/bin/tvm-build <<\EOF
#!/bin/sh

ROOT=$(dirname $(dirname $(readlink -f "$0")))
export TVM_LLVM_BINARY_DIR=$ROOT/bin
export TVM_LINKER=$ROOT/bin/tvm_linker
export TVM_LIBRARY_PATH=$ROOT/stdlib

python $ROOT/bin/tvm-build.py $@
EOF
chmod +x ~/TON-Compiler-$REV/bin/tvm-build
chmod +x ~/TON-Compiler-$REV/bin/tvm-build++

cp    ~/TON-Compiler/stdlib/*.tvm   ~/TON-Compiler-$REV/stdlib/
cp -r ~/TON-Compiler/stdlib/ton-sdk ~/TON-Compiler-$REV/stdlib/
cp -r ~/TON-Compiler/stdlib/cpp-sdk ~/TON-Compiler-$REV/stdlib/

### Building and installing linker tool

cd ~/TVM-linker/tvm_linker
cargo build --release
cp ~/TVM-linker/tvm_linker/target/release/tvm_linker ~/TON-Compiler-$REV/bin/

### Packaging

cd ~
tar czf TON-Compiler-$REV.tgz TON-Compiler-$REV/
