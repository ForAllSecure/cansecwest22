# Lab 2: Reproducing Defects Locally

## Overview

This lab will show you how to reproduce a defect discovered by Mayhem.

**Time to complete**: About 5 minutes

## Step 1. Sync with Mayhem CLI

Assuming you're still in the lighttpd folder from the last exercise, download the test cases with the Mayhem CLI:

    ```
    mayhem sync .
    ```

After syncing with the Mayhem CLI, list the contents of the directory.

    ```
    ls
    ```

You should see some additional folders, in particular, the tests and defects folders:

    ```
    ...
    tests
    defects
    ...
    ```

## Step 2. Start the lighttpd run locally.

Using Docker, we can start the same lighttpd Docker image locally:

```
docker run --rm -p 8080:80 --name lighttpd -d forallsecure/lighttpd:vulnerable
```

You can verify that the lighttpd image is running locally using `docker ps`.

```
docker ps
```

You should see output similar to the above:

```
CONTAINER ID   IMAGE                              COMMAND                  CREATED          STATUS          PORTS                                   NAMES
6b10b9c09fcf   forallsecure/lighttpd:vulnerable   "/usr/local/sbin/lig…"   28 seconds ago   Up 26 seconds   0.0.0.0:8080->80/tcp, :::8080->80/tcp   lighttpd
```

From here we can see that lighttpd successfully started.

## Step 3. Send lighttpd a test case that demonstrates the crash.

With lighttpd running, we'll use netcat to send one of the test cases that demonstrates a defect to lighttpd to crash it.

```
nc 127.0.0.1 8080 < ./defects/ba0dbafbd0b787a564635b887f77926ae0b3f979dcc72d30cf7fdb1707581919
```

Now, check to see if the lighttpd container is still running:

```
docker ps
```

You should see that the lighttpd container is now gone because it crashed!
