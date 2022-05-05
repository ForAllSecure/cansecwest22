FROM ubuntu:20.04 as builder

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y cmake g++

ADD . /libbw64
WORKDIR /libbw64

RUN mkdir build && \
    cd build/ && \
    cmake -DBW64_EXAMPLES=On .. && \
    make -j$(grep -c ^processor /proc/cpuinfo)

FROM ubuntu:20.04

COPY --from=builder /libbw64/build/examples/bw64_read_write ./

