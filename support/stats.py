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

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

class MetricContainer:

    def __check_all_not_none__(self):
        if any(value is None for key, value in self.__dict__.iteritems()):
            raise RuntimeError("Bad sim result")

class Cache(MetricContainer):

    def __init__(self, lines, cache_name):
        self.__make_all_none__()
        self.name = cache_name

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

        value_header_line = None
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
                value_header_line = i

        hit_miss_pattern         = re.compile(r"Hit Count = (\d+)  Miss Count = (\d+)")
        rate_pattern             = re.compile(r"Hit Rate =\s+(\d+\.\d)%  Miss Rate =\s+(\d+\.\d)%")
        kickout_transfer_pattern = re.compile(r"Kickouts = (\d+); Dirty kickouts = (\d+); Transfers = (\d+)")
        vc_hit_pattern           = re.compile(r"VC Hit count = (\d+)")

        if not value_header_line is None:
            hit_miss_match = re.match(hit_miss_pattern, lines[value_header_line + 1])
            rate_match = re.match(rate_pattern, lines[value_header_line + 3])
            kickout_transfer_match = re.match(kickout_transfer_pattern, lines[value_header_line + 4])
            vc_hit_match = re.match(vc_hit_pattern, lines[value_header_line + 5])
            if hit_miss_match:
                self.hit_count  = int(hit_miss_match.group(1))
                self.miss_count = int(hit_miss_match.group(2))
            if rate_match:
                self.hit_rate  = float(rate_match.group(1))
                self.miss_rate = float(rate_match.group(2))
            if kickout_transfer_match:
                self.kickouts       = int(kickout_transfer_match.group(1))
                self.dirty_kickouts = int(kickout_transfer_match.group(2))
                self.transfers      = int(kickout_transfer_match.group(3))
            if vc_hit_match:
                self.victim_cache_hit_count = int(vc_hit_match.group(1))

        self.__check_all_not_none__()


    def __make_all_none__(self):
        self.name                   = None

        self.size                   = None
        self.ways                   = None
        self.block_size             = None

        self.hit_count              = None
        self.victim_cache_hit_count = None
        self.miss_count             = None
        self.hit_rate               = None
        self.miss_rate              = None

        self.kickouts               = None
        self.dirty_kickouts         = None
        self.transfers              = None

        self.cost                   = None


    def __str__(self):
        return "{name}: {size} bytes, {ways} way, with {block_size} byte blocks".format(**self.__dict__)

    def __repr__(self):
        return "Cache(name={name},size={size},ways={ways},block_size={block_size},cost={cost})".format(**self.__dict__)


class MainMem(MetricContainer):

    def __init__(self, lines):
        self.__make_all_none__()
        config_pattern = re.compile(r"Memory ready time = (\d+) : chunksize = (\d+) : chunktime = (\d+)")
        cost_pattern   = re.compile(r".*Memory cost = \$(\d+)")

        for line in lines:
            config_match = re.match(config_pattern, line)
            cost_match = re.match(cost_pattern, line)
            if config_match:
                self.memory_ready_time = int(config_match.group(1))
                self.chunksize         = int(config_match.group(2))
                self.chunktime         = int(config_match.group(3))
            if cost_match:
                self.cost = cost_match.group(1)

        self.__check_all_not_none__()

    def __make_all_none__(self):
        self.memory_ready_time = None
        self.chunksize         = None
        self.chunktime         = None

        self.cost              = None

    def __str__(self):
        return "Main memory: {memory_ready_time} ready cycles, {chunksize} byte chunks in {chunktime} cycles".format(**self.__dict__)

    def __repr__(self):
        return "MainMem(memory_ready_time={memory_ready_time},chunksize={chunksize},chunktime={chunktime},cost={cost})".format(**self.__dict__)


class References(MetricContainer):

    def __init__(self, lines):
        self.__make_all_none__()

        header_pattern = re.compile(r"Number of reference types:")

        header_line = None
        for i, line in enumerate(lines):
            if re.match(header_pattern, line):
                header_line = i
                break

        reference_pattern = re.compile(r"\w+\.?\s+=\s+(\d+)\s+\[\s?(\d+\.\d)%\]")
        read_match = re.match(reference_pattern, lines[header_line + 1])
        write_match = re.match(reference_pattern, lines[header_line + 2])
        instr_match = re.match(reference_pattern, lines[header_line + 3])

        if read_match:
            self.reads        = int(read_match.group(1))
            self.read_percent = float(read_match.group(2))
        if write_match:
            self.writes        = int(write_match.group(1))
            self.write_percent = float(write_match.group(2))
        if instr_match:
            self.instrs        = int(instr_match.group(1))
            self.instr_percent = float(instr_match.group(2))

        self.__check_all_not_none__()

    def __make_all_none__(self):
        self.reads         = None
        self.writes        = None
        self.instrs        = None

        self.read_percent  = None
        self.write_percent = None
        self.instr_percent = None

    def total(self):
        return self.reads + self.writes + self.instrs

    def __str__(self):
        return ("Reference counts: {reads} Reads ({read_percent}%), " +
                "{writes} Writes ({write_percent}%), " +
                "{instrs} Instructions ({instr_percent}%)").format(**self.__dict__)

    def __repr__(self):
        return ("References(reads={reads},writes={writes},instrs={instrs}," +
                "read_percent={read_percent},write_percent={write_percent},instr_percent={instr_percent})").format(**self.__dict__)

