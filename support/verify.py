#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2016 Austin Glaser <austin@boulderes.com>
#
# Distributed under terms of the MIT license.

"""
Verifies all results and configurations
"""

import os
import subprocess
import re


if __name__ == "__main__":
    config_dir = 'config'
    traces_dir = 'traces'
    results_dir = 'validation-results'
    traces_short_dir = os.path.join(traces_dir, 'traces-short')
    traces_5M_dir = os.path.join(traces_dir, 'traces-5M')
    sample_output_dir = os.path.join(traces_dir, 'sample-output')
    program = './build-make/simulator'

    configs = [os.path.join(config_dir, c) for c in os.listdir(config_dir) if not re.search(r".*MemBandwidth.*", c)]
    traces = [os.path.join(traces_short_dir, t) for t in os.listdir(traces_short_dir)]
    ztraces = [os.path.join(traces_5M_dir, 'sjeng.gz')]
    traces.extend(ztraces)

    for trace in traces:
        for config in configs:
            if trace in ztraces:
                cat = 'zcat'
                tracesep = '-5M.'
            else:
                cat = 'cat'
                tracesep = '.'
            trace_basename = os.path.splitext(os.path.basename(trace))[0]
            config_basename = os.path.basename(config)

            result_filename = trace_basename + tracesep + config_basename
            result = os.path.join(results_dir, result_filename)
            good_result = os.path.join(sample_output_dir, result_filename)

            command_line = ' '.join([cat, trace, '|', program, config, '-t', trace_basename, '>', result])

            os.system(command_line)

            diff = ['diff', '-w', good_result, result]
            try:
                subprocess.check_output(diff)
            except:
                print 'invalid output for', os.path.basename(result)

