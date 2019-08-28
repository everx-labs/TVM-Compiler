FROM alpine as build-ton-compiler

RUN apk add --no-cache \
    && apk add --virtual build-dependencies \
        build-base \
        binutils-gold \
        python2 \
        gcc \
        wget \
        git \
        ninja \
        cmake \
    && apk add \
        bash

# Same as in Eclipse CHE image
RUN addgroup -S user && adduser -S -G user user 

WORKDIR /home/user
COPY . TON-Compiler

RUN mkdir -p /home/user/TON-Compiler/llvm/build

RUN cd /home/user/TON-Compiler/llvm/build && cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DLLVM_BUILD_TOOLS=OFF -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=gold -DCMAKE_INSTALL_PREFIX=/home/user/LLVM -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=TVM -DLLVM_TARGETS_TO_BUILD=X86 -GNinja .. \
    && ninja clang && ninja install

RUN find /home/user/LLVM -name "*.a" -type f -delete
RUN find /home/user/LLVM -type f -not -name 'clang' -not -name 'clang++' -not -name 'clang-cl'  -not -name 'clang-cpp'   -not -name 'clang-7' -not -name 'llc' -not -name 'FileCheck' -not -name 'tvm-testrun' -delete

FROM alpine
COPY --from=build-ton-compiler /home/user/TON-Compiler/samples/sdk-prototype /app
COPY --from=build-ton-compiler /home/user/LLVM /usr/bin/LLVM
COPY --from=build-ton-compiler /home/user/LLVM/bin/clang /usr/bin/
COPY --from=build-ton-compiler /home/user/LLVM/bin/clang-7 /usr/bin/
COPY --from=build-ton-compiler /home/user/TON-Compiler/install.sh /app/install.sh
