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
#include <sstream>
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

class JSValue;
std::ostream& operator << (std::ostream& ostream, const JSValue& js_value);

/*!
  @class
  @discussion A JSValue is an RAII wrapper around a JSValueRef, the
  JavaScriptCore C API representation of a JavaScript value.
*/
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

	// Converting to and from JSON formatted strings
	
	/*!
	  @function
	  @abstract         Create a JavaScript value from a JSON formatted string.
	  @param string     The JSString containing the JSON string to be parsed.
	  @param js_context The execution context to use.
	  @result           A JSValue containing the parsed value.
	  @throws           std::invalid_argument exception if the input is invalid.
	*/
	static JSValue FromJSONString(const JSString& js_string, const JSContext& js_context) {
		JSValueRef js_value_ref = JSValueMakeFromJSONString(js_context, js_string);
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
		
		return js_value;
	}
	
	/*!
	  @method
	  @abstract       Create a JSString containing the JSON serialized representation of the given JSValue.
	  @param js_value The JSValue to serialize to its JSON representation.
	  @param indent   The number of spaces to indent when nesting. If 0 (the default), the resulting JSON will not contain newlines. The size of the indent is clamped to 10 spaces.
	  @result         A JSString with the result of serialization.
	  @throws         std::invalid_argument exception if the input is invalid.
	*/
	static JSString ToJSONString(const JSValue& js_value, unsigned indent = 0) {
		JSValueRef exception { nullptr };
		JSStringRef js_string_ref = JSValueCreateJSONString(js_value, js_value, indent, &exception);
		if (exception) {
			// assert(!js_string_ref);
			static const std::string log_prefix { "MDL: JSValueCreateJSONString: " };
			std::ostringstream os;
			os << "JSValue could not be serialized to a JSON string: " << JSValue(exception, js_value.get_js_context());
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
		
		JSString js_string;
		if (js_string_ref) {
			js_string = JSString(js_string_ref);
			JSStringRelease(js_string_ref);
		}
		
		return js_string;
	}
	
	// Constructors.
	
	/*!
	  @method
	  @abstract         Create a JavaScript value of the string type.
	  @param string     The JSString to assign to the newly created JSValue.
    @param js_context The execution context to use.
	  @result           A JSValue of the string type, representing the value of string.
	*/
	JSValue(const JSString& string, const JSContext& js_context) : js_value_ref_(JSValueMakeString(js_context, string)), js_context_(js_context) {
	}

	/*!
	  @method
	  @abstract Convert this JSValue to a JSString.
	  @result   A JSString with the result of conversion.
	  @throws   std::logic_error if this JSValue could not be converted to a JSString.
	*/
	operator JSString() const {
		JSValueRef exception { nullptr };
		JSStringRef js_string_ref = JSValueToStringCopy(js_context_, js_value_ref_, &exception);
		if (exception) {
			static const std::string log_prefix { "MDL: JSValue::operator JSString() const: " };
			std::ostringstream os;
			os << "JSValue could not be converted to a JSString: "<< JSValue(exception, js_context_);
			const std::string message = os.str();
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::logic_error(message);
		}

		assert(js_string_ref);
		JSString js_string(js_string_ref);
		JSStringRelease(js_string_ref);
		
		return js_string;
	}
	
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
    @abstract       Returns a JavaScript value's type.
    @result         A value of type JSType that identifies value's type.
    @throws         std::logic_error if the JSValue's type could not be determined.
  */
  Type get_type() const {
	  const JSType js_type = JSValueGetType(js_context_, js_value_ref_);
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
			  static const std::string log_prefix { "MDL: JSValue::get_type() const: " };
			  const std::string message = "JSValue could not decode JSType = " + std::to_string(js_type);
			  std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			  throw std::logic_error(message);
	  }
  }


	/*!
    @method
    @abstract       Tests whether a JavaScript value's type is the undefined type.
    @result         true if value's type is the undefined type, otherwise false.
  */
  bool IsUndefined() const {
	  return JSValueIsUndefined(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the null type.
    @result         true if value's type is the null type, otherwise false.
  */
  bool IsNull() const {
	  return JSValueIsNull(js_context_, js_value_ref_);
  }
  
  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the boolean type.
    @result         true if value's type is the boolean type, otherwise false.
  */
  bool IsBoolean() const {
	  return JSValueIsBoolean(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the number type.
    @result         true if value's type is the number type, otherwise false.
  */
  bool IsNumber() const {
	  return JSValueIsNumber(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the string type.
    @result         true if value's type is the string type, otherwise false.
  */
  bool IsString() const {
	  return JSValueIsString(js_context_, js_value_ref_);
  }

  /*!
    @method
    @abstract       Tests whether a JavaScript value's type is the object type.
    @result         true if value's type is the object type, otherwise false.
  */
  bool IsObject() const {
	  return JSValueIsObject(js_context_, js_value_ref_);
  }

  ~JSValue() {
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
    swap(first.js_value_ref_, second.js_value_ref_);
    swap(first.js_context_  , second.js_context_);
  }
  
private:

  // For interoperability with the JavaScriptCore C API.
	JSValue(const JSValueRef& js_value_ref, const JSContext& js_context) : js_value_ref_(js_value_ref), js_context_(js_context) {
		if (js_value_ref) {
			JSValueProtect(js_context_, js_value_ref_);
		} else {
			js_value_ref_ = JSValueMakeUndefined(js_context);
		}
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
inline
bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs) {
	JSValueRef exception { nullptr };
	//const bool result = JSValueIsEqual(lhs.js_context_, lhs.js_value_ref_, rhs.js_value_ref_, &exception);
	const bool result = JSValueIsEqual(lhs, lhs, rhs, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: IsEqualWithTypeCoercion: " };
		std::ostringstream os;
		os << "caught exception: " << JSValue(exception, lhs.js_context_);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl; 
	}
	
	return result;
}

inline
std::ostream& operator << (std::ostream& ostream, const JSValue& js_value) {
	ostream << static_cast<std::string>(js_value);
	return ostream;
}

} // namespace JavaScriptCoreCPP

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
