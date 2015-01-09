# HAL
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

set(SOURCE_Widget
  Widget.hpp
  Widget.cpp
  WidgetExample.hpp
  WidgetExample.cpp
)

add_library(HAL_examples STATIC
  ${SOURCE_Widget}
  )
target_include_directories(HAL_examples INTERFACE
  ${PROJECT_SOURCE_DIR}/examples
  )
target_link_libraries(HAL_examples HAL)

set(SOURCE_WidgetMain
  WidgetMain.cpp
  )
add_executable(WidgetMain
  ${SOURCE_WidgetMain}
)
target_link_libraries(WidgetMain HAL_examples)

set(SOURCE_EvaluateScript
  EvaluateScript.cpp
  )
add_executable(EvaluateScript
  ${SOURCE_EvaluateScript}
  )
target_link_libraries(EvaluateScript HAL)

source_group(HAL\\Examples FILES
  ${SOURCE_Widget}
  ${SOURCE_WidgetMain}
  ${SOURCE_EvaluateScript}
  )
