add_library(JavaScriptCoreCPP
	JSArray.cpp
	JSClass.cpp
	JSClassDefinition.cpp
	JSContext.cpp
	JSContextGroup.cpp
	JSDate.cpp
	JSError.cpp
	JSFunction.cpp
	JSObject.cpp
	JSPropertyNameArray.cpp
	JSRegExp.cpp
	JSString.cpp
	JSValue.cpp
	detail/JSBase.cpp
	detail/JSLoggerPimpl.cpp
	detail/JSPropertyCallback.cpp
	detail/JSStaticFunction.cpp
	detail/JSStaticValue.cpp
	detail/JSUtil.cpp
  )

target_link_libraries(JavaScriptCoreCPP
  ${JavaScriptCore_LIBRARIES}
  ${Boost_LIBRARIES}
  )

