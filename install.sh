apk add --no-cache \
&& apk add --virtual \
    build-dependencies \
    build-base \
    binutils-gold \
    python2 \
    gcc \
    wget \
    git \
    ninja \
    cmake \
&& apk add \
    bash \
&& cp -rv /app/ton-sdk /usr/include \
&& cp -v \
    /app/LLVM/bin/llc \
    /app/LLVM/bin/FileCheck \
    /app/LLVM/bin/tvm-testrun \
    /app/LLVM/bin/llvm-config \
    /app/LLVM/bin/count \
    /app/LLVM/bin/not \
    /app/LLVM/bin/clang \
    /app/LLVM/bin/clang-7 \
    /app/abi_parser.py \
    /app/stdlib_c.tvm \
    /usr/bin \
&& chmod a+x \
    /usr/bin/llc \
    /usr/bin/FileCheck \
    /usr/bin/tvm-testrun \
    /usr/bin/llvm-config \
    /usr/bin/count \
    /usr/bin/not \
    /usr/bin/clang \
    /usr/bin/clang-7 \
    /usr/bin/abi_parser.py \
&& ln -s /usr/bin/abi_parser.py /usr/bin/ton-abi-parser
