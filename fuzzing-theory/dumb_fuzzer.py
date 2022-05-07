#!/usr/bin/env python3

import os
import random

# The function we want to fuzz. Also called a fuzz target.
def fuzzme(data: bytes):
    if len(data) < 3:
        return
    if data[0] != 0x62: # 'b'
        return
    if data[1] != 0x75: # 'u'
        return
    if data[2] != 0x67: # 'g'
        return
    assert False # you found the bug!

def make_testcase():
    """
    Generates a random test case.
    """
    length = random.randrange(1, 10)
    return os.urandom(length)

# Our main loop for fuzzing. Will continue running until a crash or you kill it.
run = True
iters = 0
while run:

    iters += 1
    if not (iters % 100000):
        print("iterations: {}".format(iters))

    # Generate a test case:
    testcase = make_testcase()

    # Fuzz!
    try:
        fuzzme(testcase)
    except:
        print("Crash detected, test case: {}".format(testcase))
        run = False

