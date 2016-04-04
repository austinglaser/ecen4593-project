#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 Austin Glaser <austin@boulderes.com>
#
# Distributed under terms of the MIT license.

"""
Statistics manipulation and calculation
"""

def prepend_to_each_line(in_str, prefix="    "):
    in_str_lines = in_str.split('\n')
    out_str_lines = [prefix + line for line in in_str_lines]
    return "\n".join(out_str_lines)

class TypeCounter:

    def __init__(self, read, write, instr):
        self.read = read
        self.write = write
        self.instr = instr
        self.total = read + write + instr

    def percentage(self, stat):
        return float(stat) / float(self.total) * 100

    def __repr__(self):
        repr_str = "TypeCounter(read={read}, write={write}, instr={instr}, total={total})"
        return repr_str.format(read  = self.read,
                               write = self.write,
                               instr = self.instr,
                               total = self.total)

    def __str__(self):
        str_str = ( "Reads  = {read:12}    [{rp:2.1f}%]\n" +
                    "Writes = {write:12}    [{wp:2.1f}%]\n" +
                    "Inst.  = {instr:12}    [{ip:2.1f}%]\n" +
                    "Total  = {total:12}" )
        return str_str.format(read  = self.read, rp = self.percentage(self.read),
                              write = self.write, wp = self.percentage(self.write),
                              instr = self.instr, ip = self.percentage(self.instr),
                              total = self.total)

class Statistics:

    def __init__(self, counts, cycles):
        self.counts    = counts
        self.cycles    = cycles
        self.cpi       = float(self.cycles.total) / float(self.counts.instr)

    def __repr__(self):
        repr_str = "Statistics(counts={counts}, cycles={cycles})"
        return repr_str.format(counts = repr(self.counts),
                               cycles = repr(self.cycles))

    def __str__(self):
        counts_str = prepend_to_each_line(str(self.counts))
        cycles_str = prepend_to_each_line(str(self.cycles))
        str_str = ( "Number of reference types:\n"      +
                    "{counts}\n"                        +
                    "\n"                                +
                    "Total cycles for activities:\n"    +
                    "{cycles}\n"                        +
                    "\n"                                +
                    "CPI = {cpi:2.1f}" )
        return str_str.format(counts = counts_str,
                              cycles = cycles_str,
                              cpi    = self.cpi)


if __name__ == "__main__":
    ref_counter   = TypeCounter(940933865,  374031357,  3685034778)
    cycle_counter = TypeCounter(6362234282, 5668329310, 18577582319)
    s = Statistics(ref_counter, cycle_counter)

    print
    print s
    print
