FROM --platform=linux/amd64 ubuntu:20.04 as builder

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y gcc g++ cmake make

ADD . /libbw64
WORKDIR /libbw64
RUN rm -rf build && \
    mkdir build && \
    cd build/ && \
    cmake .. && \
    make

FROM --platform=linux/amd64 ubuntu:20.04

COPY --from=builder /libbw64/build/examples/bw64_read_write /
