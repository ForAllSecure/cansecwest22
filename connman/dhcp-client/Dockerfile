FROM ubuntu:20.04

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y libglib2.0-0 libc6-dbg && \
    rm -rf /var/lib/apt/lists/*

ADD ./connman/mayhem/fuzz_dhcp_client /

CMD ["/fuzz_dhcp_client"]
