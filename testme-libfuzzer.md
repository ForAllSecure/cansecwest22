# libFuzzer Tutorial

## Overview

In this lab, you will run a simple libFuzzer target locally, and on Mayhem.

**Time to complete**: About 10 minutes

## Step 1. Compile testme with libFuzzer instrumentation

First we need to build our libFuzzer target. For this, all we need is a Clang with libFuzzer.

Change into the `testme-libfuzzer` folder inside cansecwest22:

```
cd testme-libfuzzer
```

Build `testme.c` with `clang` and enabling libFuzzer:

```
clang -fsanitize=fuzzer testme.c -o testme
```

(Optional) Verify that libFuzzer is inside our binary.

```
objdump -t testme | grep libfuzzer_is_present
```

You should see output that looks similar to this:

```
000000000042bdf0 l     F .text	0000000000000006              .hidden __libfuzzer_is_present
```

## Step 2. Fuzz with libFuzzer!

Starting a libFuzzer target is really easy, just run the binary:

```
./testme
```

Unlike AFL, libFuzzer doesn't give you a pretty TUI to look at. Instead you'll see log output. However, you should find a crash within almost immediately after starting. libFuzzer will actually show you the crashing input as well, so you should see output that looks similar to this:

```
INFO: Seed: 2036655530
INFO: Loaded 1 modules   (6 inline 8-bit counters): 6 [0x4e8040, 0x4e8046), 
INFO: Loaded 1 PC tables (6 PCs): 6 [0x4be930,0x4be990), 
INFO: -max_len is not provided; libFuzzer will not generate inputs larger than 4096 bytes
INFO: A corpus is not provided, starting from an empty corpus
#2	INITED cov: 3 ft: 4 corp: 1/1b exec/s: 0 rss: 24Mb
#1221	NEW    cov: 4 ft: 5 corp: 2/6b lim: 14 exec/s: 0 rss: 24Mb L: 5/5 MS: 4 ShuffleBytes-CMP-ChangeBit-CMP- DE: "\xfe\xff\xff\xff"-"b\x00\x00\x00"-
#1297	REDUCE cov: 4 ft: 5 corp: 2/4b lim: 14 exec/s: 0 rss: 24Mb L: 3/3 MS: 1 EraseBytes-
#1353	REDUCE cov: 4 ft: 5 corp: 2/3b lim: 14 exec/s: 0 rss: 24Mb L: 2/2 MS: 1 EraseBytes-
#1474	REDUCE cov: 4 ft: 5 corp: 2/2b lim: 14 exec/s: 0 rss: 24Mb L: 1/1 MS: 1 CrossOver-
#15941	REDUCE cov: 5 ft: 6 corp: 3/7b lim: 156 exec/s: 0 rss: 24Mb L: 5/5 MS: 2 CopyPart-CMP- DE: "u\x00\x00\x00"-
#16082	REDUCE cov: 5 ft: 6 corp: 3/6b lim: 156 exec/s: 0 rss: 24Mb L: 4/4 MS: 1 EraseBytes-
#16568	REDUCE cov: 5 ft: 6 corp: 3/5b lim: 156 exec/s: 0 rss: 24Mb L: 3/3 MS: 1 EraseBytes-
#17265	REDUCE cov: 5 ft: 6 corp: 3/4b lim: 156 exec/s: 0 rss: 24Mb L: 2/2 MS: 2 ChangeBinInt-EraseBytes-
==1494418== ERROR: libFuzzer: deadly signal
    #0 0x4adec0 in __sanitizer_print_stack_trace (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4adec0)
    #1 0x45a1c8 in fuzzer::PrintStackTrace() (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x45a1c8)
    #2 0x43f313 in fuzzer::Fuzzer::CrashCallback() (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x43f313)
    #3 0x7f5cb6d083bf  (/lib/x86_64-linux-gnu/libpthread.so.0+0x143bf)
    #4 0x7f5cb69c903a in __libc_signal_restore_set /build/glibc-sMfBJT/glibc-2.31/signal/../sysdeps/unix/sysv/linux/internal-signals.h:86:3
    #5 0x7f5cb69c903a in raise /build/glibc-sMfBJT/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:48:3
    #6 0x7f5cb69a8858 in abort /build/glibc-sMfBJT/glibc-2.31/stdlib/abort.c:79:7
    #7 0x4ae282 in fuzzme (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4ae282)
    #8 0x4ae300 in LLVMFuzzerTestOneInput (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4ae300)
    #9 0x4409d1 in fuzzer::Fuzzer::ExecuteCallback(unsigned char const*, unsigned long) (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4409d1)
    #10 0x440115 in fuzzer::Fuzzer::RunOne(unsigned char const*, unsigned long, bool, fuzzer::InputInfo*, bool*) (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x440115)
    #11 0x4423b7 in fuzzer::Fuzzer::MutateAndTestOne() (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4423b7)
    #12 0x4430b5 in fuzzer::Fuzzer::Loop(std::__Fuzzer::vector<fuzzer::SizedFile, fuzzer::fuzzer_allocator<fuzzer::SizedFile> >&) (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x4430b5)
    #13 0x431a6e in fuzzer::FuzzerDriver(int*, char***, int (*)(unsigned char const*, unsigned long)) (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x431a6e)
    #14 0x45a8b2 in main (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x45a8b2)
    #15 0x7f5cb69aa0b2 in __libc_start_main /build/glibc-sMfBJT/glibc-2.31/csu/../csu/libc-start.c:308:16
    #16 0x40680d in _start (/home/nathan/src/cansecwest22/testme-libfuzzer/testme+0x40680d)

NOTE: libFuzzer has rudimentary signal handlers.
      Combine libFuzzer with AddressSanitizer or similar for better crash reports.
SUMMARY: libFuzzer: deadly signal
MS: 1 InsertByte-; base unit: 6721d9fb11ca730b528749d7e4f9e8c52766e450
0x62,0x75,0x67,
bug
artifact_prefix='./'; Test unit written to ./crash-6885858486f31043e5839c735d99457f045affd0
Base64: YnVn
```

