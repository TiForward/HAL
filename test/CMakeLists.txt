include_directories(${GTEST_INCLUDE_DIRS})

#add_executable(foo foo.cc)
#target_link_libraries(foo ${GTEST_BOTH_LIBRARIES})

cxx_test(JSStringTests . JavaScriptCoreCPP ${JavaScriptCore_LIBRARIES})
