FROM --platform=linux/amd64 ubuntu:20.04 as builder
RUN apt-get update && apt-get install -y afl++ clang
ADD testme.c /
RUN afl-clang-fast testme.c -o testme

FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /testme /

