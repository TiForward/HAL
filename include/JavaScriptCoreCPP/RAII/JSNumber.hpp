/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNUMBER_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNUMBER_HPP_

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include <cstdint>
#include <stdexcept>

namespace JavaScriptCoreCPP {

/*!
  @class
  @discussion A JavaScript value of the boolean type.
*/
class JSNumber final : public JSValue {
	
public:
	
  /*!
    @method
    @abstract         Create a JavaScript value of the number type with the value 0.
    @param js_context The execution context to use.
    @result           A JavaScript value of the number type, representing the value of 0.
  */
	JSNumber(const JSContext& js_context) : JSNumber(0, js_context) {
	}

	/*!
    @method
    @abstract         Create a JavaScript value of the number type.
    @param number     The double to assign to the newly created JSNumber.
    @param js_context The execution context to use.
    @result           A JavaScript value of the number type, representing the value of number.
  */
	JSNumber(double number, const JSContext& js_context) : JSValue(JSValueMakeNumber(js_context, number), js_context) {
	}

  /*!
    @method
    @abstract         Create a JavaScript value of the number type.
    @param number     The int32_t to assign to the newly created JSNumber.
    @param js_context The execution context to use.
    @result           A JavaScript value of the number type, representing the value of number.
  */
	JSNumber(int32_t number, const JSContext& js_context) : JSNumber(static_cast<double>(number), js_context) {
	}

  /*!
    @method
    @abstract         Create a JavaScript value of the number type.
    @param number     The uint32_t to assign to the newly created JSNumber.
    @param js_context The execution context to use.
    @result           A JavaScript value of the number type, representing the value of number.
  */
	JSNumber(uint32_t number, const JSContext& js_context) : JSNumber(static_cast<double>(number), js_context) {
	}

	/*!
	  @method
	  @abstract       Create a JavaScript value of the number type from the given JavaScript value.
	  @param js_value The JSValue to convert.
	  @throws         std::invalid_argument if the given JavaScript value could not be converted to a JavaScript number.
	*/
	JSNumber(const JSValue& js_value) : JSValue(js_value) {
		static const std::string log_prefix { "MDL: JSNumber(const JSValue& js_value): " };
		if (!IsNumber()) {
			const std::string message = "JSValue is not a number value";
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
	}
	
	/*!
    @method
    @abstract     Assign the given number to this JSNumber.
    @param number The number to assign to this JSNumber
    @result       The JSNumber with the new value of the given number.
  */
	JSNumber& operator=(double number) {
		return operator=(JSNumber(number, js_context_));
  }

	/*!
    @method
    @abstract     Assign the given number to this JSNumber.
    @param number The number to assign to this JSNumber
    @result       The JSNumber with the new value of the given number.
  */
	JSNumber& operator=(int32_t number) {
		return operator=(JSNumber(number, js_context_));
  }

	/*!
    @method
    @abstract     Assign the given number to this JSNumber.
    @param number The number to assign to this JSNumber
    @result       The JSNumber with the new value of the given number.
  */
	JSNumber& operator=(uint32_t number) {
		return operator=(JSNumber(number, js_context_));
  }

	/*!
    @method
    @abstract       Assign the given JavaScript value to this JSNumber.
    @param js_value The JavaScript value to assign to this JSNumber
    @result         The JSNumber with the new value of the given JavaScript value.
	  @throws         std::invalid_argument if the given JavaScript value is not a number.
	*/
	JSNumber& operator=(const JSValue& js_value) {
		JSValue::operator=(JSNumber(js_value));
		return *this;
  }

	/*!
	  @method
	  @abstract Convert a JSNumber to a double.
	  @result   The double result of conversion.
	  @throws   std::logic_error if this JSNumber could not be converted to a double.
	*/
	operator double() const;

	/*!
	  @method
	  @abstract Convert a JSNumber to an int32_t according to the rules specified by the JavaScript language.
	  @result   The int32_t result of conversion.
	  @throws   std::logic_error if this JSNumber could not be converted to an int32_t.
	*/
	explicit operator int32_t() const;
	
	/*!
	  @method
	  @abstract Convert a JSNumber to an uint32_t according to the rules specified by the JavaScript language.
	  @discussion The JSNumber is converted to an uint32_t according to
	  the rules specified by the JavaScript language (implements
	  ToUInt32, defined in ECMA-262 9.6).
	  @result The uint32_t result of the conversion.
	  @throws   std::logic_error if this JSNumber could not be converted to an uint32_t.
	*/
	explicit operator uint32_t() const  {
		// As commented in the spec, the operation of ToInt32 and ToUint32
		// only differ in how the result is interpreted; see NOTEs in
		// sections 9.5 and 9.6.
		return operator int32_t();
	}
};

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSNUMBER_HPP_
