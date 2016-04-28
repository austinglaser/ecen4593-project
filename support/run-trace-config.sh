#! /bin/bash
#
# run-trace-config.sh
# Copyright (C) 2016 Austin Glaser <austin@boulderes.com>
#
# Distributed under terms of the MIT license.
#


trace=$1
config=$2

sim=./build/release/project.out

trace_basename=$(basename ${trace} .gz)
config_basename=$(basename ${config})

resultsdir=results
timesdir=times
resultsname=${trace_basename}.${config_basename}
resultsfile=${resultsdir}/${resultsname}
timefile=${timesdir}/${resultsname}.time

echo "Running '${trace_basename}' with config '${config_basename}'"

$(zcat ${trace} | time -o ${timefile} ${sim} ${config} -t ${trace_basename} > ${resultsfile})
