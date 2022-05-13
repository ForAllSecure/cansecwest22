# ConnMan DHCP Client Lab

## Step 1. Build the Connman DHCP Client Harness Locally

```
cd cansecwest22/connman/connman
```

```
./bootstrap
```

```
./configure --enable-debug
```

```
make
```

```
docker build -t ghcr.io/nathanjackson/connman-dhcp-client:latest .
```

```
docker push ghcr.io/nathanjackson/connman-dhcp-client:latest
```

Mark as public.

## Step 2. Fuzz the DHCP Client with Mayhem and Coverage Collection

```
mayhem init ghcr.io/nathanjackson/connman-dhcp-client:latest
```

Adjust Mayhemfile:
    * Set the Input Path
    * Set Duration
    * Enable Coverage Analysis

Populate `tests` dir.

```
mayhem run .
```

## Step 3. Identify Failing Check with lcov

```
mayhem sync .
```

```
genhtml -o /tmp/html --ignore-errors source line_coverage.lcov
```

Find the `check_package_owner` call, work backwards to see the xid fails.

Determine how `dhcp_client->xid` gets set.

Look at how `dhcp_get_random` works.

## Step 4. Override the Open System Call

Modify the Open System Call to open `/dev/zero` instead of `/dev/urandom`.

```
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
```

## Step 5. Fuzz Again, Find Defect

