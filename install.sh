apk add --no-cache \
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
        bash && \
cp -r /app/ton-sdk /usr/include && \
cp -r /app/cpp-sdk /usr/include && \
cp /app/abi_parser.py /usr/bin  && \
chmod a+x /usr/bin/abi_parser.py  && \
ln -s /usr/bin/abi_parser.py /usr/bin/ton-abi-parser
