#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 Austin Glaser <austin@boulderes.com>
#
# Distributed under terms of the MIT license.

"""
Statistics manipulation
"""

class Cache:

    def __init__(self, size, ways, block_size,
                 hit_count, victim_cache_hit_count, miss_count, hit_rate, miss_rate, 
                 kickouts, dirty_kickouts, transfers,
                 cost):
        self.size                   = size
        self.ways                   = ways
        self.block_size             = block_size

        self.hit_count              = hit_count
        self.victim_cache_hit_count = victim_cache_hit_count
        self.miss_count             = miss_count
        self.hit_rate               = hit_rate
        self.miss_rate              = miss_rate

        self.kickouts               = kickouts
        self.dirty_kickouts         = dirty_kickouts
        self.transfers              = transfers

        self.cost                   = cost

class MainMem:

    def __init__(self, memory_ready_time, chunksize, chunktime, cost):
        self.memory_ready_time = memory_ready_time
        slef.chunksize         = chunksize
        self.chunktime         = chunktime

        self.cost              = cost

class MemorySystem:

    def __init__(self, l1d_cache, l1i_cache, l2_cache, main_mem):
        self.l1d_cache = l1d_cache
        self.l1i_cache = l1i_cache
        self.l2_cache  = l2_cache
        self.main_mem  = main_mem

class References:

    def __init__(self, reads, writes, instrs):
        self.reads  = reads
        self.writes = reads
        self.instrs = instrs 

class Cycles:

    def __init__(self, reads, writes, instrs, ideal, ideal_misaligned,
                 cpi, cpi_ideal, cpi_ideal_misaligned):
        self.reads                = reads
        self.writes               = writes
        self.instrs               = instrs

        self.ideal                = ideal
        self.ideal_misaligned     = ideal_misaligned

        self.cpi                  = cpi
        self.cpi_ideal            = cpi_ideal
        self.cpi_ideal_misaligned = cpi_ideal_misaligned

class Result:

    def __init__(self, memory_system, references, cycles):
        self.memory_system = memory_system
        self.references    = references
        self.cycles        = cycles

    def from_file(filename):
        pass

if __name__ == "__main__":
    pass
