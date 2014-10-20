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
#include <iostream>
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

class JSObject;

/*!
  @class
  @discussion A JSValue is an RAII wrapper around a JSValueRef, the
  JavaScriptCore C API representation of a JavaScript value.
*/
class JSValue	{
	
 public:

	/*!
	  @enum Type
	  @abstract     An enum identifying the type of a JSValue.
	  @constant     Undefined  The unique undefined value.
	  @constant     Null       The unique null value.
	  @constant     Boolean    A primitive boolean value, one of true or false.
	  @constant     Number     A primitive number value.
	  @constant     String     A primitive string value.
	  @constant     Object     An object value (meaning that this JSValue is a JSObject).
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
	  @method
	  @abstract            Create a JavaScript value either of the string type, or from a JSON formatted string.
	  @param string        The JSString to either assign to the newly created JSValue, or that contains the JSON string to be parsed.
    @param js_context    The execution context to use.
    @param parse_as_json If true then parse string as a JSON formatted string, otherwise simply assign to the string to the newly created JavaScript value.
	  @result              A JSValue either of the string type that represents the value of string, or a JavaScript value containing the parsed JSON formatted string.
	  @throws              std::invalid_argument exception if parse_as_json is true but the string isn't a valid JSON formatted string.
	*/
	JSValue(const JSString& js_string, const JSContext& js_context, bool parse_as_json = false);

	/*!
	  @method
	  @abstract       Return a JSString containing the JSON serialized representation of this JavaScript value.
	  @param indent   The number of spaces to indent when nesting. If 0 (the default), the resulting JSON will not contain newlines. The size of the indent is clamped to 10 spaces.
	  @result         A JSString containing the JSON serialized representation of this JavaScript value.
	*/
	JSString ToJSONString(unsigned indent = 0);
	
	/*!
	  @method
	  @abstract Convert this JSValue to a JSString.
	  @result   A JSString with the result of conversion.
	  @throws   std::logic_error if this JSValue could not be converted to a JSString.
	*/
	operator JSString() const;
	
	/*!
	  @method
	  @abstract Convert this JSValue to a std::string.
	  @result   A std::string with the result of conversion.
	  @throws   std::logic_error if this JSValue could not be converted to a std::string.
	*/
	operator std::string() const {
		return operator JSString();
	}

	/*!
	  @method
	  @abstract Convert this JSValue to a JSObject and return the resulting object.
	  @result   The JSObject result of conversion.
	  @throws   std::logic_error if this JSValue could not be converted to a JSObject.
	*/
	// operator JSObject() const {
	// 	JSValueRef exception { nullptr };
	// 	JSObjectRef js_object_ref = JSValueToObject(js_context_, js_value_ref_, &exception);
	// 	if (exception) {
	// 		static const std::string log_prefix { "MDL: JSValue::operator JSObject() const: " };
	// 		std::ostringstream os;
	// 		os << "JSValue could not be converted to a JSObject: " << JSValue(exception, js_context_);
	// 		const std::string message = os.str();
	// 		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			
	// 		assert(!js_object_ref);
	// 		// if (js_object_ref) {
	// 		// 	JSValueUnprotect(js_context_, js_object_ref);
	// 		// }
			
	// 		throw std::logic_error(message);
	// 	}
		
	// 	assert(js_object_ref);
	// 	JSObject js_object(js_object, js_context_);
	// 	JSValueUnprotect(js_context_, js_object_ref);
		
	// 	return js_object;
	// }
	
	JSContext get_js_context() const {
		return js_context_;
	}

  /*!
    @method
    @abstract Return this JavaScript value's type.
    @result   A value of type JSValue::Type that identifies this JavaScript value's type.
  */
	Type GetType() const;


