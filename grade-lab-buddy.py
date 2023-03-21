#!/usr/bin/env python

import re
from gradelib import *

r = Runner(save("xv6.out"))

@test(10, "buddy_allocator, test get buddy")
def test_get_buddy():
    r.run_qemu(shell_script(['buddy_test 0']))
    r.match("^test_get_buddy\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(20, "buddy_allocator, test basic alloc")
def test_basic_alloc():
    r.run_qemu(shell_script(['buddy_test 1']))
    r.match("^test_basic_alloc\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(20, "buddy_allocator, test basic free")
def test_basic_free():
    r.run_qemu(shell_script(['buddy_test 2']))
    r.match("^test_basic_free\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(15, "buddy_allocator, test wrong size coalesce")
def test_wrong_size_coalesce():
    r.run_qemu(shell_script(['buddy_test 3']))
    r.match("^test_wrong_size_coalesce\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(10, "buddy_allocator, test gracefully run out of memory")
def test_gracefully_run_out_of_memory():
    r.run_qemu(shell_script(['buddy_test 4']))
    r.match("^test_gracefully_run_out_of_memory\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(10, "buddy_allocator, test stress big heap chunks")
def test_stress_big_heap_chunks():
    r.run_qemu(shell_script(['buddy_test 5']))
    r.match("^test_stress_big_heap_chunks\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

@test(15, "buddy_allocator, test stress overlapping")
def test_stress_overlapping():
    r.run_qemu(shell_script(['buddy_test 6']))
    r.match("^test_stress_overlapping\\(\\d+\\): OK\.",
            no=[".*Assertion FAILED.*"])

run_tests()
