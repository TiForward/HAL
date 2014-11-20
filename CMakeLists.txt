# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

cmake_minimum_required(VERSION 3.0.0)
project(JavaScriptCoreCPP)

option(JavaScriptCoreCPP_DISABLE_TESTS "Disable compiling the tests" OFF)

get_filename_component(APPCELERATOR_CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../cmake ABSOLUTE)
list(INSERT CMAKE_MODULE_PATH 0 ${APPCELERATOR_CMAKE_MODULE_PATH})

# Define helper functions and macros.
include(${APPCELERATOR_CMAKE_MODULE_PATH}/internal_utils.cmake)

# Defined in internal_utils.cmake.
config_compiler_and_linker()
#config_cmake_system_framework_path(macosx)

# Allow "make test" to work.
enable_testing()

find_package(JavaScriptCore REQUIRED)

set( Boost_USE_STATIC_LIBS    ON  )
set( Boost_USE_MULTITHREADED  ON  )
set( Boost_USE_STATIC_RUNTIME OFF )

# Travis only supports Boost 1.55, which is good enough for now.
find_package( Boost 1.55.0 REQUIRED )

set(SOURCE_JavaScriptCoreCPP
  include/JavaScriptCoreCPP/JavaScriptCoreCPP.hpp
  include/JavaScriptCoreCPP/JSString.hpp
  src/JSString.cpp
  )

set(SOURCE_JavaScriptCoreCPP_detail
  include/JavaScriptCoreCPP/detail/JSBase.hpp
  src/detail/JSBase.cpp
  include/JavaScriptCoreCPP/detail/JSUtil.hpp
  src/detail/JSUtil.cpp
  include/JavaScriptCoreCPP/detail/HashUtilities.hpp
  include/JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp
  include/JavaScriptCoreCPP/detail/JSPerformanceCounterPrinter.hpp
  )

set(SOURCE_JSExport
  include/JavaScriptCoreCPP/JSExport.hpp
  include/JavaScriptCoreCPP/JSExportObject.hpp
  src/JSExportObject.cpp
  )

set(SOURCE_JSExport_detail
  include/JavaScriptCoreCPP/detail/JSExportClassDefinition.hpp
  include/JavaScriptCoreCPP/detail/JSExportClassDefinitionBuilder.hpp
  include/JavaScriptCoreCPP/detail/JSExportClass.hpp
  include/JavaScriptCoreCPP/detail/JSExportCallbacks.hpp
  include/JavaScriptCoreCPP/detail/JSExportNamedFunctionPropertyCallback.hpp
  include/JavaScriptCoreCPP/detail/JSExportNamedValuePropertyCallback.hpp
  include/JavaScriptCoreCPP/detail/JSValueUtil.hpp
  src/detail/JSValueUtil.cpp
  )
  
set(SOURCE_JSClass
  include/JavaScriptCoreCPP/JSClassDefinition.hpp
  src/JSClassDefinition.cpp
  include/JavaScriptCoreCPP/JSClassAttribute.hpp
  include/JavaScriptCoreCPP/JSClass.hpp
  src/JSClass.cpp
  )
  
set(SOURCE_JSClass_detail
  include/JavaScriptCoreCPP/detail/JSPropertyCallback.hpp
  src/detail/JSPropertyCallback.cpp
  include/JavaScriptCoreCPP/detail/JSStaticValue.hpp
  src/detail/JSStaticValue.cpp
  include/JavaScriptCoreCPP/detail/JSStaticFunction.hpp
  src/detail/JSStaticFunction.cpp
  )

set(SOURCE_JSContext
  include/JavaScriptCoreCPP/JSContextGroup.hpp
  src/JSContextGroup.cpp
  include/JavaScriptCoreCPP/JSContext.hpp
  src/JSContext.cpp
  )

