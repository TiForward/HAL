# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

set(SOURCE_Widget
  Widget.hpp
  Widget.cpp
)

add_library(JavaScriptCoreCPP_examples STATIC
  ${SOURCE_Widget}
  )
target_include_directories(JavaScriptCoreCPP_examples INTERFACE
  ${PROJECT_SOURCE_DIR}/examples
  )
target_link_libraries(JavaScriptCoreCPP_examples JavaScriptCoreCPP)

set(SOURCE_WidgetMain
  WidgetMain.cpp
  )
add_executable(WidgetMain
  ${SOURCE_WidgetMain}
)
target_link_libraries(WidgetMain JavaScriptCoreCPP_examples)

set(SOURCE_EvaluateScript
  EvaluateScript.cpp
  )
add_executable(EvaluateScript
  ${SOURCE_EvaluateScript}
  )
target_link_libraries(EvaluateScript JavaScriptCoreCPP)

source_group(JavaScriptCoreCPP\\Examples FILES
  ${SOURCE_Widget}
  ${SOURCE_WidgetMain}
  ${SOURCE_EvaluateScript}
  )
