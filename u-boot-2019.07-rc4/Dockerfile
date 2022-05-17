FROM --platform=linux/amd64 ubuntu:20.04 as builder

RUN apt-get update && \
    apt-get install -y clang make bison flex afl++ libssl-dev

ADD . /uboot
WORKDIR /uboot

ENV NO_SDL 1
RUN make sandbox_defconfig
RUN make CC=afl-clang-fast CXX=afl-clang-fast++ -j$(grep -c ^processor /proc/cpuinfo)

FROM --platform=linux/amd64 ubuntu:20.04

COPY --from=builder /uboot/u-boot /

