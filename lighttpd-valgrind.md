# Reproducing Advanced Triage Defects

## Overview

This lab will show you how to reproduce Advanced Triage defects with Valgrind.

**Time to complete**: About 5 minutes

## Step 1. Sync with Mayhem CLI

Change into the lighttpd folder in the cansecwest22 repo.

    ```
    cd cansecwest22/lighttpd
    ```

Sync the test cases (may not need to do this if you completed the earlier exercise).

    ```
    mayhem sync .
    ```

## Step 2. Start bash in the lighttpd container and install Valgrind and netcat

Next, we need to start the lighttpd container, but we're not going to start the server yet. Instead, we'll start a bash shell.

Launch a new container:

    ```
    docker run --rm --name lighttpd -it -v $PWD/tests:/tests forallsecure/lighttpd:vulnerable bash
    ```

Now, install Valgrind and netcat in the newly created shell:

    ```
    apt-get update && apt-get install -y valgrind netcat
    ```

## Step 3. Reproduce the vulnerabilities with lighttpd.

First, start lighttpd inside Valgrind and let it run in the background:

    ```
    valgrind --tool=memcheck --log-file='lighttpd-valgrind.log' /usr/local/sbin/lighttpd -D -f /usr/local/etc/lighttpd.conf &    
    ```

Next, feed a failing test case into lighttpd using netcat:

    ```
    nc 127.0.0.1 80 < /tests/ba0dbafbd0b787a564635b887f77926ae0b3f979dcc72d30cf7fdb1707581919
    ```

Stop valgrind by bringing it to the foreground and pressing CTRL-C:

    ```
    fg
    <CTRL-C>
    ```

View the valgrind output:

    ```
    less lighttpd-valgrind.log
    ```