class Cycles(MetricContainer):

    def __init__(self, lines):
        self.__make_all_none__()

        header_pattern   = re.compile(r"Total cycles for activities:")
        cpi_pattern      = re.compile(r"CPI =\s+(\d+\.\d)")
        ideal_pattern    = re.compile(r"Ideal:\s+Exec\. Time = \s+(\d+); CPI =\s+(\d+\.\d)")
        ideal_ma_pattern = re.compile(r"Ideal mis-aligned:\s+Exec\. Time = \s+(\d+); CPI =\s+(\d+\.\d)")

        header_line = None
        for i, line in enumerate(lines):
            if re.match(header_pattern, line):
                header_line = i

            cpi_match = re.match(cpi_pattern, line)
            ideal_match = re.match(ideal_pattern, line)
            ideal_ma_match = re.match(ideal_pattern, line)
            if cpi_match:
                self.cpi = float(cpi_match.group(1))
            if ideal_match:
                self.ideal     = int(ideal_match.group(1))
                self.cpi_ideal = float(ideal_match.group(2))
            if ideal_ma_match:
                self.ideal_misaligned     = int(ideal_ma_match.group(1))
                self.cpi_ideal_misaligned = float(ideal_ma_match.group(2))

        reference_pattern = re.compile(r"\w+\.?\s+=\s+(\d+)\s+\[\s?(\d+\.\d)%\]")
        read_match = re.match(reference_pattern, lines[header_line + 1])
        write_match = re.match(reference_pattern, lines[header_line + 2])
        instr_match = re.match(reference_pattern, lines[header_line + 3])
        if read_match:
            self.reads        = int(read_match.group(1))
            self.read_percent = float(read_match.group(2))
        if write_match:
            self.writes        = int(write_match.group(1))
            self.write_percent = float(write_match.group(2))
        if instr_match:
            self.instrs        = int(instr_match.group(1))
            self.instr_percent = float(instr_match.group(2))

        self.__check_all_not_none__()

    def __make_all_none__(self):
        self.reads                = None
        self.writes               = None
        self.instrs               = None

        self.read_percent         = None
        self.write_percent        = None
        self.instr_percent        = None

        self.ideal                = None
        self.ideal_misaligned     = None

        self.cpi                  = None
        self.cpi_ideal            = None
        self.cpi_ideal_misaligned = None

    def total(self):
        return self.reads + self.writes + self.instrs

    def __str__(self):
        return ("Cycle counts: {reads} Reads ({read_percent}%), " +
                "{writes} Writes ({write_percent}%), " +
                "{instrs} Instructions ({instr_percent}%)").format(**self.__dict__)

    def __repr__(self):
        return ("Cycles(reads={reads},writes={writes},instrs={instrs}," +
                "read_percent={read_percent},write_percent={write_percent},instr_percent={instr_percent})").format(**self.__dict__)

class MemorySystem:

    def __init__(self, l1d_cache, l1i_cache, l2_cache, main_mem):
        self.l1d_cache = l1d_cache
        self.l1i_cache = l1i_cache
        self.l2_cache  = l2_cache
        self.main_mem  = main_mem

    def __str__(self):
        return ("Memory system:\n" +
                "    " + str(self.l1d_cache) + "\n" +
                "    " + str(self.l1i_cache) + "\n" +
                "    " + str(self.l2_cache) + "\n" +
                "    " + str(self.main_mem))

    def __repr__(self):
        return ("MemorySystem(l1d_cache={l1d_cache}," +
                             "l1i_cache={l1i_cache}," +
                             "l2_cache={l2_cache}," +
                             "main_mem={main_mem},").format(l1d_cache=repr(self.l1d_cache),
                                                            l1i_cache=repr(self.l1i_cache),
                                                            l2_cache=repr(self.l2_cache),
                                                            main_mem=repr(self.main_mem))

