/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_

#include "JavaScriptCoreCPP/RAII/JSContext.hpp"
#include "JavaScriptCoreCPP/RAII/JSString.hpp"
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#include <iostream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace detail {
class JSPropertyNameArray;
}}

namespace JavaScriptCoreCPP { namespace RAII {

class JSObject;

/*!
  @class
  
  @discussion A JSValue is an RAII wrapper around a JSValueRef, the
  JavaScriptCore C API representation of a JavaScript value.

  The only way to create a JSValue is by using the
  JSContext::CreateValue member function.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER
class JSValue	: public ::JavaScriptCoreCPP::detail::JSPerformanceCounter<JSValue> {
#else
class JSValue	{
#endif
	
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

public:
	
	/*!
	  @method
	  
	  @abstract Return a JSString containing the JSON serialized
	  representation of this JavaScript value.
	  
	  @param indent The number of spaces to indent when nesting. If 0
	  (the default), the resulting JSON will not contain newlines. The
	  size of the indent is clamped to 10 spaces.
	  
	  @result A JSString containing the JSON serialized representation
	  of this JavaScript value.
	*/
	JSString ToJSONString(unsigned indent = 0);
	
	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSString.
	  
	  @result A JSString with the result of conversion.
	  
	  @throws std::logic_error if this JSValue could not be converted to
	  a JSString.
	*/
	operator JSString() const;
	
	/*!
	  @method
	  
	  @abstract Convert this JSValue to a std::string.
	  
	  @result A std::string with the result of conversion.
	  
	  @throws std::logic_error if this JSValue could not be converted to
	  a std::string.
	*/
	operator std::string() const {
		return operator JSString();
	}

	/*!
	  @method
	  
	  @abstract Convert a JSValue to a boolean.
	  
	  @result The boolean result of conversion.
	*/
	explicit operator bool() const {
		return JSValueToBoolean(js_context_, js_value_ref_);
	}
	
	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSBoolean.
	  
	  @result A JSBoolean with the result of conversion.
	  
	  @throws std::logic_error if this JSValue could not be converted to
	  a JSBoolean.
	*/
	operator JSBoolean() const;

	/*!
	  @method
	  
	  @abstract Convert a JSValue to a double.
	  
	  @result The double result of conversion.
	*/
	explicit operator double() const;

	/*!
	  @method
	  
	  @abstract Convert a JSValue to an int32_t according to the rules
	  specified by the JavaScript language.
	  
	  @result The int32_t result of conversion.
	*/
	explicit operator int32_t() const;
	
	/*!
	  @method
	  
	  @abstract Convert a JSValue to an uint32_t according to the rules
	  specified by the JavaScript language.
	  
	  @discussion The JSValue is converted to an uint32_t according to
	  the rules specified by the JavaScript language (implements
	  ToUInt32, defined in ECMA-262 9.6).
	  
	  @result The uint32_t result of the conversion.
	*/
	explicit operator uint32_t() const  {
		// As commented in the spec, the operation of ToInt32 and ToUint32
		// only differ in how the result is interpreted; see NOTEs in
		// sections 9.5 and 9.6.
		return operator int32_t();
	}

	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSNumber.
	  
	  @result A JSNumber with the result of conversion.
	  
	  @throws std::logic_error if this JSValue could not be converted to
	  a JSNumber.
	*/
	operator JSNumber() const;

	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSObject.
	  
	  @result A JSObject with the result of conversion.
	  
	  @throws std::logic_error if this JSValue could not be converted to
	  a JSObject.
	*/
	operator JSObject() const;

	JSContext get_js_context() const {
		return js_context_;
	}

  /*!
    @method
    
    @abstract Return this JavaScript value's type.
    
    @result A value of type JSValue::Type that identifies this
    JavaScript value's type.
  */
	Type GetType() const;

