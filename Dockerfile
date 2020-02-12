FROM alpine as build-ton-compiler

RUN apk add binutils-gold musl-dev gcc g++ python2 git ninja cmake wget bash
RUN addgroup -S user && adduser -S -G user user

WORKDIR /home/user
COPY . TON-Compiler

WORKDIR /home/user/TON-Compiler/build
RUN cmake -G Ninja \
-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold" \
-DCMAKE_BUILD_TYPE=Release \
-DLLVM_ENABLE_ASSERTIONS=1 \
-DLLVM_BUILD_TOOLS=0 \
-DLLVM_TARGETS_TO_BUILD="" \
-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM \
-DLLVM_BYTE_SIZE_IN_BITS=257 \
-DCLANG_ENABLE_STATIC_ANALYZER=0 \
-DCLANG_ENABLE_ARCMT=0 \
../llvm

RUN ninja clang llc FileCheck count not llvm-config llvm-as llvm-link opt
RUN bin/llvm-lit ../llvm/test/CodeGen/TVM
RUN bin/llvm-lit ../llvm/tools/clang/test/CodeGen/tvm


FROM alpine

COPY --from=build-ton-compiler /home/user/TON-Compiler/stdlib/ton-sdk        /usr/include/
COPY --from=build-ton-compiler /home/user/TON-Compiler/stdlib/cpp-sdk        /usr/include/
COPY --from=build-ton-compiler /home/user/TON-Compiler/stdlib/abi_parser.py  /usr/bin/

COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/clang       /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llc         /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/count       /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/not         /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/FileCheck   /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-config /usr/bin/

# TODO Remove these after fixing TON-Infrastructure/pipelines/compilers/Dockerfile
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/clang-7     /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llc         /usr/bin/LLVM/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/count       /usr/bin/LLVM/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/not         /usr/bin/LLVM/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/FileCheck   /usr/bin/LLVM/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-config /usr/bin/LLVM/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/stdlib                /app
COPY --from=build-ton-compiler /home/user/TON-Compiler/install.sh            /app/

COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/opt         /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-link   /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-as     /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/tools/tvm-build/tvm-build.py /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/tools/tvm-build/tvm-build++.py /app/
