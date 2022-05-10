#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include <dlfcn.h>

#include <fcntl.h>
#include <poll.h>

#include <netinet/if_ether.h>

#include "gdhcp/gdhcp.h"
#include "gdhcp/common.h"

static int payload_delivered = 0;
static int payload_fd = -1;

static int raw_socket_fd = -1;

int socket(int domain, int type, int protocol)
{
    static int (*real_socket)(int, int, int) = NULL;

    // if the program requests a raw socket, return a fake fd
    if (PF_PACKET == domain && htons(ETH_P_IP) == protocol) {
        return open("/dev/null", O_RDONLY);
    }

    // otherwise, just do the regular socket call
    if (NULL == real_socket) {
        real_socket = dlsym(RTLD_NEXT, "socket");
    }
    return real_socket(domain, type, protocol);
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    /*static int (*real_bind)(int, const struct sockaddr *, socklen_t) = NULL;

    if (NULL == real_bind) {
        real_bind = dlsym(RTLD_NEXT, "bind");
        assert(real_bind);
    }
    return real_bind(sockfd, addr, addrlen); */

    /* this is really hacky */
    if (-1 == raw_socket_fd) {
        raw_socket_fd = sockfd;
    }
    return 0;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int i = -1;
    int res = 0;

    /*if (NULL == real_poll) {
        real_poll = dlsym(RTLD_NEXT, "poll");
        assert(real_poll);
    }*/

    if (payload_delivered) {
        fprintf(stderr, "payload delivered, goodbye\n");
        exit(0);
    }

    for (i = 0; i < nfds; i++) {
        if (raw_socket_fd == fds[i].fd) {
            fds[i].revents = fds[i].events;
            res = 1;
        } else {
            fds[i].events = 0;
        }
    }

    return res;
}

static ssize_t (*real_read)(int fd, void *buf, size_t count) = NULL;

ssize_t read(int fd, void *buf, size_t count)
{
    int result = -1;

    if (NULL == real_read) {
        real_read = dlsym(RTLD_NEXT, "read");
        assert(real_read);
    }

    if (raw_socket_fd == fd) {
        /* The DHCP client is reading from a raw socket, so we need to return
         * the IP and UDP headers BEFORE we can return the DHCP payload. Connman
         * does sanity checking + checksumming before processing the packet. */
        struct ip_udp_dhcp_packet packet;

        // initialize IP portion
        memset(&packet.ip, 0x0, sizeof(packet.ip));
        packet.ip.protocol = IPPROTO_UDP;
        packet.ip.version = IPVERSION;
        packet.ip.ihl = sizeof(packet.ip) >> 2;

        // initialize UDP portion
        memset(&packet.udp, 0x0, sizeof(packet.udp));
        packet.udp.dest = htons(CLIENT_PORT);

        // Read the DHCP payload.
        int dhcp_len = real_read(payload_fd, &packet.data, sizeof(packet.data));

        // Update IP and UDP fields.
        packet.udp.len = htons(sizeof(packet.udp) + dhcp_len);

        result = sizeof(packet.ip) + sizeof(packet.udp) + dhcp_len;
        assert(result <= count);
        packet.ip.tot_len = htons(result);
        packet.ip.check = dhcp_checksum(&packet.ip, sizeof(packet.ip));

        // save a copy of the ip header
        struct iphdr tmp = packet.ip;
        // zero up to the field protocol
        memset(&packet.ip, 0, offsetof(struct iphdr, protocol));
        // replace tot_len with udp length for checksum computation
        packet.ip.tot_len = packet.udp.len;
        // set checksum to zero
        packet.ip.check = 0;
        packet.udp.check = dhcp_checksum(&packet, result);

        // restore ip header
        packet.ip = tmp;

        // Copy the result into the buffer.
        memcpy(buf, &packet, result);

        // Mark payload as delivered.
        payload_delivered = 1;
    } else {
        result = real_read(fd, buf, count);
    }

    return result;
}

void debug_func(const char *str, gpointer data)
{
    fprintf(stderr, "DEBUG: %s\n", str);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
    // dont actually send anything, just return success
    debug_func("in sendto", NULL);
    return (ssize_t)len;
}

int open(const char *pathname, int flags, ...)
{
    static int (*real_open)(const char*, int, ...) = NULL;

    if (NULL == real_open) {
        real_open = dlsym(RTLD_NEXT, "open");
    }
    assert(real_open);

    if (0 == strcmp(pathname, "/dev/urandom")) {
        return real_open("/dev/zero", O_RDONLY);
    }

    return real_open(pathname, flags);
}


int main(int argc, char **argv)
{
    GDHCPClientError err;
    GMainLoop *main_loop = NULL;

    if (2 > argc) {
        printf("usage: %s PAYLOAD\n", argv[0]);
        return 1;
    }

    payload_fd = open(argv[1], O_RDONLY);
    if (-1 == payload_fd) {
        fprintf(stderr, "could not open file: %s\n", strerror(errno));
        return 2;
    }

    GDHCPClient *client = g_dhcp_client_new(G_DHCP_IPV4, 1, &err);
    debug_func("client created", NULL);

    assert(client);
    assert(G_DHCP_CLIENT_ERROR_NONE == err);

    g_dhcp_client_set_debug(client, &debug_func, NULL);
    debug_func("debug func set",NULL);

    g_dhcp_client_start(client, NULL);
    debug_func("client started", NULL);

    main_loop = g_main_loop_new(NULL, FALSE);
    debug_func("main loop created", NULL);
    g_main_loop_run(main_loop);

    return 0;
}