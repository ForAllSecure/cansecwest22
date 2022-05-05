# Lab 5: Finding CVE-2019-13103 - CVE-2019-13106

## Overview

This lab will show you how to find several vulnerabilities in U-Boot.

U-Boot is an open-source bootloader for embedded systems. U-Boot can be compiled into a Linux userspace program that allows us to test features without having an embedded device to run it on. As such, we can fuzz it in Mayhem!

We will also be making use of Mayhem's ability to use AFL targets as our version of U-Boot was compiled with AFL instrumentation.

**Time to complete**: About 10 minutes

## Step 1. Start Mayhem!

So the U-Boot vulnerabilities actually take several hours to find, longer than the time we have alloted for today's class. So we've saved the tests that cause the defects and will load these into our target's test directory before starting so we can quickly reproduce the issue.

First, change into the u-boot directory:

```
cd uboot/
```

Copy the defect test cases into the tests directory:

```
cp defects/* tests/
```

Start Mayhem!

```
mayhem run .
```
