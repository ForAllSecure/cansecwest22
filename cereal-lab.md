# Cereal Tutorial

## Overview

In this lab, you will create a libFuzzer harness against a version of cereal that is vulnerable to CVE-2020-11105 and also test the inputs and outputs for correctness.

## Part 1. Harnessing Cereal

**Time to complete**: About 10 minutes

### Step 1. Identify a Target

First we need to identify a target inside the Cereal codebase. There are a few ways to do this, but we can start by reading the documentation here: https://uscilab.github.io/cereal/.

After reading about Cereal, we find that it is a header-only C++11 library that provides serialization for arbitrary data types to binary, XML, or JSON.

So given that Cereal is a library for serialization, we'll want to test the encoding and decoding routines. There are multiple serialization formats we can test, but we'll start with JSON. In theory, we could write a test driver that exercises all of them!

Focusing on JSON, we find a minimal example of how to encode to JSON here : https://uscilab.github.io/cereal/serialization_archives.html

### Step 2. Fuzz the JSONOutputArchive

Let's modfiy the stubbed out harness in the `cansecwest22/cereal` folder.

To test this, we'll test the JSON serializer's ability to handle strings, so modify the `json_fuzzer.cpp` like so:

```
...
int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("usage: %s TEST\n", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1], std::ios::binary);
    std::istreambuf_iterator<char> begin{ifs}, end;
    std::string s(begin, end);

    std::stringstream ss;

    {
        cereal::JSONOutputArchive oa(ss);
        oa(s);
    }


    return 0;
}
...
```

Next, build the Docker image for the target:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest .
```

And of course, push to the registry:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest
```

Once you've pushed the image to the registry, be sure to mark its visibility to public. See: https://docs.github.com/en/packages/learn-github-packages/configuring-a-packages-access-control-and-visibility#configuring-visibility-of-container-images-for-your-personal-account

Now generate a `Mayhemfile`:

```
mayhem init ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest
```

Notice this time we provided a Docker image to `mayhem init`. We did this because the Mayhem CLI will automatically inspect a container to try to determine how to run your target. Inspect the resulting `Mayhemfile` with your favorite text editor.

You should see that most of the fields have been filled in for you automatically! We just need to tell Mayhem how to attack our target. So modify the command used to exercise your target in the `Mayhemfile` using your favorite text editor by adding the `@@` symbol:

```
...
  - cmd: /json_fuzzer @@
...
```

Now, start the run:

```
mayhem run .
```

You should see output that indicates a run was started successfully.

```
Run started: cereal-json/cereal-json-latest/1
Run URL: https://mayhem.forallsecure.com:443/nathanjackson/cereal-json/cereal-json-latest/1
cereal-json/cereal-json-latest/1
```

## Part 2. Improving our Fuzzer

Now that that you're successfully fuzzing cereal, you'd normally let this run for awhile, with the hope that you find a bug. What happens if you get to the point where you're not generating many new test cases and you haven't found any bugs?

Well, the next step is to look to improve your target. There are multiple ways to do this, and we'll implement two of them in this part. First, we'll convert our harness into a libFuzzer harness which will enable in-memory fuzzing. As a result, our target should run much faster. Secondly, we'll add a custom assertion that allows us to check for symmetry. In other words, we're checking to see if we can serialize and deserialize and expect to get the same input.

### Step 1. Convert our Existing Harness to use libFuzzer

First, we need adapt our harness to use libFuzzer. Fortunately, we'll be deleting code, so this step is pretty easy. Remember that libFuzzer harnesses deliver test cases via arguments to a function named `LLVMFuzzerTestOneInput`. So for this step, we'll rename our `main` function and remove the file IO code.

Change into `cansecwest22/cereal`:

    ```
    cd cansecwest22/cereal
    ```

Modify `json_fuzzer.cpp` and the main function for libFuzzer using your favorite text editor. It should look similar to this:

```
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    std::string s(data, data + size);
    std::stringstream ss;

    {
        cereal::JSONOutputArchive oa(ss);
        oa(s);
    }

    return 0;
}
```

Next we need to update the `Makefile` to add libFuzzer instrumentation to our target. We'll change the compiler from GCC to Clang, and add `-fsanitize=fuzzer`. The resulting Makefile should look similar to this:

```
json_fuzzer:
    clang++ -std=c++17 -c -Icereal/include -fsanitize=fuzzer json_fuzzer.cpp
    clang++ -fsanitize=fuzzer json_fuzzer.o -o json_fuzzer

clean:
    rm -f *.o json_fuzzer
```

### Step 2. Modify `json_harness.cpp` to deserialize the result and check for equality.

Now we need to add a step to deserialize the data, and check for equality. Modify `json_harness.cpp` to look similar to this:

```
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    std::string s(data, data + size);

    std::stringstream ss;

    std::string s2;

    // Note: we have to wrap writing to the output archive because the data
    // isn't flushed to stringstream until it goes out of scope.
    {
        cereal::JSONOutputArchive oa(ss);
        oa(s);
    }

    cereal::JSONInputArchive ia(ss);
    ia(s2);

    assert(s == s2);

    return 0;
}
```

### Step 3. Update the `Mayhemfile`

Since our Cereal harness no longer accepts file input, we need to update the Mayhemfile so that it doesn't pass the input via file. Update the `cmd` structure to remove the `@@`:

```
...
  - cmd: /json_fuzzer
...
```

### Step 3. Start a run against the updated target.

First, re-build the Docker image:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest .
```

Push to the registry:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest
```

You should not have to mark the package as public again.

Finally, start the run:

```
mayhem run .
```

You should see output that indicates the run started successfully:

```
Run started: cereal-json/cereal-json-latest/2
Run URL: https://mayhem.forallsecure.com:443/nathanjackson/cereal-json/cereal-json-latest/2
cereal-json/cereal-json-latest/2
```

Open the Mayhem UI, and you should see that Mayhem finds a defect within a few seconds due to an assertion error.

