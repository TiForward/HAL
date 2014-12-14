/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _HAL_JSNUMBER_HPP_
#define _HAL_JSNUMBER_HPP_

#include "HAL/JSValue.hpp"
#include <cstdint>

namespace HAL {

/*!
  @class
  
  @discussion A JavaScript value of the number type.

  The only way to create a JSNumber is by using the
  JSContext::CreateNumber member function.
*/
class HAL_EXPORT JSNumber final : public JSValue HAL_PERFORMANCE_COUNTER2(JSNumber) {
	
public:
	
	/*!
    @method
    
    @abstract Assign a double to this JSNumber.
    
    @param number The double to assign to this JSNumber
    
    @result The JSNumber with a new value of the given number.
  */
	JSNumber& operator=(double number) {
		return operator=(JSNumber(get_context(), number));
  }

	/*!
    @method
    
    @abstract Assign an int32_t to this JSNumber.
    
    @param number The int32_t to assign to this JSNumber
    
    @result The JSNumber with the new value of the given number.
  */
	JSNumber& operator=(int32_t number) {
		return operator=(JSNumber(get_context(), number));
  }

	/*!
    @method
    
    @abstract Assign an uint32_t to this JSNumber.
    
    @param number The uint32_t to assign to this JSNumber
    
    @result The JSNumber with the new value of the given number.
  */
	JSNumber& operator=(uint32_t number) {
		return operator=(JSNumber(get_context(), number));
  }

	/*!
    @method
    
    @abstract Convert the given JavaScript value to a JavaScript
    number and assign it to this JSNumber.
    
    @param js_value The JavaScript value to assign to this JSNumber
    
    @result The JSNumber with the new value of the given JavaScript
    value.
    
	  @throws std::invalid_argument if the given JavaScript value could
	  not be converted to a JavaScript number.
	*/
	JSNumber& operator=(const JSValue& js_value) {
		JSValue::operator=(static_cast<JSNumber>(js_value));
		return *this;
  }

	JSNumber& operator=(JSValue&& js_value) {
		JSValue::operator=(static_cast<JSNumber>(js_value));
		return *this;
	}

private:
	
	// Only a JSContext can create a JSNumber.
	friend JSContext;

	explicit JSNumber(const JSContext& js_context, double number = 0)
			: JSValue(js_context, JSValueMakeNumber(js_context, number)) {
	}
	
	JSNumber(const JSContext& js_context, int32_t number)
			: JSNumber(js_context, static_cast<double>(number)) {
	}
	
	JSNumber(const JSContext& js_context, uint32_t number)
			: JSNumber(js_context, static_cast<double>(number)) {
	}
};

} // namespace HAL {

#endif // _HAL_JSNUMBER_HPP_
