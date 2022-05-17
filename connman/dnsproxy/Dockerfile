FROM ubuntu:20.04

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y libglib2.0-0 libc6-dbg libdbus-1-3 libxtables12 libgnutls30 && \
    rm -rf /var/lib/apt/lists/*

ADD ./src/connmand /

CMD ["/connmand"]
