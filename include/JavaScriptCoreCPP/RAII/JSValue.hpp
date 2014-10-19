/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_

#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include <stdexcept>
#include <iostream>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

class JSValue final	{
	
 public:

	/*!
	  @enum Type
	  @abstract     An enum identifying the type of a JSValue.
	  @constant     Undefined  The unique undefined value.
	  @constant     Null       The unique null value.
	  @constant     Boolean    A primitive boolean value, one of true or false.
	  @constant     Number     A primitive number value.
	  @constant     String     A primitive string value.
	  @constant     Object     An object value (meaning that this JSValueRef is a JSObjectRef).
	*/
	enum class Type {
		Undefined = kJSTypeUndefined,
		Null      = kJSTypeNull,
		Boolean   = kJSTypeBoolean,
		Number    = kJSTypeNumber,
		String    = kJSTypeString,
		Object    = kJSTypeObject
	};

	/*!
	  @function
	  @abstract Creates a JavaScript value of the null type.
	  @result   The unique null value.
	*/
	static JSValue MakeNull(const JSContext& js_context) {
		return JSValue(JSValueMakeNull(js_context), js_context);
	}


	// Converting to and from JSON formatted strings
	
	/*!
	  @function
	  @abstract         Creates a JavaScript value from a JSON formatted string.
	  @param string     The JSString containing the JSON string to be parsed.
	  @param js_context The execution context to use.
	  @throws           std::invalid_argument exception if the input is invalid.
	  @result            A JSValue containing the parsed value.
	*/
	static JSValue MakeFromJSONString(const JSString& string, const JSContext& js_context) {
		JSValueRef js_value_ref = JSValueMakeFromJSONString(js_context, string);
		if (!js_value_ref) {
			const std::string message = "Invalid JSON string: " + static_cast<std::string>(string);
			throw std::invalid_argument(message);
		}

		JSValue js_value(js_value_ref, js_context);
		JSValueUnprotect(js_context, js_value_ref);

		return js_value;
	}

	// Constructors.

	/*!
    @method
	  @abstract         Creates a JavaScript value of the undefined type.
    @param js_context The execution context to use.
	  @result           The unique undefined value.
  */
	JSValue(const JSContext& js_context) : js_value_(JSValueMakeUndefined(js_context)), js_context_(js_context) {
  }

	/*!
    @method
    @abstract         Creates a JavaScript value of the boolean type.
    @param boolean    The bool to assign to the newly created JSValue.
    @param js_context The execution context to use.
    @result           A JSValue of the boolean type, representing the value of boolean.
  */
	JSValue(bool boolean, const JSContext& js_context) : js_value_(JSValueMakeBoolean(js_context, boolean)), js_context_(js_context) {
  }

  /*!
    @method
    @abstract         Creates a JavaScript value of the number type.
    @param number     The double to assign to the newly created JSValue.
    @param js_context The execution context to use.
    @result           A JSValue of the number type, representing the value of number.
  */
	JSValue(double number, const JSContext& js_context) : js_value_(JSValueMakeNumber(js_context, number)), js_context_(js_context) {
	}
	
	/*!
	  @method
	  @abstract         Creates a JavaScript value of the string type.
	  @param string     The JSString to assign to the newly created JSValue.
    @param js_context The execution context to use.
	  @result           A JSValue of the string type, representing the value of string.
	*/
	JSValue(const JSString& string, const JSContext& js_context) : js_value_(JSValueMakeString(js_context, string)), js_context_(js_context) {
	}
	

	// Converting to primitive values.
	
	/*!
	  @method
	  @abstract Converts a JavaScript value to boolean and returns the resulting boolean.
	  @result   The boolean result of conversion.
	*/
	operator bool() const {
		return JSValueToBoolean(js_context_, js_value_);
	}
	
	/*!
	  @method
	  @abstract Converts a JavaScript value to number and returns the resulting number.
	  @throws   std::logic_error if the JSValue could not be converted to a double.
	  @result   The numeric result of conversion, or NaN if an exception is thrown.
	*/
	operator double() const {
		JSValueRef exception { nullptr };
		const double result = JSValueToNumber(js_context_, js_value_, &exception);
		if (exception) {
			const std::string message = "JSValue could not be converted to a double: " + static_cast<std::string>(JSValue(exception, js_context_));
			throw std::logic_error(message);
			// return std::numeric_limits<double>::quiet_NaN();
		}
		
		return result;
	}
	
