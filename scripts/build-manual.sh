#!/bin/bash

CXX=g++
BUILDDIR=$(pwd)/build

mkdir -p ${BUILDDIR}

${CXX} -I include/ -W -Wall -std=c++23 -O0 -g -o ${BUILDDIR}/testprog src/tests/testprog.cpp 

${CXX} -I include/ -fPIC -W -Wall -std=c++23 -O3 -g -c -o ${BUILDDIR}/throwinfo.o src/throwinfo/throwinfo.cpp -lstdc++exp
${CXX} --shared -o ${BUILDDIR}/throwinfo.so ${BUILDDIR}/throwinfo.o -lstdc++exp  -ldl

LD_PRELOAD=${BUILDDIR}/throwinfo.so ${BUILDDIR}/testprog

exit 0
