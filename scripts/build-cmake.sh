#!/bin/bash

set -eu

BUILDDIR=$(pwd)/build-cmake
make -S . -B ${BUILDDIR} -G Ninja
cmake --build ${BUILDDIR}

LD_PRELOAD=${BUILDDIR}/src/libthrowinfo.so ${BUILDDIR}/src/testprog

exit 0
