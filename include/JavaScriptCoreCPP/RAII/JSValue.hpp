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
#include "JavaScriptCoreCPP/RAII/detail/JSPerformanceCounter.hpp"
#include <iostream>
#include <sstream>
#include <cassert>


#ifdef JAVASCRIPTCORECPP_RAII_THREAD_SAFE
#include <mutex>

#undef  JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_TYPE
#define JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_TYPE std::recursive_mutex

#undef  JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME_PREFIX
#define JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME_PREFIX js_context

#undef  JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME
#define JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME_PREFIX##_mutex_

#undef  JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_TYPE JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME;

#undef  JAVASCRIPTCORECPP_RAII_JSVALUE_LOCK_GUARD
#define JAVASCRIPTCORECPP_RAII_JSVALUE_LOCK_GUARD std::lock_guard<JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_TYPE> JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME_PREFIX##_lock(JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX_NAME);

#else
#define JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX
#define JAVASCRIPTCORECPP_RAII_JSVALUE_LOCK_GUARD
#endif  // JAVASCRIPTCORECPP_RAII_THREAD_SAFE


namespace JavaScriptCoreCPP { namespace detail {
class JSPropertyNameArray;
}}

namespace JavaScriptCoreCPP { namespace RAII {

class JSObject;
class JSFunction;
class JSArray;
class JSDate;
class JSError;
class JSRegExp;

/*!
  @class
  
  @discussion A JSValue is an RAII wrapper around a JSValueRef, the
  JavaScriptCore C API representation of a JavaScript value.
  
  The only way to create a JSValue is by using the
  JSContext::CreateXXX member functions.
*/
#ifdef JAVASCRIPTCORECPP_RAII_PERFORMANCE_COUNTER_ENABLE
class JSValue	: public detail::JSPerformanceCounter<JSValue> {
#else
class JSValue {
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
	*/
	explicit operator JSString() const;
	
	/*!
	  @method
	  
	  @abstract Convert this JSValue to a std::string.
	  
	  @result A std::string with the result of conversion.
	*/
	explicit operator std::string() const {
		return operator JSString();
	}

	/*!
	  @method
	  
	  @abstract Convert a JSValue to a boolean.
	  
	  @result The boolean result of conversion.
	*/
	explicit operator bool() const {
		return JSValueToBoolean(js_context__, js_value_ref__);
	}
	
	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSBoolean.
	  
	  @result A JSBoolean with the result of conversion.
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
	*/
	operator JSNumber() const;

