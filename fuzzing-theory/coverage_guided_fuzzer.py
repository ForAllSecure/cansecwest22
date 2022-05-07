#!/usr/bin/env python3

import os
import random
import sys

def fuzzme_instrumented(data: bytes, coverage: set):
    add_coverage(coverage)
    if len(data) < 3:
        add_coverage(coverage)
        return
    add_coverage(coverage)
    if data[0] != 0x62: # 'b'
        add_coverage(coverage)
        return
    add_coverage(coverage)
    if data[1] != 0x75: # 'u'
        add_coverage(coverage)
        return
    add_coverage(coverage)
    if data[2] != 0x67: # 'g'
        add_coverage(coverage)
        return
    add_coverage(coverage)
    assert False # you found the bug!
   

def make_testcase():
    """
    Generates a random test case.
    """
    global test_suite
    # if the test suite is empty, generate a new one
    if not len(test_suite):
        length = random.randrange(1, 10)
        return os.urandom(length)

    # if the test suite is not empty, pick a random test case:
    tc = bytearray(random.choice(list(test_suite)))

    # replace a single byte:
    index = random.randrange(0, len(tc))
    tc[index] = random.randrange(0, 255)

    # append some random data to the test case
    append_length = random.randrange(1, 10)
    tc += os.urandom(append_length)

    return bytes(tc)

def add_coverage(cov: set):
    """
    Adds the calling line number to a coverage set.
    """
    # grab stackframe and find line number of caller function
    location = sys._getframe().f_back.f_lineno
    cov.add(location)

# Our main loop for fuzzing. Will continue running until a crash or you kill it.
run = True
iters = 0
total_coverage = set()
test_suite = set()
while run:

    iters += 1
    if not (iters % 100000):
        print("iterations: {}".format(iters))

    # Generate a test case:
    testcase = make_testcase()

    coverage = set()

    # Fuzz!
    try:
        fuzzme_instrumented(testcase, coverage)
        if 0 < len(coverage - total_coverage):
            print("Found new coverage, test case: {}".format(testcase))
            total_coverage.update(coverage)
            test_suite.add(testcase)
    except AssertionError:
        print("Crash detected, test case: {}".format(testcase))
        run = False

