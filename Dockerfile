FROM alpine as build-ton-compiler

RUN apk add binutils-gold musl-dev gcc g++ python2 git ninja cmake wget bash
RUN addgroup -S user && adduser -S -G user user

WORKDIR /home/user
COPY . TON-Compiler

WORKDIR /home/user/TON-Compiler/build
RUN rm -f CMakeCache.txt
RUN cmake -G Ninja \
-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold" \
-DCMAKE_INSTALL_PREFIX=/home/user/TON-Compiler/Distr \
-C ../cmake/Cache/ton-compiler.cmake \
../llvm

RUN ninja FileCheck count not install-distribution
RUN ninja check-llvm-codegen-tvm
RUN bin/llvm-lit ../llvm/tools/clang/test/CodeGen/tvm

FROM alpine

COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/projects/ton-compiler/ton-sdk        /usr/include/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/projects/ton-compiler/cpp-sdk        /usr/include/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/projects/ton-compiler/abi_parser.py  /usr/bin/

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
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/lib/clang/7.0.0/include /usr/lib/clang/7.0.0/include
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/projects/ton-compiler/                /app
COPY --from=build-ton-compiler /home/user/TON-Compiler/install.sh            /app/

COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/opt         /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-link   /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/build/bin/llvm-as     /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/tools/tvm-build/tvm-build.py /app/
COPY --from=build-ton-compiler /home/user/TON-Compiler/llvm/tools/tvm-build/tvm-build++.py /app/
