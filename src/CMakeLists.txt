add_library(JavaScriptCoreCPP
  JSContext.cpp
  JSProxy.cpp
  JSString.cpp
  JSValue.cpp
  JSVirtualMachine.cpp

	RAII/JSValue.cpp
  )

target_link_libraries(JavaScriptCoreCPP
  ${JavaScriptCore_LIBRARIES}
  ${Boost_LIBRARIES}
  )
