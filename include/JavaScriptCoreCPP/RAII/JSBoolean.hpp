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

namespace JavaScriptCoreCPP { namespace RAII {

/*!
  @class
  @discussion A JavaScript value of the boolean type.
*/
class JSBoolean final : public JSValue {
	
public:
	
	/*!
    @method
    @abstract         Create a JavaScript value of the boolean type.
    @param boolean    The bool to assign to the newly created JSBoolean.
    @param js_context The execution context to use.
    @result           A JavaScript value of the boolean type, representing the value of boolean.
  */
	JSBoolean(bool boolean, const JSContext& js_context) : JSValue(js_context, JSValueMakeBoolean(js_context, boolean)) {
	}
	
	/*!
	  @method
	  @abstract       Create a JavaScript value of the number type from the given JavaScript value.
	  @param js_value The JSValue to convert.
	  @throws         std::invalid_argument if the given JavaScript value could not be converted to a JavaScript boolean.
	*/
	JSBoolean(const JSValue& js_value) : JSValue(js_value) {
		static const std::string log_prefix { "MDL: JSBoolean(const JSValue& js_value): " };
		if (!IsBoolean()) {
			const std::string message = "JSValue is not a boolean";
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
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
			JSValue::operator=(JSBoolean(boolean, js_context_));
		}
		return *this;
	}

	/*!
    @method
    @abstract       Assign the given JavaScript value to this JSBoolean.
    @param js_value The JavaScript value to assign to this JSBoolean
    @result         The JSBoolean with the new value of the given JavaScript value.
	  @throws         std::invalid_argument if the given JavaScript value is not a boolean.
	*/
	JSBoolean& operator=(const JSValue& js_value) {
		JSValue::operator=(JSBoolean(js_value));
		return *this;
  }

	/*!
	  @method
	  @abstract Convert a JSBoolean to a boolean.
	  @result   The boolean result of conversion.
	*/
	operator bool() const {
		return JSValueToBoolean(js_context_, js_value_ref_);
	}
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSBOOLEAN_HPP_
