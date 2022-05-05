# Lab 5: Finding CVE-2019-13103 - CVE-2019-13106

## Overview

This lab will show you how to find several vulnerabilities in U-Boot.

U-Boot is an open-source bootloader for embedded systems. U-Boot can be compiled into a Linux userspace program that allows us to test features without having an embedded device to run it on. As such, we can fuzz it in Mayhem!

We will also be making use of Mayhem's ability to use AFL targets as our version of U-Boot was compiled with AFL instrumentation.

**Time to complete**: About 10 minutes

## Step 1. Start Mayhem!

So the U-Boot vulnerabilities actually take several hours to find, longer than the time we have alloted for today's class. So we've saved the tests that cause the defects from a previous Mayhem run and we will load these into our target's test directory before starting so we can quickly reproduce the issue.

First, change into the u-boot directory:

```
cd uboot/
```

Copy the defect test cases into the tests directory:

```
cp defects/* tests/
```

Start Mayhem:

```
mayhem run .
```

You should see output that looks similar to the following:

```
Run started: uboot/ext4/1
Run URL: https://mayhem.forallsecure.com:443/nathanjackson/uboot/ext4/1
uboot/ext4/1
```

If you follow the Run URL, you should see that Mayhem starts finding defects shortly after the runs starts up.

## Step 2. Reproduce Locally

For this step, you have to be inside a local u-boot container. So first, start a container while you're in the uboot directory:

```
docker run -v $PWD:/uboot --rm -it forallsecure/uboot-cve-2019-13103-13106 bash
```

Now, we need to run u-boot just like Mayhem ran it, but we can change the file argument to point to one of the defects. For example:

```
/mayhem/u-boot-2019.07-rc4/u-boot -c "host bind 0 /uboot/defects/div0_1574 ; ls host 0"
```

You should see output like the following:

```
U-Boot 2019.07-rc4 (Mar 02 2021 - 17:29:10 +0000)

DRAM:  128 MiB
WDT:   Not found!
MMC:   
In:    serial
Out:   serial
Err:   serial
SCSI:  
Net:   No ethernet found.
Floating point exception (core dumped)

```

Congratulations! You just reproduced a CVE with Mayhem!
