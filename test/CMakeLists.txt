# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

include_directories(${PROJECT_SOURCE_DIR}/examples)

#add_executable(foo foo.cc)
#target_link_libraries(foo ${GTEST_BOTH_LIBRARIES})

cxx_test(JSContextGroupTests . JavaScriptCoreCPP)
cxx_test(JSContextTests      . JavaScriptCoreCPP)
cxx_test(JSStringTests       . JavaScriptCoreCPP)
cxx_test(JSValueTests        . JavaScriptCoreCPP)
cxx_test(JSObjectTests       . JavaScriptCoreCPP)
cxx_test(JSExportTests       . JavaScriptCoreCPP_examples)
