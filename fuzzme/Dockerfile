# Build Stage:
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y gcc

## Add Source Code
ADD fuzzme.c /

## Build Step
RUN gcc -g fuzzme.c -o fuzzme

# Package Stage
FROM --platform=linux/amd64 ubuntu:20.04

COPY --from=builder /fuzzme /