set(SOURCE_JSValue
  include/JavaScriptCoreCPP/JSValue.hpp
  src/JSValue.cpp
  include/JavaScriptCoreCPP/JSUndefined.hpp
  include/JavaScriptCoreCPP/JSNull.hpp
  include/JavaScriptCoreCPP/JSBoolean.hpp
  include/JavaScriptCoreCPP/JSNumber.hpp
  )

set(SOURCE_JSObject
  include/JavaScriptCoreCPP/JSPropertyAttribute.hpp
  include/JavaScriptCoreCPP/JSPropertyNameArray.hpp
  src/JSPropertyNameArray.cpp
  include/JavaScriptCoreCPP/JSObject.hpp
  src/JSObject.cpp
  include/JavaScriptCoreCPP/JSArray.hpp
  src/JSArray.cpp
  include/JavaScriptCoreCPP/JSDate.hpp
  src/JSDate.cpp
  include/JavaScriptCoreCPP/JSError.hpp
  src/JSError.cpp 
  include/JavaScriptCoreCPP/JSRegExp.hpp
  src/JSRegExp.cpp
  include/JavaScriptCoreCPP/JSFunction.hpp
  src/JSFunction.cpp
  )
  
set(SOURCE_JSObject_detail
  include/JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp
  )

set(SOURCE_JSLogger_detail
  include/JavaScriptCoreCPP/detail/JSLogger.hpp
  include/JavaScriptCoreCPP/detail/JSLoggerPolicyInterface.hpp
  include/JavaScriptCoreCPP/detail/JSLoggerPolicyConsole.hpp
  include/JavaScriptCoreCPP/detail/JSLoggerPolicyFile.hpp
  include/JavaScriptCoreCPP/detail/JSLoggerPimpl.hpp
  src/detail/JSLoggerPimpl.cpp
  )

source_group(JavaScriptCoreCPP                   FILES ${SOURCE_JavaScriptCoreCPP})
source_group(JavaScriptCoreCPP\\detail           FILES ${SOURCE_JavaScriptCoreCPP_detail})
source_group(JavaScriptCoreCPP\\JSExport         FILES ${SOURCE_JSExport})
source_group(JavaScriptCoreCPP\\JSExport\\detail FILES ${SOURCE_JSExport_detail})
source_group(JavaScriptCoreCPP\\JSClass          FILES ${SOURCE_JSClass})
source_group(JavaScriptCoreCPP\\JSClass\\detail  FILES ${SOURCE_JSClass_detail})
source_group(JavaScriptCoreCPP\\JSContext        FILES ${SOURCE_JSContext})
source_group(JavaScriptCoreCPP\\JSValue          FILES ${SOURCE_JSValue})
source_group(JavaScriptCoreCPP\\JSObject         FILES ${SOURCE_JSObject})
source_group(JavaScriptCoreCPP\\JSObject\\detail FILES ${SOURCE_JSObject_detail})
source_group(JavaScriptCoreCPP\\JSLogger\\detail FILES ${SOURCE_JSLogger_detail})

add_library(JavaScriptCoreCPP
  ${SOURCE_JavaScriptCoreCPP}
  ${SOURCE_JavaScriptCoreCPP_detail}
  ${SOURCE_JSExport}
  ${SOURCE_JSExport_detail}
  ${SOURCE_JSClass}
  ${SOURCE_JSClass_detail}
  ${SOURCE_JSContext}
  ${SOURCE_JSValue}
  ${SOURCE_JSObject}
  ${SOURCE_JSObject_detail}
  ${SOURCE_JSLogger_detail}
  )

target_compile_definitions(JavaScriptCoreCPP PUBLIC STATICALLY_LINKED_WITH_JavaScriptCore)
target_include_directories(JavaScriptCoreCPP PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${JavaScriptCore_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  )

target_link_libraries(JavaScriptCoreCPP
  ${JavaScriptCore_LIBRARIES}
  ${Boost_LIBRARIES}
  )

if (NOT JavaScriptCoreCPP_DISABLE_TESTS)
  add_subdirectory(examples)
  add_subdirectory(test)
endif()
