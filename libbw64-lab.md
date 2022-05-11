# Lab 4: Fuzzing a Real Target

## Overview

This lab will show you how to package libbw64 and fuzz it with Mayhem.

**Time to complete**: About 10 minutes

## Step 1. Build libbw64

Enter the libbw64 directory:

```
cd libbw64/
```

Create a build directory and change into it:

```
mkdir build
cd build/
```

Run CMake to generate the build files:

```
cmake ..
```

Finally, build with `make`:

```
make
```

## Step 2. Identify Easy-to-Fuzz Targets

With libbw64 built, let's try to identify some easy-to-fuzz targets. Take a look at the build output from the step above, this gives us some clues about what the build process generated. You should see output that looks similar to this:

```
...
Scanning dependencies of target bw64_read_write
[  6%] Building CXX object examples/CMakeFiles/bw64_read_write.dir/bw64_read_write.cpp.o
[ 12%] Linking CXX executable bw64_read_write
[ 12%] Built target bw64_read_write
Scanning dependencies of target bw64_create_sine
[ 18%] Building CXX object examples/CMakeFiles/bw64_create_sine.dir/bw64_create_sine.cpp.o
[ 25%] Linking CXX executable bw64_create_sine
[ 25%] Built target bw64_create_sine
Scanning dependencies of target bw64_extract_adm
[ 31%] Building CXX object examples/CMakeFiles/bw64_extract_adm.dir/bw64_extract_adm.cpp.o
[ 37%] Linking CXX executable bw64_extract_adm
[ 37%] Built target bw64_extract_adm
...
```

The build output actually gives us some ideas for potential targets. Notice that several executables are built from the libbw64 example code: `bw64_read_write`, `bw64_create_sine`, `bw64_extract_adm`. Let's see if these executables make sense to fuzz. Remember, we want something that takes file or network input. If we see something like that, we should be able to start fuzzing without writing any code.

First, execute `bw64_read_write` to see if it gives us any clue how it gets used:

```
./examples/bw64_read_write
```

You should see output that looks similar to this:

```
usage: ./examples/bw64_read_write [INFILE] [OUTFILE]
```

Fortunately for us, `bw64_read_write` tells us its expecting an input and output file. So let's fuzz it!

## Step 3. Package bw64_read_write

Before we can start fuzzing `bw64_read_write`, we need to package it so Mayhem can ingest it. We'll use Docker for this!

Using your favorite text editor, create a Dockerfile in the root of your libbw64 repo with the following content:

```
FROM ubuntu:20.04

ADD ./build/examples/bw64_read_write /
```

Now, let's build an image that we can push to the GitHub container registry:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/bw64_read_write:latest .
```

Once the build completes, push your newly created image. Note that you should be logged in still to ghcr.io from a previous lab. If not, see this page: https://docs.github.com/en/packages/working-with-a-github-packages-registry/working-with-the-container-registry

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/bw64_read_write:latest
```

Finally, you need to mark the package as public. At this time, we're only allowing Mayhem Free users to analyze public images. Follow these instructions to change the package visibility:

https://docs.github.com/en/packages/learn-github-packages/configuring-a-packages-access-control-and-visibility#configuring-access-to-container-images-for-your-personal-account

## Step 4. Create a Folder for our target and Mayhemfile

Next, we need to create a Mayhemfile that tells Mayhem how to attack this application. We also need to create a folder where we'll keep the artifacts for our target.

Create a `mayhem` folder and change into it:

```
mkdir mayhem
cd mayhem/
```

Next, generate a new `Mayhemfile`:

```
mayhem init
```

Open the resulting `Mayhemfile` with a text editor and edit the `project`, `target`, `image`, and `cmd`. Your resulting Mayhemfile should effectively look something like this:

```
project: <YOUR GITHUB USERNAME>/libbw64
target: bw64-read-write

image: ghcr.io/<YOUR GITHUB USERNAME>/bw64_read_write:latest

cmds:
  - cmd: /bw64_read_write @@ /dev/null
```

The project and target fields are used to tell Mayhem how to organize the run. Typically, a project will map to a single repo, in this case, a GitHub repo. Target is usually mapped to a specific test driver or executable within a project. However, these fields are freeform. For another project or target, you should pick values that make sense.

The image field specifies which Docker image Mayhem should pull.

The `cmds` structure is what tells Mayhem how to attack the application. For this scenario, we want to analyze `/bw64_read_write` inside our Docker image that we built earlier. Remember that `bw64_read_write` takes two arguments: an input and output file. We can tell Mayhem how to pass a payload to the executable using `@@`. For the second argument, since we're fuzzing and don't particularly care about the output, we can simply pass `/dev/null` as the output argument.

## Step 5. Find Sample Inputs

Since libbw64 is a library for reading and writing a specialized version of WAV audio files. It makes sense to provide some sample WAV files up front that will allow us to penetrate deeper into the program early on in our fuzzing run.

libbw64 actually includes several WAV files in the test data directory that we can use for this purpose:

```
ls tests/test_data
```

So we'll just use these inputs as-is!

Copy the `test_data` folder to `mayhem/tests`:

```
cp -r tests/test_data mayhem/tests
```

## Step 6. Start the run

Back in the `mayhem` folder, now we can start the run using `mayhem run`:

```
mayhem run .
```

You should see output that looks similar to this:

```
Run started: libbw64/test/1
Run URL: https://mayhem.forallsecure.com:443/nathanjackson/libbw64/test/1
libbw64/test/1
```

If you open the run URL in your browser. You should see the run start and find a defect within a few seconds!

