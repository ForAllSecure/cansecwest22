# U-Boot Lab

## Overview

In this lab you will fuzz U-Boot to reproduce defects for known vulnerabilities with AFL and Mayhem.

**Time to complete**: About 10 minutes

### Step 1. Build and Push Docker Image

For this lab, we've already created a Dockerfile to build an AFL compiled U-Boot. Follow these instructions to build and push.

Change into `u-boot-2019.07-rc4`:

```
cd u-boot-2019.07-rc4/
```

Build the Docker image:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/u-boot:latest .
```

Push the Docker image:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/u-boot:latest
```

Mark the image as public on GitHub:

https://docs.github.com/en/packages/learn-github-packages/configuring-a-packages-access-control-and-visibility#configuring-visibility-of-container-images-for-your-personal-account

### Step 2. Launch on Mayhem

Next we'll create a `Mayhemfile` that describes how to analyze U-Boot.

First run `mayhem init`:

```
mayhem init
```

Then using your favorite text editor, update the resulting `Mayhemfile` so that it looks similar to the following:

```
project: u-boot
target: ext4

image: ghcr.io/<YOUR GITHUB USERNAME>/u-boot:latest

cmds:
  - cmd: /u-boot -c "host bind 0 /fs.ext4 ; ls host 0"
    filepath: /fs.ext4
```

Now start a run on Mayhem:

```
mayhem run .
```

### Step 3. Patch in AFL_INIT

Next, we're going to take advantage of AFL to increase our fuzzing throughput by delaying the point at which AFL forks the target process using `__AFL_INIT()`.

Using your favorite text editor, edit `cmd/host.c` to insert the following lines at the beginning of the `do_host_bind` function:

```
#ifdef __AFL_HAVE_MANUAL_CONTROL
  __AFL_INIT();
#endif
```

Build the Docker image:

```
docker build -t ghcr.io/<YOUR GITHUB USERNAME>/u-boot:latest .
```

Push the Docker image:

```
docker push ghcr.io/<YOUR GITHUB USERNAME>/u-boot:latest
```

### Step 4. Create a Better Seed

Next, we can create a better seed. Notice that we're analyzing the ext4 driver of U-Boot. So why not provide an ext4 disk image?

Create a new `tests` folder:

```
mkdir tests
```

Create an empty file:

```
fallocate -l 1M tests/seed
```

Create an ext4 file system in the empty file:

```
mkfs.ext4 tests/seed
```

### Step 5. Re-launch on Mayhem

Re-run on Mayhem:

```
mayhem run .
```

