add_library(JavaScriptCoreCPP
	JSArray.cpp
	JSClass.cpp
	JSContext.cpp
	JSContextGroup.cpp
	JSDate.cpp
	JSError.cpp
	JSFunction.cpp
	JSObject.cpp
	JSRegExp.cpp
	JSValue.cpp
	#detail/HashUtilities.cpp
	detail/JSUtil.cpp
  )

target_link_libraries(JavaScriptCoreCPP
  ${JavaScriptCore_LIBRARIES}
  ${Boost_LIBRARIES}
  )

