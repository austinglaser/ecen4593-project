# ECEN4593 Project Sp 2016
-------------------------

## Directories

Directory             | Use
--------------------- | ---------------
`inc/`                | All header files
`src/`                | All source file
`test/`               | Test suites
`support/`            | Scripts
`vendor/`             | Test framework
`traces/`             | All traces
`results/`            | Results from production traces
`times/`              | CPU time for each simulation run
`config/`             | The various memory configuration files
`validation-results/` | Results produced by our simulator to validate against known results

## Compiling

Our simulator has a makefile to compile a release binary; it can be built using
`make`.

To build a release binary and run the test suite, run `rake`. This requires the
installation of ruby.
