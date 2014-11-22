# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.
cmake_minimum_required(VERSION 3.0.0)

project(JavaScriptCoreCPP)

set(JavaScriptCoreCPP_VERSION 0.5.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

option(JavaScriptCoreCPP_DISABLE_TESTS "Disable compiling the tests" OFF)

# Define helper functions and macros.
include(${PROJECT_SOURCE_DIR}/cmake/internal_utils.cmake)

# Defined in internal_utils.cmake.
config_compiler_and_linker()
#config_cmake_system_framework_path(macosx)

# Allow "make test" to work.
enable_testing()

# We have a custom finder for JavaScriptCoreCPP.
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
find_package(JavaScriptCore REQUIRED)

set(Boost_USE_STATIC_LIBS    ON )
set(Boost_USE_MULTITHREADED  ON )
set(Boost_USE_STATIC_RUNTIME OFF)

# Travis only supports Boost 1.55, which is good enough for now.
find_package(Boost 1.55.0 REQUIRED)

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

#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(JavaScriptCoreCPP SHARED
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

include(GenerateExportHeader)
generate_export_header(JavaScriptCoreCPP)
target_compile_definitions(JavaScriptCoreCPP PRIVATE JavaScriptCoreCPP_EXPORTS)
#target_compile_definitions(JavaScriptCoreCPP PUBLIC JAVASCRIPTCORECPP_STATIC_DEFINE)

target_compile_definitions(JavaScriptCoreCPP PRIVATE STATICALLY_LINKED_WITH_JavaScriptCore)
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


# Support find_package(JavaScriptCoreCPP 0.5 REQUIRED)

if (WIN32)
	# Silence this warning when lnking with the JavaScriptCore static
	# library:
	#
	# "warning LNK4099: PDB 'vc120.pdb' was not found with
	# 'JavaScriptCore-Debug.lib(...)' ...".
	set_property(TARGET JavaScriptCoreCPP PROPERTY LINK_FLAGS "/IGNORE:4099")
	
	# Silence this warning when lnking with the JavaScriptCore static
	# library for the i386 emulator:
	#
	# "JavaScriptCore-Debug.lib(JSStringRef.obj) : warning LNK4075:
	# ignoring '/EDITANDCONTINUE' due to '/SAFESEH' specification".
	#
	# According to "/IGNORE (Ignore Specific Warnings)" (see
	# http://msdn.microsoft.com/en-us/library/dn782850.aspx) some linker
	# warnings cannot be ignored and LNK4075 is one of them. However,
	# according to "/SAFESEH (Image has Safe Exception Handlers)" (see
	# http://msdn.microsoft.com/en-us/library/9a89h429.aspx):
	#
	# "/SAFESEH is only valid when linking for x86 targets."
	#
	# Therefore we test for the processor architecture we are targeting
	# and if its i386 (i.e. the emulator) then we pass /SAFESEH:NO to
	# the linker.
	include(${PROJECT_SOURCE_DIR}/cmake/TargetArch.cmake)
	target_architecture(target_architecture)
	if (${target_architecture} STREQUAL i386)
		set_property(TARGET JavaScriptCoreCPP APPEND_STRING PROPERTY LINK_FLAGS " /SAFESEH:NO")
	endif()
endif()

set_property(TARGET JavaScriptCoreCPP PROPERTY VERSION ${JavaScriptCoreCPP_VERSION})
set_property(TARGET JavaScriptCoreCPP PROPERTY SOVERSION 0)
set_property(TARGET JavaScriptCoreCPP PROPERTY INTERFACE_JavaScriptCoreCPP_MAJOR_VERSION 0)
set_property(TARGET JavaScriptCoreCPP APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING JavaScriptCoreCPP_MAJOR_VERSION
  )

install(TARGETS JavaScriptCoreCPP EXPORT JavaScriptCoreCPPTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )

export(EXPORT JavaScriptCoreCPPTargets
  FILE "${CMAKE_BINARY_DIR}/JavaScriptCoreCPPTargets.cmake"
)

configure_file(cmake/JavaScriptCoreCPPConfig.cmake
  "${CMAKE_BINARY_DIR}/JavaScriptCoreCPPConfig.cmake"
  COPY_ONLY
  )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/JavaScriptCoreCPPConfigVersion.cmake"
  VERSION ${JavaScriptCoreCPP_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

# Store the current build directory in the CMake user package registry
# so that find_package(JavaScriptCoreCPP) works.
export(PACKAGE JavaScriptCoreCPP)

#[[
# TODO: Install files to external locations.

set(ConfigPackageLocation lib/cmake/JavaScriptCoreCPP)
install(EXPORT JavaScriptCoreCPPTargets
  DESTINATION ${ConfigPackageLocation}
  )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/JavaScriptCoreCPPConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/JavaScriptCoreCPPConfigVersion.cmake"
  DESTINATION ${ConfigPackageLocation}
  COMPONENT Devel
  )
]]