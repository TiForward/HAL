# HAL
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.
cmake_minimum_required(VERSION 3.0.0)

project(HAL)

set(HAL_VERSION 0.5.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

option(HAL_DISABLE_TESTS "Disable compiling the tests" OFF)

# Define helper functions and macros.
include(${PROJECT_SOURCE_DIR}/cmake/internal_utils.cmake)

# Defined in internal_utils.cmake.
config_compiler_and_linker()
#config_cmake_system_framework_path(macosx)

# Allow "make test" to work.
enable_testing()

# We have a custom finder for HAL.
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
find_package(JavaScriptCore REQUIRED)

set(SOURCE_HAL
  include/HAL/HAL.hpp
  include/HAL/JSString.hpp
  src/JSString.cpp
  )

set(SOURCE_HAL_detail
  include/HAL/detail/JSBase.hpp
  src/detail/JSBase.cpp
  include/HAL/detail/JSUtil.hpp
  src/detail/JSUtil.cpp
  include/HAL/detail/HashUtilities.hpp
  include/HAL/detail/JSPerformanceCounter.hpp
  include/HAL/detail/JSPerformanceCounterPrinter.hpp
  )

set(SOURCE_JSExport
  include/HAL/JSExport.hpp
  include/HAL/JSExportObject.hpp
  src/JSExportObject.cpp
  )

set(SOURCE_JSExport_detail
  include/HAL/detail/JSExportClassDefinition.hpp
  include/HAL/detail/JSExportClassDefinitionBuilder.hpp
  include/HAL/detail/JSExportClass.hpp
  include/HAL/detail/JSExportCallbacks.hpp
  include/HAL/detail/JSExportNamedFunctionPropertyCallback.hpp
  include/HAL/detail/JSExportNamedValuePropertyCallback.hpp
  include/HAL/detail/JSValueUtil.hpp
  src/detail/JSValueUtil.cpp
  )
  
set(SOURCE_JSClass
  include/HAL/JSClassDefinition.hpp
  src/JSClassDefinition.cpp
  include/HAL/JSClassAttribute.hpp
  include/HAL/JSClass.hpp
  src/JSClass.cpp
  )
  
set(SOURCE_JSClass_detail
  include/HAL/detail/JSPropertyCallback.hpp
  src/detail/JSPropertyCallback.cpp
  include/HAL/detail/JSStaticValue.hpp
  src/detail/JSStaticValue.cpp
  include/HAL/detail/JSStaticFunction.hpp
  src/detail/JSStaticFunction.cpp
  )

set(SOURCE_JSContext
  include/HAL/JSContextGroup.hpp
  src/JSContextGroup.cpp
  include/HAL/JSContext.hpp
  src/JSContext.cpp
  )

set(SOURCE_JSValue
  include/HAL/JSValue.hpp
  src/JSValue.cpp
  include/HAL/JSUndefined.hpp
  include/HAL/JSNull.hpp
  include/HAL/JSBoolean.hpp
  include/HAL/JSNumber.hpp
  )

set(SOURCE_JSObject
  include/HAL/JSPropertyAttribute.hpp
  include/HAL/JSPropertyNameArray.hpp
  src/JSPropertyNameArray.cpp
  include/HAL/JSObject.hpp
  src/JSObject.cpp
  include/HAL/JSArray.hpp
  src/JSArray.cpp
  include/HAL/JSDate.hpp
  src/JSDate.cpp
  include/HAL/JSError.hpp
  src/JSError.cpp 
  include/HAL/JSRegExp.hpp
  src/JSRegExp.cpp
  include/HAL/JSFunction.hpp
  src/JSFunction.cpp
  )
  
set(SOURCE_JSObject_detail
  include/HAL/detail/JSPropertyNameAccumulator.hpp
  )

set(SOURCE_JSLogger_detail
  include/HAL/detail/JSLogger.hpp
  include/HAL/detail/JSLoggerPolicyInterface.hpp
  include/HAL/detail/JSLoggerPolicyConsole.hpp
  include/HAL/detail/JSLoggerPolicyFile.hpp
  include/HAL/detail/JSLoggerPimpl.hpp
  src/detail/JSLoggerPimpl.cpp
  )

source_group(HAL                   FILES ${SOURCE_HAL})
source_group(HAL\\detail           FILES ${SOURCE_HAL_detail})
source_group(HAL\\JSExport         FILES ${SOURCE_JSExport})
source_group(HAL\\JSExport\\detail FILES ${SOURCE_JSExport_detail})
source_group(HAL\\JSClass          FILES ${SOURCE_JSClass})
source_group(HAL\\JSClass\\detail  FILES ${SOURCE_JSClass_detail})
source_group(HAL\\JSContext        FILES ${SOURCE_JSContext})
source_group(HAL\\JSValue          FILES ${SOURCE_JSValue})
source_group(HAL\\JSObject         FILES ${SOURCE_JSObject})
source_group(HAL\\JSObject\\detail FILES ${SOURCE_JSObject_detail})
source_group(HAL\\JSLogger\\detail FILES ${SOURCE_JSLogger_detail})

#set(CMAKE_CXX_VISIBILITY_PRESET hidden)
#set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

add_library(HAL SHARED
  ${SOURCE_HAL}
  ${SOURCE_HAL_detail}
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
generate_export_header(HAL)
target_compile_definitions(HAL PRIVATE HAL_EXPORTS)
#target_compile_definitions(HAL PUBLIC HAL_STATIC_DEFINE)

target_compile_definitions(HAL PRIVATE STATICALLY_LINKED_WITH_JavaScriptCore)
target_include_directories(HAL PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${JavaScriptCore_INCLUDE_DIRS}
  )

target_link_libraries(HAL
  ${JavaScriptCore_LIBRARIES}
  )

if (WIN32)
  # Silence this warning when lnking with the JavaScriptCore static
  # library:
  #
  # "warning LNK4099: PDB 'vc120.pdb' was not found with
  # 'JavaScriptCore-Debug.lib(...)' ...".
  set_property(TARGET HAL APPEND PROPERTY LINK_FLAGS "/IGNORE:4099")
  
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
    set_property(TARGET HAL APPEND_STRING PROPERTY LINK_FLAGS " /SAFESEH:NO")
  endif()

  # Silence this warning when lnking the Debug configuration:
  #
  # "LINK : warning LNK4075: ignoring '/INCREMENTAL' due to '/OPT:ICF'
  # specification ..."
  #
  # According to "/IGNORE (Ignore Specific Warnings)" (see
  # http://msdn.microsoft.com/en-us/library/dn782850.aspx) some linker
  # warnings cannot be ignored and LNK4075 is one of them. However,
  # according to "/INCREMENTAL (Link Incrementally)" (see
  # http://msdn.microsoft.com/en-us/library/4khtbfyf.aspx):
  #
  # "Most programs can be linked incrementally. However, some changes
  # are too great, and some options are incompatible with incremental
  # linking. LINK performs a full link if any of the following options
  # are specified:
  # ...
  # /OPT:REF is selected
  # /OPT:ICF is selected
  # ..."
  #
  # And according to "/OPT (Optimizations)" (see
  # http://msdn.microsoft.com/en-us/library/bxwfs976.aspx):
  #
  # "If /DEBUG is specified, the default for /OPT is NOREF, and all
  # functions are preserved in the image."
  #
  # As well as:
  #
  # "... we do not recommend that you use /OPT:ICF in debug builds ..."
  #
  # Therefore we disable /OPT:REF and /OPT:ICF for the Debug
  # configuration.
  set_property(TARGET HAL APPEND_STRING PROPERTY LINK_FLAGS_DEBUG "/OPT:NOREF /OPT:NOICF")
endif()

if (NOT HAL_DISABLE_TESTS)
  add_subdirectory(examples)
  add_subdirectory(test)
endif()

# Support find_package(HAL 0.5 REQUIRED)

set_property(TARGET HAL PROPERTY VERSION ${HAL_VERSION})
set_property(TARGET HAL PROPERTY SOVERSION 0)
set_property(TARGET HAL PROPERTY INTERFACE_HAL_MAJOR_VERSION 0)
set_property(TARGET HAL APPEND PROPERTY
  COMPATIBLE_INTERFACE_STRING HAL_MAJOR_VERSION
  )

install(TARGETS HAL EXPORT HALTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
  )

export(EXPORT HALTargets
  FILE "${CMAKE_BINARY_DIR}/HALTargets.cmake"
)

configure_file(cmake/HALConfig.cmake
  "${CMAKE_BINARY_DIR}/HALConfig.cmake"
  COPY_ONLY
  )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_BINARY_DIR}/HALConfigVersion.cmake"
  VERSION ${HAL_VERSION}
  COMPATIBILITY AnyNewerVersion
  )

# Store the current build directory in the CMake user package registry
# so that find_package(HAL) works.
export(PACKAGE HAL)

#[[
# TODO: Install files to external locations.

set(ConfigPackageLocation lib/cmake/HAL)
install(EXPORT HALTargets
  DESTINATION ${ConfigPackageLocation}
  )

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/HALConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/HALConfigVersion.cmake"
  DESTINATION ${ConfigPackageLocation}
  COMPONENT Devel
  )
]]