class Result:

    def __init__(self, trace, config,
                 memory_system, references, cycles):
        self.trace = trace
        self.config = config

        self.memory_system = memory_system
        self.references    = references
        self.cycles        = cycles


    def __str__(self):
        return "Result({trace}, {config})".format(**self.__dict__)

    def __repr__(self):
        return "Result(trace={trace}, config={config})".format(trace=repr(self.trace),
                                                               config=repr(self.config))
        #return ("Result(trace={trace},config={config}," +
        #        "memory_system={memory_system}," +
        #        "references={references}" + 
        #        "cycles={cycles}").format(trace=repr(trace),config=repr(config),
        #                                  memory_system=repr(self.memory_system),
        #                                  references=repr(self.references),
        #                                  cycles=repr(self.cycles))

def get_stripped_lines(f):
    return map(str.strip, f.readlines())

def get_divider_line_indices(lines, div_re=r'^-+$'):
    divider_pattern = re.compile('^-+$')
    divider_lines = []
    for i, line in enumerate(lines):
        if re.match(divider_pattern, line):
            divider_lines.append(i)
    return divider_lines

def get_trace_config(lines, divider_lines):
    name_line = divider_lines[0] + 1
    names = lines[name_line].split(' ')[0].split('.')
    return names[0], names[1]

def strip_header_footer_empty(lines, divider_lines):
    lines = lines[divider_lines[1] + 1:divider_lines[2]]
    lines = [l for l in lines if l]
    return lines

def plot_results(traces, configs,
                 title, independent_name, dependent_name,
                 independent_getter, dependent_getter, log_x=False, x_limits=None):
    independent = {}
    dependent = {}
    for trace in traces:
        independent[trace] = []
        dependent[trace] = []
        for config in configs:
            r = results[config][trace]
            independent[trace].append(independent_getter(r))
            dependent[trace].append(dependent_getter(r))

        plt.plot(independent[trace], dependent[trace], label=trace, marker='o', linestyle=' ')
    plt.xlabel(independent_name)
    plt.ylabel(dependent_name)
    plt.title(title)
    ax = plt.gca()
    handles, labels = ax.get_legend_handles_labels()
    ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(.5, -0.1), ncol=3)
    plt.gcf().subplots_adjust(bottom=0.2)
    if log_x:
        ax.set_xscale('log')
    if x_limits:
        plt.xlim(x_limits)
    plt.show()

