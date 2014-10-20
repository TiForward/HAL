/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSUNDEFINED_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSUNDEFINED_HPP_

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include <stdexcept>
#include <iostream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @class
  @discussion A JavaScript value of the undefined type.
*/
class JSUndefined final	{
	
public:
	
	/*!
	  @method
	  @abstract         Create a JavaScript value of the undefined type.
	  @param js_context The execution context to use.
	  @result           The unique undefined value.
	*/
	JSUndefined(const JSContext& js_context) : js_value_(JSValueMakeUndefined(js_context), js_context) {
	}
	
	/*!
	  @method
	  @abstract Converts a JSUndefined into a JSValue.
	  @result   The JSValue result of conversion.
	*/
	operator JSValue() const {
		return js_value_;
	}
	
private:
	
	/*!
	  @method
	  @abstract       Create a JavaScript value of the undefined type from the given JSValue.
	  @param js_value The JSValue to convert.
	  @result         The unique undefined value.
	  @throws         std::logic_error if the JSValue is not the undefined value.
	*/
	JSUndefined(const JSValue& js_value) : js_value_(js_value) {
		static const std::string log_prefix { "MDL: JSUndefined(const JSValue& js_value): " };
		if (!js_value.IsUndefined()) {
			const std::string message = "JSValue is not the undefined value";
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::logic_error(message);
		}
	}

	JSValue js_value_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSUNDEFINED_HPP_
