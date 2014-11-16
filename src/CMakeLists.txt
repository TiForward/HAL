# JavaScriptCoreCPP
# Author: Matthew D. Langston
#
# Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
# Licensed under the terms of the Apache Public License.
# Please see the LICENSE included with this distribution for details.

add_library(JavaScriptCoreCPP
  JSArray.cpp
  JSClass.cpp
  JSClassDefinition.cpp
  JSContext.cpp
  JSContextGroup.cpp
  JSDate.cpp
  JSError.cpp
  JSExportObject.cpp
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
  detail/JSValueUtil.cpp
  )

target_link_libraries(JavaScriptCoreCPP
  ${JavaScriptCore_LIBRARIES}
  ${Boost_LIBRARIES}
  )

