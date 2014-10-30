/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSBOOLEAN_HPP_
#define _JAVASCRIPTCORECPP_JSBOOLEAN_HPP_

#include "JavaScriptCoreCPP/JSValue.hpp"
#include <stdexcept>
#include <iostream>

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion A JavaScript value of the boolean type.

  The only way to create a JSBoolean is by using the
  JSContext::CreateBoolean member function.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSBoolean final : public JSValue, public detail::JSPerformanceCounter<JSBoolean> {
#else
class JSBoolean final : public JSValue {
#endif
	
 public:
	
	/*!
    @method
    
    @abstract Assign the given JavaScript value to this JSBoolean.
    
    @param js_value The JavaScript value to assign to this JSBoolean
    
    @result The JSBoolean with the new value of the given JavaScript
    value.
    
	  @throws std::invalid_argument if the given JavaScript value could
	  not be converted to a boolean.
	*/
	JSBoolean& operator=(const JSValue& js_value) {
		JSValue::operator JSBoolean();
		return *this;
  }

	/*!
	  @method
	  
	  @abstract Convert a JSBoolean to a boolean.
	  
	  @result The boolean result of conversion.
	*/
	operator bool() const {
		return JSValueToBoolean(get_context(), *this);
	}

	/*!
    @method
    
    @abstract Assign the given boolean value to the JSBoolean.
    
    @param boolean The bool to assign to the JSBoolean
    
    @result The JSBoolean with the new value of the given boolean.
  */
	JSBoolean& operator=(bool boolean) {
		const bool current_value = operator bool();
		if (current_value != boolean) {
			JSValue::operator=(JSBoolean(get_context(), boolean));
		}
		return *this;
	}

 private:

	JSBoolean(const JSContext& js_context, bool boolean) : JSValue(js_context, JSValueMakeBoolean(js_context, boolean)) {
	}
	
	// Only JSContext and JSValue can create a JSBoolean.
	friend JSContext;
	friend JSValue;
};

}} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSBOOLEAN_HPP_
