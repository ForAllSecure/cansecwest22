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
    length = random.randrange(1, 10)
    return os.urandom(length)

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
    except:
        print("Crash detected, test case: {}".format(testcase))
        run = False

