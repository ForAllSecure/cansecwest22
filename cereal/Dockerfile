FROM --platform=linux/amd64 ubuntu:20.04 as builder

RUN apt-get update && \
    apt-get install -y g++ make

ADD . /cereal
WORKDIR /cereal
RUN make

FROM --platform=linux/amd64 ubuntu:20.04

COPY --from=builder /cereal/json_fuzzer /json_fuzzer
