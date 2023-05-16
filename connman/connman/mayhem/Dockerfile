FROM ubuntu as builder

RUN apt update -y && DEBIAN_FRONTEND=noninteractive apt install -y clang \
                            make \
                            cmake \
                            git \
                            autotools-dev \
                            autoconf \
                            libtool \
                            libgtk2.0-dev \
                            libc6-dbg \
                            libdbus-1-dev \
                            libxtables-dev \
                            libreadline-dev \
                            gnutls-dev \
                            systemd
COPY . .
RUN git apply ./mayhem/dnsproxy-fuzzer.patch
RUN autoupdate && ./bootstrap && CC=clang ./configure && make

FROM ubuntu

COPY --from=builder /src/connmand /
RUN apt update -y && DEBIAN_FRONTEND=noninteractive apt install -y libglib2.0-0 libc6-dbg libdbus-1-dev libxtables12 libgnutls30

#CMD ["/connmand"]
