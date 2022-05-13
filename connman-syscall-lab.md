# ConnMan DHCP Client Lab

## Step 1. Build the Connman DHCP Client Harness Locally

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
    * Enable Coverage Analysis

```
mayhem run .
```

## Step 3. Identify Failing Check with lcov

## Step 4. Override the Open System Call

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

## Step 5. Fuzz Again

