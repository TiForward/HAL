/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTCALLBACKHANDLER_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTCALLBACKHANDLER_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"

#include <vector>

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class

  @discussion A JSNativeObjectCallbackHandler is an abstract base
  class (i.e. an interface) for C++ classes that can handle
  JSNativeObject callbacks.
*/
class JSNativeObjectCallbackHandler {
  
 public:
  
  JSNativeObjectCallbackHandler()                                                = default;
  virtual ~JSNativeObjectCallbackHandler()                                       = default;
  JSNativeObjectCallbackHandler(const JSNativeObjectCallbackHandler&)            = default;
  JSNativeObjectCallbackHandler(JSNativeObjectCallbackHandler&&)                 = default;
  JSNativeObjectCallbackHandler& operator=(const JSNativeObjectCallbackHandler&) = default;
  JSNativeObjectCallbackHandler& operator=(JSNativeObjectCallbackHandler&&)      = default;


	// All JSNativeClass<T> callback handlers must implement the
	// following interface.
	
  virtual JSValue  GetNativeProperty(const JSObject& object, const JSString& property_name)                             const = 0;
  virtual bool     SetNativeProperty(JSObject object, const JSString& property_name, const JSValue& value)                    = 0;
  virtual JSValue  CallAsNativeFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments)       = 0;

	virtual void     Initialize(JSObject object)                                                                                = 0;
  virtual void     Finalize(JSObject object)                                                                                  = 0;
  virtual JSValue  CallAsFunction(JSObject function, JSObject this_object, const std::vector<JSValue>& arguments)             = 0;
  virtual JSObject CallAsConstructor(JSObject constructor, const std::vector<JSValue>& arguments)                             = 0;
  virtual bool     HasInstance(const JSObject& constructor, const JSValue& possible_instance)                           const = 0;
  virtual JSValue  ConvertToType(const JSObject& object, const JSValue::JSType& type)                                   const = 0;
};

} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTCALLBACKHANDLER_HPP_
