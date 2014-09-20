#!/usr/bin/env bash

# Author: Matt Langston
# Date: 2014.09.15
#

if ! test -d "${GTEST_ROOT}"; then
    echo "GTEST_ROOT must point to your Google Test installation."
    exit 1
fi

declare -rx VERBOSE=1

declare -r CMAKE_BUILD_TYPE=Debug
declare -r BUILD_DIR=build.$(echo ${CMAKE_BUILD_TYPE} | tr '[:upper:]' '[:lower:]')

function echo_and_eval {
    local -r cmd="${1:?}"
    echo "${cmd}" && eval "${cmd}"
}

cmd+="cmake"
cmd+=" -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} ../"
cmd+=" ../"

echo_and_eval "rm -rf \"${BUILD_DIR}\""
echo_and_eval "mkdir \"${BUILD_DIR}\""
echo_and_eval "pushd \"${BUILD_DIR}\""
echo_and_eval "${cmd}"
echo_and_eval "make -j 4"
echo_and_eval "ctest -VV"
echo_and_eval "popd"
