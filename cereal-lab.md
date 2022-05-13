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

## Part 2. Convert to libFuzzer

## Part 3. Adding Additional Checks

Now that that you're successfully fuzzing cereal, you'd normally let this run for awhile, with the hope that you find a bug. What happens if you get to the point where you're not generating many new test cases and you haven't found any bugs?

Well, the next step is to look to improve your target. One way we can do that is to add additional checks to our target. Since cereal is a serialization library, it probably makes sense to verify that we can serialize and deserialize data and get the same result. So in this part of the lab, we'll add our own assertion to validate the symmetry (what goes in, must come out).

**Time to complete**: About 10 minutes

### Step 1. Modify `json_harness.cpp` to deserialize the result and check for equality.

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

### Step 2. Start a run against the updated target.

First, re-build the Docker image:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest .
```

Push to the registry:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/cereal-json:latest
```

You should not have to mark the package as public again.

Finally, start the run (your existing Mayhemfile should work just fine):

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
