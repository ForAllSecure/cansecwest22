#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>

#include "../src/connman.h"


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t sz)
{
    unsigned char buf[4096];

    size_t len = sz > sizeof(buf) ? sizeof(buf) : sz;
    memcpy(buf, data, len);

    forward_dns_reply(buf, len, IPPROTO_UDP, data);

    return 0;
}