	/*!
	  @method
	  @abstract Converts a JavaScript value to JSString.
	  @throws   std::logic_error if the JSValue could not be converted to a JSString.
	  @result   A JSString with the result of conversion.
	*/
	operator JSString() const {
		JSValueRef exception { nullptr };
		JSStringRef js_string_ref = JSValueToStringCopy(js_context_, js_value_, &exception);
		if (exception) {
			const std::string message = "JSValue could not be converted to a JSString: " + static_cast<std::string>(JSValue(exception, js_context_));
			throw std::logic_error(message);
		}
		
		JSString js_string(js_string_ref);
		JSStringRelease(js_string_ref);
		
		return js_string;
	}
	
	/*!
	  @method
	  @abstract Converts a JavaScript value to std::string.
	  @throws   std::logic_error if the JSValue could not be converted to a JSString.
	  @result   A JSString with the result of conversion.
	*/
	operator std::string() const {
		return operator JSString();
	}

	/*!
	  @method
	  @abstract Converts a JavaScript value to object and returns the resulting object.
	  @throws   std::logic_error if the JSValue could not be converted to a JSObject.
	  @result   The JSObject result of conversion.
	*/
	// operator JSObject() const {
	// 	JSValueRef exception { nullptr };
	// 	JSObjectRef js_object_ref = JSValueToObject(js_context_, js_value_, &exception);
	// 	if (exception) {
	// 		const std::string message = "JSValue could not be converted to a JSObject: " + static_cast<std::string>(JSValue(exception, value.get_js_context()));
	// 		throw std::logic_error(message);
	// 	}
		
	// 	return JSObject(js_object, js_context_);
	// }
	
  operator JSValueRef() const {
	  return js_value_;
  }

  operator JSContextRef() const {
	  return js_context_;
  }

  JSContext get_js_context() const {
	  return js_context_;
  }

  /*!
    @method
    @abstract       Returns a JavaScript value's type.
    @result         A value of type JSType that identifies value's type.
  */
  Type get_type() const {
	  const JSType js_type = JSValueGetType(js_context_, js_value_);
	  switch (js_type) {
		  case kJSTypeUndefined:
			  return Type::Undefined;
			  break;

		  case kJSTypeNull:
			  return Type::Null;
			  break;

		  case kJSTypeBoolean:
			  return Type::Boolean;
			  break;

		  case kJSTypeNumber:
			  return Type::Number;
			  break;

		  case kJSTypeString:
			  return Type::String;
			  break;

		  case kJSTypeObject:
			  return Type::Object;
			  break;

		  default:
			  const std::string message = "JSValue could not decode JSType = " + std::to_string(js_type);
			  throw std::logic_error(message);
	  }
  }
  
  /*!
    @method
    @abstract     Creates a JavaScript string containing the JSON serialized representation of a JS value.
    @param indent The number of spaces to indent when nesting.  If 0, the resulting JSON will not contain newlines. The size of the indent is clamped to 10 spaces.
    @throws       std::invalid_argument exception if the input is invalid.
    @result       A JSString with the result of serialization.
  */
  JSString ToJSONString(unsigned indent) {
	  JSValueRef exception { nullptr };
	  JSStringRef js_string_ref = JSValueCreateJSONString(js_context_, js_value_, indent, &exception);
	  if (exception) {
		  // assert(!js_string);
		  const std::string message = "JSValue could not be converted to a JSON string: " + static_cast<std::string>(JSValue(exception, js_context_));
		  throw std::invalid_argument(message);
	  }
	  
	  JSString js_string(js_string_ref);
	  JSStringRelease(js_string_ref);
	  
	  return js_string;
  }


