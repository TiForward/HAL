/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSDATE_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSDATE_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <sstream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @class
  @discussion A JSDate is an RAII wrapper around a JSDateRef, the
  JavaScriptCore C API representation of a JavaScript object. A
  JSDate is a JSValue.
*/
class JSDate final	{
	
 public:

	/*!
	  @method
	  @abstract         Create a JavaScript Date object, as if by invoking the built-in Date constructor.
	  @param js_context The execution context to use.
    @result           A JavaScript value of the Date type.
	*/
	JSDate(const JSContext& js_context) : js_value_(JSObjectMakeDate(js_context, boolean), js_context) {
		JSObjectRef js_object_ref = JSObjectMakeDate(js_context, js_string);
		if (!js_value_ref) {
			static const std::string log_prefix { "MDL: JSONStringToJSValue: " };
			std::ostringstream os;
			os << "Input is not a valid JSON string: " << js_string;
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl; 
			throw std::invalid_argument(message);
		}
		
		JSValue js_value(js_value_ref, js_context);
		JSValueUnprotect(js_context, js_value_ref);
	}


 private:
	
	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects

	JSObject js_date_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSDATE_HPP_
