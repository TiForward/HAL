/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSNativeObjectCallbacks.hpp"
#include "JavaScriptCoreCPP/JSPropertyAttribute.hpp"
#include "JavaScriptCoreCPP/detail/HashUtilities.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"
#include <functional>
#include <sstream>

namespace JavaScriptCoreCPP {

template<typename T>
class JSNativeObject;

}} // namespace JavaScriptCoreCPP {


namespace JavaScriptCoreCPP { namespace detail {

using namespace JavaScriptCoreCPP;

/*!
  @class

  @discussion A JSNativeObjectFunctionPropertyCallback is an RAII
  wrapper around the JavaScriptCore C API JSStaticFunction, which
  describes a function property of a JavaScript object.
*/
template<typename T>
class JSNativeObjectFunctionPropertyCallback final	{
	
 public:

	/*!
	  @method
	  
	  @abstract Create a callback to invoke when a JavaScript object is
	  called as a function either directly or when it is a property of
	  another JavaScript object.
	  
	  @discussion If this callback does not exist, then calling your
	  object as a function will throw an exception.
	  
	  For example, given this class definition:
	  
	  class Foo {
	    JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object);
	  };
	  
	  You would define the callback like this:
	  
	  CallAsFunctionCallback callback(&Foo::CallAsFunction);
	  
	  @param function_name A JSString containing the function's name.
	  
	  @param call_as_function_callback The callback to invoke when
	  calling the JavaScript object as a function.
	  
	  @param attributes The set of JSPropertyAttributes to give to the
	  function property.
	  
	  @result The callback to invoke when a JavaScript object is called
	  as a function either directly or when it is a property of another
	  JavaScript object.

	  @throws std::invalid_argument exception under these preconditions:

	  1. If function_name is empty or otherwise has a JavaScript syntax
	  error.
	  
	  2. If the call_as_function_callback is not provided.
	*/
	JSNativeObjectFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, const std::unordered_set<JSPropertyAttribute>& attributes);

	JSString get_function_name() const {
		return function_name__;
	}
	
	CallAsFunctionCallback<T> get_call_as_function_callback() const {
		return call_as_function_callback__;
	}
	
	std::unordered_set<JSPropertyAttribute> get_attributes() const {
		return attributes__;
	}

	std::size_t get_hash_value() const {
		return hash_value__;
	}
	
	~JSNativeObjectFunctionPropertyCallback() {
	}
	
	// Copy constructor.
	JSNativeObjectFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback& rhs)
			: function_name__(rhs.function_name__)
			, call_as_function_callback__(rhs.call_as_function_callback__)
			, attributes__(rhs.attributes__)
			, hash_value__(hash_val(function_name__, ToJSPropertyAttributes(attributes__))) {
	}
	
	// Move constructor.
	JSNativeObjectFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback&& rhs)
			: function_name__(rhs.function_name__)
			, call_as_function_callback__(rhs.call_as_function_callback__)
			, attributes__(rhs.attributes__)
			, hash_value__(hash_val(function_name__, ToJSPropertyAttributes(attributes__))) {
	}
	
	// Create a copy of another JSNativeObjectFunctionPropertyCallback by assignment. This is
	// a unified assignment operator that fuses the copy assignment
	// operator,
	// X& X::operator=(const X&), and the move assignment operator,
	// X& X::operator=(X&&);
	JSNativeObjectFunctionPropertyCallback& operator=(JSNativeObjectFunctionPropertyCallback rhs) {
		swap(*this, rhs);
		return *this;
	}
  
	friend void swap(JSNativeObjectFunctionPropertyCallback& first, JSNativeObjectFunctionPropertyCallback& second) noexcept {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;
		
		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.function_name__            , second.function_name__);
		swap(first.call_as_function_callback__, second.call_as_function_callback__);
		swap(first.attributes__               , second.attributes__);
    swap(first.hash_value__               , second.hash_value__);
	}

 private:
	
	template<typename U>
	friend bool operator==(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

	template<typename U>
	friend bool operator<(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

	JSString                                function_name__;
	CallAsFunctionCallback<T>               call_as_function_callback__ { nullptr };
	std::unordered_set<JSPropertyAttribute> attributes__;

	// Precomputed hash value for JSNativeObjectValuePropertyCallback
	// since instances of this class template are immutable.
	std::size_t                             hash_value__;
};

template<typename T>
JSNativeObjectFunctionPropertyCallback<T>::JSNativeObjectFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, const std::unordered_set<JSPropertyAttribute>& attributes)
		: function_name__(function_name)
		, call_as_function_callback__(call_as_function_callback)
		, attributes__(attributes)
		, hash_value__(hash_val(function_name__, ToJSPropertyAttributes(attributes__))) {
	
	static const std::string log_prefix { "MDL: JSNativeObjectFunctionPropertyCallback: " };
	
	if (function_name__.empty()) {
		std::ostringstream os;
		os << "The function_name is empty. A valid JavaScript property name must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	if (!call_as_function_callback) {
		std::ostringstream os;
		os << "The call_as_function_callback must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
}

// Return true if the two JSNativeObjectFunctionPropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	if (lhs.function_name__ != rhs.function_name__) {
		return false;
	}

	if (lhs.call_as_function_callback__ && !rhs.call_as_function_callback__) {
		return false;
	}
	
	if (!lhs.call_as_function_callback__ && rhs.call_as_function_callback__) {
		return false;
	}

	return lhs.attributes__ == rhs.attributes__;
}

// Return true if the two JSString are not equal.
template<typename T>
bool operator!=(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSNativeObjectFunctionPropertyCallbacks.
template<typename T>
bool operator<(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	if (lhs.function_name__ < rhs.function_name__) {
		return true;
	}
	
	return lhs.attributes__ < rhs.attributes__;
}

template<typename T>
bool operator>(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return rhs < lhs;
}

template<typename T>
bool operator<=(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return ! (lhs > rhs);
}

template<typename T>
bool operator>=(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return ! (lhs < rhs);
}

template<typename T>
struct hash<JSNativeObjectFunctionPropertyCallback<T>> {
	using argument_type = JSNativeObjectFunctionPropertyCallback<T>;
	using result_type   = std::size_t;
	
	result_type operator()(const argument_type& callback) const {
		return callback.get_hash_value();
	}
};

template<typename T>
using JSNativeObjectFunctionPropertyCallbackHash = hash<JSNativeObjectFunctionPropertyCallback<T>>;

}} // namespace JavaScriptCoreCPP { namespace detail {

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_
