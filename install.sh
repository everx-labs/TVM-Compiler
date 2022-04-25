apk add libgcc libstdc++ python2 bash

cp -r /app/ton-sdk /usr/include
cp -r /app/cpp-sdk /usr/include
cp /app/abi_parser.py /usr/bin
chmod a+x /usr/bin/abi_parser.py
ln -s /usr/bin/abi_parser.py /usr/bin/ton-abi-parser

ln -s /usr/bin/clang /usr/bin/clang++
cp /app/tvm-build.py /app/tvm-build++.py /app/opt /app/llvm-link /app/llvm-as /usr/bin