You should also have a file that contains the crashing test case in your current directory.

## Step 3. Replay the crash.

Now we'll use the test case that libFuzzer wrote to a file to feed back into our executable.

Feed the crashing test case to `testme`, note the file name maybe different for you:

```
./testme ./crash-6885858486f31043e5839c735d99457f045affd0
```

You should see that libFuzzer caught a deadly signal.

## Step 4. Package the Target with Docker

We can also analyze libFuzzer targets in Mayhem.

First, we need to create a Docker image that contains our testme executable. In your favorite text editor, create a `Dockerfile` with the following contents:

```
FROM --platform=linux/amd64 ubuntu:20.04 as builder
RUN apt-get update && apt-get install -y clang
ADD testme.c /
RUN clang -fsanitize=fuzzer testme.c -o testme

FROM --platform=linux/amd64 ubuntu:20.04
COPY --from=builder /testme /
```

Next, build the Docker image (be sure edit the command with your github username):

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/testme-libfuzzer:latest .
```

Before running the next step, you may need to login to `ghcr.io`. Please follow these instructions to login if you haven't already:

https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-container-registry#authenticating-to-the-container-registry

After logging in, push your image:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/testme-libfuzzer:latest
```

Once you've pushed your image, you'll need to mark is as public (so Mayhem Free can see it):

https://docs.github.com/en/packages/learn-github-packages/configuring-a-packages-access-control-and-visibility#configuring-visibility-of-container-images-for-your-personal-account

## Step 5. Fuzz with Mayhem

Now that we've packaged our target. We'll fuzz it with Mayhem!

First, create an initial `Mayhemfile`:

```
mayhem init
```

In your favorite text editor, modify the `Mayhemfile` so that it looks similar to this:

```
project: testme

target: testme-libfuzzer

image: ghcr.io/<YOUR GITHUB USERNAME>/libfuzzer-testme:latest

advanced_triage: false

cmds:

  - cmd: /testme
```

Note that in our `Mayhemfile`, we don't specify a file input. This is because libFuzzer targets pass the test case via function parameters.

Now start a new run on Mayhem:

```
mayhem run .
```

You should see output that looks similar to this:

```
Run started: testme/testme-libfuzzer/1
Run URL: https://mayhem.forallsecure.com:443/nathanjackson/testme/testme-libfuzzer/1
testme/testme-libfuzzer/1
```

Copy the Run URL link and paste it into your web browser.

Once the run starts, you should see a crash in the Mayhem UI within a minute or so of starting.

