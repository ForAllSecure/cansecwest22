# Connman DNS Proxy Lab

In this lab, we'll patch the Connman DNS Proxy code to find the TBONE DNS vulnerability.

## Step 1. Identify DNS Handling Code

First we need to identify the changes we need to make in the codebase for fuzzing.

We know that DNS generally operates over UDP, and if you look in `src/dnsproxy.c`, you'll find a function that calls `recv` called `udp_server_event`.

There, you can see that DNS packets are passed directly to `forward_dns_reply`, so we'll target this function.

## Step 2. Create a libFuzzer Entrypoint

First, we need to disable the built-in Connman main function because we'll be using libFuzzer. libFuzzer provides its own main function, so we only want to include Connman's main function if we're not fuzzing. We can do this by surrounding the main function in `src/main.c` with `#ifndef FUZZ`.

Next, let's create an empty `LLVMFuzzerTestOneInput` function in src/dnsproxy.c`. The function should look like this:

```
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t sz)
{
    return 0;
}
```

Finally, we need to update `Makefile.am` so the CFLAGS for `connmand` will define `FUZZ` and enable AddressSanitizer and libFuzzer instrumentation.

# Step 3. Remove Stateful Code

Next we need to remove some stateful code in `src/dnsproxy.c`. If we left this code in, we'll never reach the code that triggers the bad memcpy.

Comment Out Stateful Code in `src/dnsproxy.c`.

See the included patch file for hints.

## Step 3. Build and Fuzz

Next, let's build everything locally:

```
./bootstrap
CC=clang ./configure
make
```

Build the Docker image:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/connman-dnsproxy:latest -f dnsproxy/Dockerfile .
```

Push the Docker image:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/connman-dnsproxy:latest
```

Be sure to mark the package as public in GitHub.

## Step 4. Run on Mayhem

Next, let's create a new Mayhemfile. `mayhem init` should generate a Mayhemfile that doesn't require any changes!

```
mkdir dnsproxy
```

```
mayhem init -o dnsproxy/Mayhemfile ghcr.io/<YOUR GITHUB USERNAME>/connman-dnsproxy:latest
```

```
mayhem run dnsproxy
```

