#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>

#include <dlfcn.h>

#include <errno.h>
#include <fcntl.h>
#include <poll.h>

#include "gdhcp/gdhcp.h"

static int dhcp_fd = -1;
static int payload_fd = -1;
static int payload_delivered = 0;

static int (*real_bind)(int, const struct sockaddr *, socklen_t) = NULL;

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (NULL == real_bind) {
        real_bind = dlsym(RTLD_NEXT, "bind");
        assert(real_bind);
    }

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *in = (const struct sockaddr_in*)addr;
        if (67 == ntohs(in->sin_port)) {
            fprintf(stderr, "DEBUG: found DHCP server FD: %d\n", sockfd);
            dhcp_fd = sockfd;
        }
    }

    return real_bind(sockfd, addr, addrlen);
}

//static int (*real_poll)(struct pollfd *, nfds_t, int);

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
    int i = -1;
    int res = 0;

    /*if (NULL == real_poll) {
        real_poll = dlsym(RTLD_NEXT, "poll");
        assert(real_poll);
    }*/

    if (payload_delivered) {
        exit(0);
    }

    for (i = 0; i < nfds; i++) {
        if (dhcp_fd == fds[i].fd) {
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

    if (dhcp_fd == fd) {
        result = real_read(payload_fd, buf, count);
        payload_delivered = 1;
    } else {
        result = real_read(fd, buf, count);
    }

    return result;
}

void debug_func(const char *str, gpointer data) {
    fprintf(stderr, "DEBUG: %s\n", str);
}

int main(int argc, char **argv) {
    GDHCPServerError err;
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

    memset(&err, 0x0, sizeof(err));

    GDHCPServer *server = g_dhcp_server_new(G_DHCP_IPV6, 1, &err);
    assert(server);
    assert(G_DHCP_SERVER_ERROR_NONE == err);

    g_dhcp_server_set_debug(server, &debug_func, NULL);

    int res = g_dhcp_server_set_ip_range(server, "192.168.1.100", "192.168.1.254");
    assert(0 == res);

    res = g_dhcp_server_start(server);
    assert(0 == res);

    main_loop = g_main_loop_new(NULL, FALSE);

    g_main_loop_run(main_loop);

    return 0;
}