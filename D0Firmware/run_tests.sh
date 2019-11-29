#!/bin/bash
set -e
make testrunner -j 4
./bin/testrunner