# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

cmake_minimum_required(VERSION 3.0.0)
project(JavaScriptCoreCPP)

get_filename_component(APPCELERATOR_CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake ABSOLUTE)
list(INSERT CMAKE_MODULE_PATH 0 ${APPCELERATOR_CMAKE_MODULE_PATH})

# Define helper functions and macros.
include(${APPCELERATOR_CMAKE_MODULE_PATH}/internal_utils.cmake)

config_compiler_and_linker()  # Defined in internal_utils.cmake.
#config_cmake_system_framework_path(macosx)

# Allow "make test" to work.
enable_testing()

find_package(JavaScriptCore REQUIRED)
include_directories(${JavaScriptCore_INCLUDE_DIRS})
add_definitions(-DSTATICALLY_LINKED_WITH_JavaScriptCore)

set( Boost_USE_STATIC_LIBS    ON  )
set( Boost_USE_MULTITHREADED  ON  )
set( Boost_USE_STATIC_RUNTIME OFF )

# Travis only supports Boost 1.55, which is good enough for now.
find_package( Boost 1.55.0 REQUIRED )

include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  )

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)
