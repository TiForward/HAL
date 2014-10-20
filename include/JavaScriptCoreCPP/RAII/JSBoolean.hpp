/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSBOOLEAN_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSBOOLEAN_HPP_

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include <stdexcept>
#include <iostream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

/*!
  @class
  @discussion A JavaScript value of the boolean type.
*/
class JSBoolean final	{
	
public:
	
	/*!
    @method
    @abstract         Create a JavaScript value of the boolean type.
    @param boolean    The bool to assign to the newly created JSBoolean.
    @param js_context The execution context to use.
    @result           A JavaScript value of the boolean type, representing the value of boolean.
  */
	JSBoolean(bool boolean, const JSContext& js_context) : js_value_(JSValueMakeBoolean(js_context, boolean), js_context) {
	}
	
	/*!
    @method
    @abstract      Assign the given boolean value to the JSBoolean.
    @param boolean The bool to assign to the JSBoolean
    @result        The JSBoolean with the new value of the given boolean.
  */
	JSBoolean& operator=(bool boolean) {
		const bool current_value = operator bool();
		if (current_value != boolean) {
			js_value_ = JSBoolean(boolean, js_value_.get_js_context());
		}
    return *this;
  }

  /*!
	  @method
	  @abstract Convert a JSBoolean to a boolean.
	  @result   The boolean result of conversion.
	*/
	operator bool() const {
		return JSValueToBoolean(js_value_, js_value_);
	}

	/*!
	  @method
	  @abstract Converts a JSBoolean into a JSValue.
	  @result   The JSValue result of conversion.
	*/
	operator JSValue() const {
		return js_value_;
	}
	
private:
	
	/*!
	  @method
	  @abstract       Create a JavaScript value of the boolean type from the given JSValue.
	  @param js_value The JSValue to convert.
	  @throws         std::logic_error if the JSValue could not be converted to a boolean.
	*/
	JSBoolean(const JSValue& js_value) : js_value_(js_value) {
		static const std::string log_prefix { "MDL: JSBoolean(const JSValue& js_value): " };
		if (!js_value.IsBoolean()) {
			const std::string message = "JSValue is not a boolean value";
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::logic_error(message);
		}
	}
	
	JSValue js_value_;
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSBOOLEAN_HPP_
