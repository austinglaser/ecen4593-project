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
        self.__init__()
        cache_config_pattern   = r"""^{name} \s+                              # The cache's identifier
                                     size          \s+ = \s+ (\d+) \s+ : \s+  # The cache's size
                                     ways          \s+ = \s+ (\d+) \s+ : \s+  # The cache's associativity
                                     block \s size \s+ = \s+ (\d+) $          # The cache's block size
                                  """
        cache_value_pattern    = r"^Memory Level: {level}$"
        if cache_name == 'L1i':
            config_pattern = re.compile(cache_config_pattern.format(name='Icache'), re.X)
            value_pattern  = re.compile(cache_value_pattern.format(level='L1i'))
            cost_pattern   = re.compile("L1 cache cost \(Icache \$(\d+)\)")
        elif cache_name == 'L1d':
            config_pattern = re.compile(cache_config_pattern.format(name='Dcache'), re.X)
            value_pattern  = re.compile(cache_value_pattern.format(level='L1d'))
            cost_pattern   = re.compile("L1 cache cost \(Icache \$\d+\) \+ \(Dcache \$(\d+)\)")
        elif cache_name == 'L2':
            config_pattern = re.compile(cache_config_pattern.format(name='L2-cache'), re.X)
            value_pattern  = re.compile(cache_value_pattern.format(level='L2'))
            cost_pattern   = re.compile("L2 cache cost = \$(\d+)")
        else:
            raise ValueError("{name} is not a valid cache name".format(name=cache_name))

        hit_miss_pattern         = re.compile(r"Hit Count = (\d+)  Miss Count = (\d+)")
        rate_pattern             = re.compile(r"Hit Rate =\s+(\d+\.\d)%  Miss Rate =\s+(\d+\.\d)%")
        kickout_transfer_pattern = re.compile(r"Kickouts = (\d+); Dirty kickouts = (\d+); Transfers = (\d+)")
        vc_hit_pattern           = re.compile(r"VC Hit count = (\d+)")

        for i, line in enumerate(lines):
            config_match = re.match(config_pattern, line)
            value_match  = re.match(value_pattern, line)
            cost_match   = re.match(cost_pattern, line)
            if config_match:
                self.size       = int(config_match.group(1))
                self.ways       = int(config_match.group(2))
                self.block_size = int(config_match.group(3))
            if cost_match:
                self.cost       = int(cost_match.group(1))
            if value_match:
                hit_miss_match = re.match(hit_miss_pattern, lines[i+1])
                if hit_miss_match:
                    self.hit_count  = int(hit_miss_match.group(1))
                    self.miss_count = int(hit_miss_match.group(2))
                else:
                    raise RuntimeError("Bad sim result")
                rate_match = re.match(rate_pattern, lines[i+3])
                if rate_match:
                    self.hit_rate  = float(rate_match.group(1))
                    self.miss_rate = float(rate_match.group(2))
                else:
                    raise RuntimeError("Bad sim result")
                kickout_transfer_match = re.match(kickout_transfer_pattern, lines[i+4])
                if kickout_transfer_match:
                    self.kickouts       = int(kickout_transfer_match.group(1))
                    self.dirty_kickouts = int(kickout_transfer_match.group(2))
                    self.transfers      = int(kickout_transfer_match.group(3))
                else:
                    raise RuntimeError("Bad sim result")
                vc_hit_match = re.match(vc_hit_pattern, lines[i+5])
                if vc_hit_match:
                    self.victim_cache_hit_count = int(vc_hit_match.group(1))
                else:
                    raise RuntimeError("Bad sim result")

        values = [self.size, self.ways, self.block_size, self.hit_count,
                  self.miss_count, self.hit_rate, self.miss_rate, self.kickouts,
                  self.dirty_kickouts, self.transfers, self.victim_cache_hit_count, self.cost]

        if any(v is None for v in values):
            raise RuntimeError("Bad sim result")

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

