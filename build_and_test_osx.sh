#!/usr/bin/env bash

# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

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
echo_and_eval "mkdir -p \"${BUILD_DIR}\""
echo_and_eval "pushd \"${BUILD_DIR}\""
echo_and_eval "${cmd}"
echo_and_eval "make -j 4"
echo_and_eval "ctest -VV --output-on-failure"
echo_and_eval "popd"
