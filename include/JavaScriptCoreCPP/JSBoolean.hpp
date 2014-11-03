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
		JSValue::operator=(static_cast<JSBoolean>(js_value));
		return *this;
  }

	JSBoolean& operator=(JSValue&& js_value) {
		JSValue::operator=(static_cast<JSBoolean>(js_value));
		return *this;
  }

	/*!
    @method
    
    @abstract Assign the given boolean value to the JSBoolean.
    
    @param boolean The bool to assign to the JSBoolean
    
    @result The JSBoolean with the new value of the given boolean.
  */
	JSBoolean& operator=(bool boolean) {
		JSValue::operator=(JSBoolean(get_context(), boolean));
		return *this;
	}

 private:

	// Only JSContext can create a JSBoolean.
	friend JSContext;
	
	JSBoolean(const JSContext& js_context, bool boolean)
			: JSValue(js_context, JSValueMakeBoolean(js_context, boolean)) {
	}
};

} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_JSBOOLEAN_HPP_