if __name__ == "__main__":
    arguments = docopt.docopt(__doc__)
    results = {}
    for filename in arguments['<data_file>']:
        with open(filename) as f:
            lines           = get_stripped_lines(f)
            divider_lines   = get_divider_line_indices(lines)
            trace, config   = get_trace_config(lines, divider_lines)
            lines           = strip_header_footer_empty(lines, divider_lines)

            # Build memory system
            l1d_cache = Cache(lines, 'L1d')
            l1i_cache = Cache(lines, 'L1i')
            l2_cache  = Cache(lines, 'L2')
            main_mem  = MainMem(lines)
            memory_system = MemorySystem(l1d_cache, l1i_cache, l2_cache, main_mem)

            # Build metrics
            references = References(lines)
            cycles = Cycles(lines)

            # Build full result
            result = Result(trace, config, memory_system, references, cycles)

            if result.config not in results:
                results[config] = {}
            results[config][trace] = result

    #  Result structure:
    #
    #   memory_system               (all memory system info)
    #     l1d_cache                   (data cache)
    #       size                        (total cache size)
    #       ways                        (cache associativity)
    #       block_size
    #       hit_count
    #       victim_cache_hit_count
    #       miss_count
    #       hit_rate
    #       miss_rate
    #       cost
    #     l1i_cache                   (instruction cache)
    #       size                        (total cache size)
    #       ways                        (cache associativity)
    #       block_size
    #       hit_count
    #       victim_cache_hit_count
    #       miss_count
    #       hit_rate
    #       miss_rate
    #       cost
    #     l2_cache                    (unified l2 cache)
    #       size                        (total cache size)
    #       ways                        (cache associativity)
    #       block_size
    #       hit_count
    #       victim_cache_hit_count
    #       miss_count
    #       hit_rate
    #       miss_rate
    #       cost
    #     main_mem                    (main memory configuration)
    #       memory_ready_time
    #       chunksize
    #       chunktime
    #       cost
    #   references                  (reference counts)
    #     reads
    #     writes
    #     instrs
    #     read_percent
    #     write_percent
    #     instr_percent
    #   cycles                      (cycle counts)
    #     reads
    #     writes
    #     instrs
    #     read_percent
    #     write_percent
    #     instr_percent
    #     ideal
    #     ideal_misaligned
    #     cpi
    #     cpi_ideal
    #     cpi_ideal_misaligned

    # traces = ['sjeng']
    # configs = ['default', 'MemBandwidth-16', 'MemBandwidth-32', 'MemBandwidth-64']
    # plot_results(traces, configs,
    #             'CPI vs memory bandwidth',
    #             'Main memory bandwidth [bytes]',
    #             'Overall CPI [cycles/instruction]',
    #             lambda r: r.memory_system.main_mem.chunksize,
    #             lambda r: r.cycles.cpi)
    #plot_results(traces, configs,
    #             'Cost vs memory bandwidth',
    #             'Main memory bandwidth [bytes]',
    #             'Main memory cost [dollars]',
    #             lambda r: r.memory_system.main_mem.chunksize,
    #             lambda r: r.memory_system.main_mem.cost)
    #plot_results(traces, configs,
    #             'CPI vs main memory cost',
    #             'Main memory cost [cycles/instruction]',
    #             'Overall CPI [cycles/instruction]',
    #             lambda r: r.memory_system.main_mem.cost,
    #             lambda r: r.cycles.cpi)
    #traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    #configs = ['default', 'L1-small', ]
    #plot_results(traces, configs,
    #            'L1 Dcache miss rate vs L1 Dcache size',
    #             'L1 Dcache size',
    #             'Miss rate [percent]',
    #             lambda r: r.memory_system.l1d_cache.size,
    #             lambda r: r.memory_system.l1d_cache.miss_rate)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'L1-small', ]
    # plot_results(traces, configs,
    #              'L1 Dcache hit rate vs L1 Dcache size',
    #              'L1 Dcache size',
    #              'Hit rate [percent]',
    #              lambda r: r.memory_system.l1d_cache.size,
    #              lambda r: r.memory_system.l1d_cache.hit_rate)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-small', ]
    # plot_results(traces, configs,
    #              'L2 Cache miss rate vs L2 Cache size',
    #              'L2 Cache size',
    #              'Miss rate [percent]',
    #              lambda r: r.memory_system.l2_cache.size,
    #              lambda r: r.memory_system.l2_cache.miss_rate)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-small', ]
    # plot_results(traces, configs,
    #              'L2 Cache hit rate vs L2 Cache size',
    #              'L2 Cache size',
    #              'Hit rate [percent]',
    #              lambda r: r.memory_system.l2_cache.size,
    #              lambda r: r.memory_system.l2_cache.hit_rate)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'L1-small']
    # plot_results(traces, configs,
    #              'CPI vs L1 Dcache Size',
    #              'L1 Dcache size',
    #              'Overall CPI [cycles/instruction',
    #              lambda r: r.memory_system.l1d_cache.size,
    #              lambda r: r.cycles.cpi,)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-small']
    # plot_results(traces, configs,
    #              'CPI vs L2 Cache Size',
    #              'L2 Cache size',
    #              'Overall CPI [cycles/instruction',
    #              lambda r: r.memory_system.l2_cache.size,
    # #              lambda r: r.cycles.cpi,)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-FA', 'All-2way', 'All-4way']
    # plot_results(traces, configs,
    #              'L1 Dcache miss rate vs L1 Dcache associativity',
    #              'L1 Dcache associativity',
    #              'L1 Dcache miss rate',
    #              lambda r: r.memory_system.l1d_cache.ways,
    #              lambda r: r.memory_system.l1d_cache.miss_rate, log_x=True, x_limits = [0.5, 600])
    #              lambda r: r.cycles.cpi,)
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-FA', 'All-2way', 'All-4way']
    # plot_results(traces, configs,
    #              'CPI vs L1 Dcache associativity',
    #              'L1 Dcache associativity',
    #              'CPI [cycles/instruction]',
    #              lambda r: r.memory_system.l1d_cache.ways,
    #              lambda r: r.cycles.cpi, log_x=True, x_limits = [0.5, 600])
    # traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    # configs = ['default', 'All-FA', 'All-2way', 'All-4way']
    # plot_results(traces, configs,
    #              'CPI vs L2 Cache associativity',
    #              'L2 Cache associativity',
    #              'CPI [cycles/instruction]',
    #              lambda r: r.memory_system.l2_cache.ways,
    #              lambda r: r.cycles.cpi, log_x=True, x_limits = [0.5, 600])
    traces = ['astar', 'bzip2', 'gobmk', 'libquantum', 'omnetpp', 'sjeng']
    configs = ['default', 'All-FA', 'All-2way', 'All-4way']
    plot_results(traces, configs,
                 'CPI vs L2 Cache associativity',
                 'L2 Cache associativity',
                 'CPI [cycles/instruction]',
                 lambda r: r.memory_system.l2_cache.ways,
                 lambda r: r.cycles.cpi, log_x=True, x_limits = [0.5, 600])

