#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 Austin Glaser <austin@boulderes.com>
#
# Distributed under terms of the MIT license.

"""
Statistics manipulation

Usage:
    stats.py <data_file> ...
"""

import docopt
import re

class Cache:

    def __init__(self, size=None, ways=None, block_size=None,
                 hit_count=None, victim_cache_hit_count=None, miss_count=None, hit_rate=None, miss_rate=None,
                 kickouts=None, dirty_kickouts=None, transfers=None,
                 cost=None,
                 lines=None, cache_name=None):
        if lines is not None:
            self.fromlines(lines, cache_name)
        else:
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

    def fromlines(self, lines, cache_name):
        cache_values_pattern = r"""^ {name} \s+                             # The cache's identifier
                                   size          \s+ = \s+ (\d+) \s+ : \s+  # The cache's size
                                   ways          \s+ = \s+ (\d+) \s+ : \s+  # The cache's associativity
                                   block \s size \s+ = \s+ (\d+) $          # The cache's block size
                                """
        if cache_name == 'L1i':
            config_pattern = re.compile(cache_values_pattern.format(name='Icache'), re.X)
            cost_pattern   = re.compile("L1 cache cost \(Icache \$(\d+)\)")
        elif cache_name == 'L1d':
            config_pattern = re.compile(cache_values_pattern.format(name='Dcache'), re.X)
            cost_pattern   = re.compile("L1 cache cost \(Icache \$\d+\) \+ \(Dcache \$(\d+)\)")
        elif cache_name == 'L2':
            config_pattern = re.compile(cache_values_pattern.format(name='L2-cache'), re.X)
            cost_pattern   = re.compile("L2 cache cost = \$(\d+)")
        else:
            raise ValueError("{name} is not a valid cache name".format(name=cache_name))

        for line in lines:
            cache_config_match = re.match(config_pattern, line)
            cost_match         = re.match(cost_pattern, line)
            if cache_config_match:
                self.size       = cache_config_match.group(1)
                self.ways       = cache_config_match.group(2)
                self.block_size = cache_config_match.group(3)
                print cache_name, "matched config"
            if cost_match:
                self.cost       = cost_match.group(1)
                print cache_name, "matched cost:"

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

    def __init__(self, trace, config,
                 memory_system, references, cycles):
        self.trace = trace
        self.config = config

        self.memory_system = memory_system
        self.references    = references
        self.cycles        = cycles

    def from_file(filename):
        pass

if __name__ == "__main__":
    arguments = docopt.docopt(__doc__)
    for filename in arguments['<data_file>']:
        with open(filename) as f:
            # Get all lines
            lines = map(str.strip, f.readlines())

            # Find the dividers
            divider_pattern = re.compile('^-+$')
            divider_lines = []
            for i, line in enumerate(lines):
                if re.match(divider_pattern, line):
                    divider_lines.append(i)

            # Get the trace's name
            name_line = divider_lines[0] + 1
            names = lines[name_line].split(' ')[0].split('.')
            trace = names[0]
            config = names[1]

            # Strip off header, cache contents, and empty lines
            lines = lines[divider_lines[1] + 1:divider_lines[2]]
            lines = [l for l in lines if l]

            # Build cache configs
            l1i_cache = Cache(lines=lines, cache_name='L1i')
            l1d_cache = Cache(lines=lines, cache_name='L1d')
            l2_cache  = Cache(lines=lines, cache_name='L2')