	/*!
	  @method
	  
	  @abstract Convert this JSValue to a JSObject.
	  
	  @result A JSObject with the result of conversion.
	  
	  @throws std::runtime_error if this JSValue could not be converted
	  to a JSObject.
	*/
	operator JSObject() const;

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
    type.
  */
  bool IsUndefined() const {
	  return JSValueIsUndefined(js_context__, js_value_ref__);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    null type.
    
    @result true if this JavaScript value's type is the null type.
  */
  bool IsNull() const {
	  return JSValueIsNull(js_context__, js_value_ref__);
  }
  
  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    boolean type.
    
    @result true if this JavaScript value's type is the boolean type.
  */
  bool IsBoolean() const {
	  return JSValueIsBoolean(js_context__, js_value_ref__);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    number type.
    
    @result true if this JavaScript value's type is the number type.
  */
  bool IsNumber() const {
	  return JSValueIsNumber(js_context__, js_value_ref__);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    string type.
    
    @result true if this JavaScript value's type is the string type.
  */
  bool IsString() const {
	  return JSValueIsString(js_context__, js_value_ref__);
  }

  /*!
    @method
    
    @abstract Determine whether this JavaScript value's type is the
    object type.
    
    @result true if this JavaScript value's type is the object type.
  */
	bool IsObject() const {
		return JSValueIsObject(js_context__, js_value_ref__);
	}
	
	/*!
	  @method
	  
	  @abstract Determine whether this JavaScript's value is an object
	  with a given class in its class chain.
	  
	  @param jsClass The JSClass to test against.
	  
	  @result true if this JavaScript value is an object with a given
	  class in its class chain.
	*/
	bool IsObjectOfClass(const JSClass& js_class) const {
		return JSValueIsObjectOfClass(js_context__, js_value_ref__, js_class);
	}

	/*!
    @method
    
    @abstract Determine whether this JavaScript value was constructed
    by the given constructor, as compared by the JavaScript
    'instanceof' operator.
    
    @param constructor The constructor to test against.
    
    @result true if this JavaScript value was constructed by the given
    constructor as compared by the JavaScript 'instanceof' operator.
  */
	bool IsInstanceOfConstructor(const JSObject& constructor) const;

	/*!
	  @method
	  
	  @abstract Determine whether this JavaScript value is equal to
	  another JavaScript by usong the JavaScript == operator.
	  
	  @param js_value The JavaScript value to test.
	  
	  @result true this JavaScript value is equal to another JavaScript
	  by usong the JavaScript == operator.
	*/
	bool IsEqualWithTypeCoercion(const JSValue& js_value) const;

	/*!
	  @method
	  
	  @abstract Return the execution context of this JavaScript value.
	  
	  @result The the execution context of this JavaScript value.
	*/
	JSContext get_context() const {
		return js_context__;
	}
	
	virtual ~JSValue() {
	  JSValueUnprotect(js_context__, js_value_ref__);
  }

	// Copy constructor.
	JSValue(const JSValue& rhs)
			: js_context__(rhs.js_context__)
			, js_value_ref__(rhs.js_value_ref__) {
		JSValueProtect(js_context__, js_value_ref__);
	}
	
	// Move constructor.
	JSValue(JSValue&& rhs)
			: js_context__(rhs.js_context__)
			, js_value_ref__(rhs.js_value_ref__) {
  }
  
#ifdef JAVASCRIPTCORECPP_RAII_MOVE_SEMANTICS_ENABLE
  JSValue& JSValue::operator=(const JSValue&) = default;
  JSValue& JSValue::operator=(JSValue&&) = default;
#endif

	// Create a copy of another JSValue by assignment. This is a unified
	// assignment operator that fuses the copy assignment operator,
  // X& X::operator=(const X&), and the move assignment operator,
  // X& X::operator=(X&&);
	JSValue& operator=(JSValue rhs) {
		JAVASCRIPTCORECPP_RAII_JSVALUE_LOCK_GUARD;
	  // Values can only be copied between contexts within the same
	  // context group.
		if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
		  static const std::string log_prefix { "MDL: JSValue& JSValue::operator=(JSValue rhs): " };
		  const std::string message = "JSValues must belong to JSContexts within the same JSContextGroup to be shared and exchanged.";
		  std::clog << log_prefix << " [ERROR] " << message << std::endl;
		  throw std::runtime_error(message);
	  }
	  
    swap(*this, rhs);
    return *this;
  }
  
	friend void swap(JSValue& first, JSValue& second) noexcept {
		JAVASCRIPTCORECPP_RAII_JSVALUE_LOCK_GUARD;
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.js_context__  , second.js_context__);
    swap(first.js_value_ref__, second.js_value_ref__);
  }
  
 private:

	JSValue(const JSContext& js_context, const JSString& js_string, bool parse_as_json = false);

	// For interoperability with the JavaScriptCore C API.
	explicit JSValue(const JSContext& js_context, JSValueRef js_value_ref)
			: js_context__(js_context)
			, js_value_ref__(js_value_ref)  {
		assert(js_value_ref__);
		JSValueProtect(js_context__, js_value_ref__);
	}

  // For interoperability with the JavaScriptCore C API.
	// operator JSContextRef() const {
	//   return js_context__;
  // }

	// For interoperability with the JavaScriptCore C API.
	operator JSValueRef() const {
	  return js_value_ref__;
  }

	// Only a JSContext can create a JSValue.
	friend class JSContext;

	// JSUndefined needs access to the JSValue constructor.
	friend class JSUndefined;

	// JSNull needs access to the JSValue constructor.
	friend class JSNull;

	// JSBoolean needs access to operator JSValueRef().
	friend class JSBoolean;

	// JSNumber::operator double() needs access to operator JSValueRef()
	// to change its value and to the JSValue constructor for reporting
	// error messages.
	friend class JSNumber;

	// JSObject needs access to the JSValue constructor for
	// GetPrototype(), SetPrototype() and for generating error messages.
	friend class JSObject;
	
	// friend class detail::JSPropertyNameArray;

	// JSArray needs access to the JSValue constructor for generating
	// error messages.
	friend class JSArray;

	// JSDate needs access to the JSValue constructor for generating
	// error messages.
	friend class JSDate;
	
	// JSError needs access to the JSValue constructor for generating
	// error messages.
	friend class JSError;
	
	// JSRegExp needs access to the JSValue constructor for generating
	// error messages.
	friend class JSRegExp;

	// JSFunction needs access to the JSValue constructor for generating
	// error messages.
	friend class JSFunction;

  // The JSNativeClass static functions need access to operator
  // JSValueRef().
	template<typename T>
	friend class JSNativeClass;
	
	// This function requires access to operator JSValueRef().
	friend bool operator==(const JSValue& lhs, const JSValue& rhs);

	JSContext  js_context__;
	JSValueRef js_value_ref__ { nullptr };
	JAVASCRIPTCORECPP_RAII_JSVALUE_MUTEX;
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
	return JSValueIsStrictEqual(lhs.get_context(), lhs, rhs);
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
inline
bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs) {
	return lhs.IsEqualWithTypeCoercion(rhs);
}


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