	/*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    undefined type.
    
    @result true if this JavaScript value's type is the undefined
    type, otherwise false.
  */
  bool IsUndefined() const {
	  return JSValueIsUndefined(js_context_, js_value_ref_);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    null type.
    
    @result true if this JavaScript value's type is the null type,
    otherwise false.
  */
  bool IsNull() const {
	  return JSValueIsNull(js_context_, js_value_ref_);
  }
  
  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    boolean type.
    
    @result true if this JavaScript value's type is the boolean type,
    otherwise false.
  */
  bool IsBoolean() const {
	  return JSValueIsBoolean(js_context_, js_value_ref_);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    number type.
    
    @result true if this JavaScript value's type is the number type,
    otherwise false.
  */
  bool IsNumber() const {
	  return JSValueIsNumber(js_context_, js_value_ref_);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    string type.
    
    @result true if this JavaScript value's type is the string type,
    otherwise false.
  */
  bool IsString() const {
	  return JSValueIsString(js_context_, js_value_ref_);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    object type.
    
    @result true if this JavaScript value's type is the object type,
    otherwise false.
  */
  bool IsObject() const {
	  return JSValueIsObject(js_context_, js_value_ref_);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value was constructed
    by the given constructor, as compared by the JS instanceof
    operator.
    
    @param constructor The constructor to test against.
    
    @result true if this JavaScript value was constructed by the given
    constructor, as compared by the JS instanceof operator, otherwise
    false.
  */
	bool IsInstanceOfConstructor(const JSObject& constructor);

	virtual ~JSValue() {
	  JSValueUnprotect(js_context_, js_value_ref_);
  }

	// Copy constructor.
	JSValue(const JSValue& rhs)
			: js_context_(rhs.js_context_)
			, js_value_ref_(rhs.js_value_ref_) {
		JSValueProtect(js_context_, js_value_ref_);
	}
	
	// Move constructor.
	JSValue(JSValue&& rhs) : js_context_(rhs.js_context_), js_value_ref_(rhs.js_value_ref_) {
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
  
	friend void swap(JSValue& first, JSValue& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_context_  , second.js_context_);
    swap(first.js_value_ref_, second.js_value_ref_);
  }
  
 protected:

	// Only a JSContext can create a JSValue.
	JSValue(const JSContext& js_context, const JSString& js_string, bool parse_as_json = false);

 private:

	// For interoperability with the JavaScriptCore C API.
	JSValue(JSContextRef js_context_ref, JSValueRef js_value_ref) : js_context_(js_context_ref), js_value_ref_(js_value_ref)  {
		assert(js_value_ref_);
		JSValueProtect(js_context_, js_value_ref_);
	}

  // For interoperability with the JavaScriptCore C API.
	operator JSContextRef() const {
	  return js_context_;
  }

	// For interoperability with the JavaScriptCore C API.
  operator JSValueRef() const {
	  return js_value_ref_;
  }

  friend class JSContext;
  friend class JSUndefined;
  friend class JSNull;
  friend class JSBoolean;
  friend class JSNumber;
  friend class JSObject;
	friend class detail::JSPropertyNameArray;
  friend class JSArray;
  friend class JSDate;
  friend class JSError;
  friend class JSRegExp;
  friend class JSFunction;

	template<typename T>
	friend class JSNativeObject;

	// Return true if the two JSValues are equal as compared by the JS === operator.
	friend bool operator==(const JSValue& lhs, const JSValue& rhs);

	// Return true if the two JSValues are equal as compared by the JS == operator.
	friend bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);
	
	JSContext  js_context_;
	JSValueRef js_value_ref_ { nullptr };
};

/*!
  @function
  @abstract Determine whether two JavaScript values are strict equal, as compared by the JS === operator.
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
  
  @abstract Determine whether two JavaScript values are equal, as
  compared by the JS == operator.
  
  @param lhs The first value to test.
  
  @param rhs The second value to test.
  
  @result true if the two values are equal, false if they are not
  equal.
*/
bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs);

inline
std::ostream& operator << (std::ostream& ostream, const JSValue& js_value) {
	ostream << static_cast<std::string>(js_value);
	return ostream;
}

std::string to_string(const JSValue::Type& js_value_type);

inline
std::ostream& operator << (std::ostream& ostream, const JSValue::Type& js_value_type) {
	ostream << to_string(js_value_type);
	return ostream;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSVALUE_HPP_