	/*!
    @method
    @abstract Tests whether this JavaScript value's type is the undefined type.
    @result   true if this JavaScript value's type is the undefined type, otherwise false.
  */
  bool IsUndefined() const {
	  return JSValueIsUndefined(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract Tests whether this JavaScript value's type is the null type.
    @result   true if this JavaScript value's type is the null type, otherwise false.
  */
  bool IsNull() const {
	  return JSValueIsNull(js_context_, js_value_ref_);
  }
  
  /*!
    @method
    @abstract Tests whether this JavaScript value's type is the boolean type.
    @result   true if this JavaScript value's type is the boolean type, otherwise false.
  */
  bool IsBoolean() const {
	  return JSValueIsBoolean(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract Tests whether this JavaScript value's type is the number type.
    @result   true if this JavaScript value's type is the number type, otherwise false.
  */
  bool IsNumber() const {
	  return JSValueIsNumber(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract Tests whether this JavaScript value's type is the string type.
    @result   true if this JavaScript value's type is the string type, otherwise false.
  */
  bool IsString() const {
	  return JSValueIsString(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract Tests whether this JavaScript value's type is the object type.
    @result   true if this JavaScript value's type is the object type, otherwise false.
  */
  bool IsObject() const {
	  return JSValueIsObject(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract          Determine whether this JavaScript value was constructed by the given constructor, as compared by the JS instanceof operator.
    @param constructor The constructor to test against.
    @result            true if this JavaScript value was constructed by the given constructor, as compared by the JS instanceof operator, otherwise false.
  */
  bool IsInstanceOfConstructor(const JSObject& constructor);

	virtual ~JSValue() {
	  JSValueUnprotect(js_context_, js_value_ref_);
  }

	// Copy constructor.
	JSValue(const JSValue& rhs) {
		js_value_ref_ = rhs.js_value_ref_;
		js_context_   = rhs.js_context_;
		JSValueProtect(js_context_, js_value_ref_);
	}
	
	// Move constructor.
  JSValue(JSValue&& rhs) {
    js_value_ref_ = rhs.js_value_ref_;
    js_context_   = rhs.js_context_;
    JSValueProtect(js_context_, js_value_ref_);
  }
  
  // Create a copy of another JSValue by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
  JSValue& operator=(JSValue rhs) {
    swap(*this, rhs);
    return *this;
  }
  
	void swap(JSValue& first, JSValue& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_value_ref_, second.js_value_ref_);
    swap(first.js_context_  , second.js_context_);
  }
  
private:

  // For interoperability with the JavaScriptCore C API.
	JSValue(const JSValueRef& js_value_ref, const JSContext& js_context) : js_value_ref_(js_value_ref), js_context_(js_context) {
		// if (js_value_ref) {
		// 	JSValueProtect(js_context_, js_value_ref_);
		// } else {
		// 	js_value_ref_ = JSValueMakeUndefined(js_context);
		// }

		assert(js_value_ref_);
		JSValueProtect(js_context_, js_value_ref_);
	}
	
  // For interoperability with the JavaScriptCore C API.
  operator JSValueRef() const {
	  return js_value_ref_;
  }

  // For interoperability with the JavaScriptCore C API.
  operator JSContextRef() const {
	  return js_context_;
  }

  friend class JSUndefined;
  friend class JSNull;
  friend class JSBoolean;
  friend class JSNumber;
  friend class JSObject;
  friend class JSPropertyNameArray;
  friend class JSArray;

	// Return true if the two JSValues are equal as compared by the JS === operator.
	friend bool operator==(const JSValue& lhs, const JSValue& rhs);

	// Return true if the two JSValues are equal as compared by the JS == operator.
	friend bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);
	
	// Prevent heap based objects.
	static void * operator new(size_t);			 // #1: To prevent allocation of scalar objects
	static void * operator new [] (size_t);	 // #2: To prevent allocation of array of objects
	
	JSValueRef js_value_ref_;
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
	return JSValueIsStrictEqual(lhs, lhs, rhs);
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
bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);

inline
std::ostream& operator << (std::ostream& ostream, const JSValue& js_value) {
	ostream << static_cast<std::string>(js_value);
	return ostream;
}

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