	/*!
    @method
    @abstract       Tests whether a JavaScript value's type is the undefined type.
    @result         true if value's type is the undefined type, otherwise false.
  */
  bool isUndefined() const {
	  return JSValueIsUndefined(js_context_, js_value_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the null type.
    @result         true if value's type is the null type, otherwise false.
  */
  bool isNull() const {
	  return JSValueIsNull(js_context_, js_value_);
  }
  
  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the boolean type.
    @result         true if value's type is the boolean type, otherwise false.
  */
  bool isBoolean() const {
	  return JSValueIsBoolean(js_context_, js_value_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the number type.
    @result         true if value's type is the number type, otherwise false.
  */
  bool isNumber() const {
	  return JSValueIsNumber(js_context_, js_value_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the string type.
    @result         true if value's type is the string type, otherwise false.
  */
  bool isString() const {
	  return JSValueIsString(js_context_, js_value_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the object type.
    @result         true if value's type is the object type, otherwise false.
  */
  bool isObject() const {
	  return JSValueIsObject(js_context_, js_value_);
  }

  /*!
    @method
    @abstract Tests whether a JavaScript value is an object with a given class in its class chain.
    @param    jsClass The JSClass to test against.
    @result   true if value is an object and has jsClass in its class chain, otherwise false.
  */
  // bool isObjectOfClass(const JSClass& js_class) {
  //   return JSValueIsObjectOfClass(js_context_, js_value_, js_class);
  // }
  
  /*!
    @method
    @abstract          Tests whether a JavaScript value is an object constructed by a given constructor, as compared by the JS instanceof operator.
    @param constructor The constructor to test against.
    @result            true if value is an object constructed by constructor, as compared by the JS instanceof operator, otherwise false.
  */
  // bool isInstanceOfConstructor(const JSObject& constructor) {
	//   static const std::string log_prefix { "MDL: JSValue::operator==:" };
	  
	//   JSValueRef exception { nullptr };
	//   const bool result = JSValueIsInstanceOfConstructor(js_context_, js_value_, constructor, &exception);
	// 	if (exception) {
	// 		const std::string message = "JSValue caught exception during JSValueIsInstanceOfConstructor: " + static_cast<std::string>(JSValue(exception, value.get_js_context()));
	// 		std::clog << log_prefix << " [DEBUG] " << message << std::endl;
	// 		assert(result == false);
	// 	}
		
	// 	return result;
  // }
  
  ~JSValue() {
	  JSValueUnprotect(js_context_, js_value_);
  }
	
	// Copy constructor.
	JSValue(const JSValue& rhs) {
		js_value_   = rhs.js_value_;
		js_context_ = rhs.js_context_;
		JSValueProtect(js_context_, js_value_);
	}
	
  // Move constructor.
  JSValue(JSValue&& rhs) {
    js_value_   = rhs.js_value_;
    js_context_ = rhs.js_context_;
    JSValueProtect(js_context_, js_value_);
  }
  
  // Create a copy of another JSContextGroup by assignment. This is a unified
  // assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
  JSValue& operator=(JSValue rhs) {
    swap(*this, rhs);
    return *this;
  }
  
  friend void swap(JSValue& first, JSValue& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_value_  , second.js_value_);
    swap(first.js_context_, second.js_context_);
  }
  
private:

	JSValue(const JSValueRef& js_value, const JSContext& js_context) : js_value_(js_value), js_context_(js_context) {
		JSValueProtect(js_context_, js_value_);
	}
	
	// Return true if the two JSValues are equal as compared by the JS === operator.
	friend bool operator==(const JSValue& lhs, const JSValue& rhs);

	// Return true if the two JSValues are equal as compared by the JS == operator.
	friend bool isEqual(const JSValue& lhs, const JSValue& rhs);

	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSValueRef js_value_;
	JSContext  js_context_;
};

/*!
  @function
  @abstract Tests whether two JavaScript values are strict equal, as compared by the JS === operator.
  @param    lhs The first value to test.
  @param    rhs The second value to test.
  @result   true if the two values are equal, false if they are not equal.
*/
inline
bool operator==(const JSValue& lhs, const JSValue& rhs) {
	return JSValueIsStrictEqual(lhs.js_context_, lhs.js_value_, rhs.js_value_);
}

// Return true if the two JSValues are not strict equal, as compared by the JS === operator.
inline
bool operator!=(const JSValue& lhs, const JSValue& rhs) {
	return ! (lhs == rhs);
}

/*!
  @function
  @abstract Tests whether two JavaScript values are equal, as compared by the JS == operator.
  @param    lhs The first value to test.
  @param    rhs The second value to test.
  @result   true if the two values are equal, false if they are not equal.
*/
bool isEqual(const JSValue& lhs, const JSValue& rhs) {
	static const std::string log_prefix { "MDL: isEqual:" };
	
	JSValueRef exception { nullptr };
	const bool result = JSValueIsEqual(lhs.js_context_, lhs.js_value_, rhs.js_value_, &exception);
	if (exception) {
		const std::string message = "JSValues could not be converted to a double: " + static_cast<std::string>(JSValue(exception, lhs.js_context_));
		std::clog << log_prefix << " [DEBUG] " << message << std::endl; 
	}
	
	return result;
}

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
