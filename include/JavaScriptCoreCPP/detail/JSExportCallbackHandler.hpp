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

#include <vector>

namespace JavaScriptCoreCPP { namespace detail {

class JSString;
class JSObject;

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

	virtual JSValue  GetNamedProperty(JSObject&& object, JSString&& property_name)                                    const = 0;
	virtual bool     SetNamedProperty(JSObject&& object, JSString&& property_name, JSValue&& value)                         = 0;
	virtual JSValue  CallNamedFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)       = 0;
	
	virtual void     Initialize(JSObject&& object)                                                                          = 0;
  virtual void     Finalize(JSObject&& object)                                                                            = 0;
  virtual JSValue  CallAsFunction(JSObject&& function, JSObject&& this_object, std::vector<JSValue>&& arguments)          = 0;
  virtual JSObject CallAsConstructor(JSObject&& constructor, std::vector<JSValue>&& arguments)                            = 0;
  virtual bool     HasInstance(const JSObject&& constructor, JSValue&& possible_instance)                           const = 0;
  virtual JSValue  ConvertToType(const JSObject&& object, JSValue::JSType&& type)                                   const = 0;
};

} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTCALLBACKHANDLER_HPP_
