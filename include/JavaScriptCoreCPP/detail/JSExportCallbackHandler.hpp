/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKHANDLER_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKHANDLER_HPP_

#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"

#include <vector>

// namespace JavaScriptCoreCPP {
// class JSString;
// class JSValue;
// class JSObject;
// }

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class

  @discussion A JSExportCallbackHandler is an abstract base class
  (i.e. an interface) for C++ classes that can handle JSExport
  callbacks.
*/
class JSExportCallbackHandler {
  
 public:
  
  JSExportCallbackHandler()                                          = default;
  virtual ~JSExportCallbackHandler()                                 = default;
  JSExportCallbackHandler(const JSExportCallbackHandler&)            = default;
  JSExportCallbackHandler(JSExportCallbackHandler&&)                 = default;
  JSExportCallbackHandler& operator=(const JSExportCallbackHandler&) = default;
  JSExportCallbackHandler& operator=(JSExportCallbackHandler&&)      = default;

	virtual void     Initialize()                                                                                                                                 = 0;
	virtual void     Finalize(void* native_object_ptr)                                                                                                            = 0;
	virtual JSObject CallAsConstructor(JSObject&& constructor, const std::vector<JSValue>&& arguments)                                                            = 0;
	virtual bool     HasInstance(JSObject&& constructor, const JSValue&& possible_instance)                                                                 const = 0;
	virtual JSValue  GetNamedProperty(JSObject&& object, const JSString&& property_name)                                                                    const = 0;
	virtual bool     SetNamedProperty(JSObject&& object, const JSString&& property_name, const JSValue&& value)                                                   = 0;
	virtual JSValue  CallNamedFunction(JSObject&& function, const JSString&& property_name, const std::vector<JSValue>&& arguments, JSObject&& this_object)       = 0;
	virtual JSValue  CallAsFunction(JSObject&& function, const std::vector<JSValue>&& arguments, JSObject&& this_object)                                          = 0;
	// virtual JSValue  ConvertToType(JSObject&& object, const JSValue::Type&& type)                                                                           const = 0;
};

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKHANDLER_HPP_
