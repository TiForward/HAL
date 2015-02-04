include(${CMAKE_CURRENT_LIST_DIR}/internal_utils.cmake)

# Build gtest from source!
set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll")
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/gtest ${CMAKE_CURRENT_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

# cxx_test_with_flags(name cxx_flags libs srcs...)
#
# Creates a named C++ test that depends on the given libs and is built
# from the given source files with the given compiler flags.
function(cxx_test_with_flags name cxx_flags libs)
  list(APPEND libs gtest_main)
  cxx_executable_with_flags(${name} "${cxx_flags}" "${libs}" ${ARGN})
  set_property(TARGET ${name}
    APPEND PROPERTY INCLUDE_DIRECTORIES ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  add_test(${name} ${name})
endfunction()

# cxx_test(name libs srcs...)
#
# Creates a named test target that depends on the given libs and is
# built from the given source files.  Unlike cxx_test_with_flags,
# name.cpp is already implicitly included in the source file list.
function(cxx_test name dir libs)
  cxx_test_with_flags("${name}" "${cxx_default}" "${libs}"
    "${dir}/${name}.cpp" ${ARGN})
endfunction()

# Sets PYTHONINTERP_FOUND and PYTHON_EXECUTABLE.
find_package(PythonInterp)

# py_test(name)
#
# Creates a Python test with the given name whose main module is in
# test/name.py.  It does nothing if Python is not installed.
function(py_test name)
  # We are not supporting Python tests on Linux yet as they consider
  # all Linux environments to be google3 and try to use google3
  # features.
  if (PYTHONINTERP_FOUND)
    # ${CMAKE_BINARY_DIR} is known at configuration time, so we can
    # directly bind it from cmake. ${CTEST_CONFIGURATION_TYPE} is
    # known only at ctest runtime (by calling ctest -c
    # <Configuration>), so we have to escape $ to delay variable
    # substitution here.
    add_test(${name}
      ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/test/${name}.py
      --build_dir=${CMAKE_CURRENT_BINARY_DIR}/\${CTEST_CONFIGURATION_TYPE})
  endif()
endfunction()

# Print all defined properties of the given target (i.e. those without
# a value of -NOTFOUND).
function(print_target_properties TARGET_NAME)
  execute_process(COMMAND cmake --help-property-list
    OUTPUT_VARIABLE CMAKE_PROPERTY_LIST_TEXT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  
  string(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST_TEXT}")
  #message(STATUS "*** ${CMAKE_PROPERTY_LIST} ***")
  
  foreach(PROPERTY ${CMAKE_PROPERTY_LIST})
    # We don't want properties like these:
    #
    # make version 2.8.9
    # COMPILE_DEFINITIONS_<CONFIG>
    # <CONFIG>_OUTPUT_NAME
    # VS_GLOBAL_<variable>
    # XCODE_ATTRIBUTE_<an-attribute>
    
    # This is how to do it using grep:
    # cmake --help-property-list | grep -v -E "(<.+>| )"
    if (NOT "${PROPERTY}" MATCHES "(<.+>| |LOCATION)")
      #message(STATUS "*** ${PROPERTY} ***")
      get_target_property(PROPERTY_VALUE ${TARGET_NAME} ${PROPERTY})
      
      if (NOT "${PROPERTY_VALUE}" MATCHES "-NOTFOUND")
		message(STATUS "*** ${PROPERTY} = ${PROPERTY_VALUE} ***")
      endif()
      
    endif()
  endforeach()
endfunction()