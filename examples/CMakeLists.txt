# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

add_executable(EvaluateScript EvaluateScript.cpp)
target_link_libraries(EvaluateScript JavaScriptCoreCPP)

add_library(JavaScriptCoreCPP_examples
  Widget.hpp
  Widget.cpp
  )
target_link_libraries(JavaScriptCoreCPP_examples JavaScriptCoreCPP)

add_executable(WidgetMain WidgetMain.cpp)
target_link_libraries(WidgetMain JavaScriptCoreCPP_examples)
