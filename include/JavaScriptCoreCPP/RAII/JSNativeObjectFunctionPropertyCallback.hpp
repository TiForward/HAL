/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/RAII/JSNativeObjectCallbacks.hpp"
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {

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
	    JSValue CallAsFunction(const std::vector<JSValue>& arguments);
	    JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object);
	  };
	  
	  You would define the two callbacks like this:
	  
	  CallAsFunctionCallback callback(&Foo::CallAsFunction);
	  CallAsFunctionWithThisCallback callback(&Foo::CallAsFunction);
	  
	  @param function_name A JSString containing the function's name. If
	  function_name is empty then the callback refers to an anonymous
	  function.
	  
	  @param call_as_function_callback The callback to invoke when
	  calling the JavaScript object as a function.
	  
	  @param call_as_function_with_this_callback The callback to invoke
	  when calling the JavaScript object as a function when it is a
	  property of another JavaScript object.

	  @param attributes A set of JSPropertyAttributes to give to the
	  function property.
	  
	  @result The callback to invoke when a JavaScript object is called
	  as a function either directly or when it is a property of another
	  JavaScript object.
	*/
	JSNativeObjectFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, CallAsFunctionWithThisCallback<T> call_as_function_with_this_callback, const std::unordered_set<JSPropertyAttribute> attributes = std::unordered_set<JSPropertyAttribute>());
	

	JSString get_function_name() const {
		return function_name_;
	}
	
	CallAsFunctionCallback<T> get_call_as_function_callback() const {
		return call_as_function_callback_;
	}
	
	CallAsFunctionWithThisCallback<T> get_call_as_function_with_this_callback() const {
		return call_as_function_with_this_callback_;
	}

	std::unordered_set<JSPropertyAttribute> get_attributes() const {
		return attributes_;
	}

	~JSNativeObjectFunctionPropertyCallback() {
	}
	
	// Copy constructor.
	JSNativeObjectFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback& rhs);
	
	// Move constructor.
	JSNativeObjectFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback&& rhs);
	
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
		swap(first.function_name_                       , second.function_name_);
		swap(first.function_name_for_js_static_function_, second.function_name_for_js_static_function_);
		swap(first.call_as_function_callback_           , second.call_as_function_callback_);
		swap(first.call_as_function_with_this_callback_ , second.call_as_function_with_this_callback_);
		swap(first.attributes_                          , second.attributes_);
	}

 private:
	
	// Return true if the two JSNativeObjectFunctionPropertyCallbacks are
	// equal.
	template<typename U>
	friend bool operator==(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

	// Define a strict weak ordering for two
	// JSNativeObjectFunctionPropertyCallbacks.
	template<typename U>
	friend bool operator<(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

	JSString                                function_name_;
	std::string                             function_name_for_js_static_function_;
	CallAsFunctionCallback<T>               call_as_function_callback_           { nullptr };
	CallAsFunctionWithThisCallback<T>       call_as_function_with_this_callback_ { nullptr };
	std::unordered_set<JSPropertyAttribute> attributes_;
};

template<typename T>
JSNativeObjectFunctionPropertyCallback<T>::JSNativeObjectFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, CallAsFunctionWithThisCallback<T> call_as_function_with_this_callback, const std::unordered_set<JSPropertyAttribute> attributes)
		: function_name_(function_name)
		, function_name_for_js_static_function_(function_name)
		, call_as_function_callback_(call_as_function_callback)
		, call_as_function_with_this_callback_(call_as_function_with_this_callback)
		, attributes_(attributes) {
	
	static const std::string log_prefix { "MDL: JSNativeObjectFunctionPropertyCallback: " };
	
	if (function_name_for_js_static_function_.empty()) {
		std::ostringstream os;
		os << "The function_name is empty. A valid JavaScript property name must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	if (!call_as_function_callback || !call_as_function_with_this_callback) {
		std::ostringstream os;
		os << "Both call_as_function_callback and call_as_function_with_this_callback must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}

	using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
	std::bitset<4> property_attributes;
	for (auto property_attribute : attributes) {
		const auto bit_position = static_cast<property_attribute_underlying_type>(property_attribute);
		property_attributes.set(bit_position);
	}
}

// Copy constructor.
template<typename T>
JSNativeObjectFunctionPropertyCallback<T>::JSNativeObjectFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback<T>& rhs)
		: function_name_(rhs.function_name_)
		, function_name_for_js_static_function_(rhs.function_name_for_js_static_function_)
		, call_as_function_callback_(rhs.call_as_function_callback_)
		, call_as_function_with_this_callback_(rhs.call_as_function_with_this_callback_)
		, attributes_(rhs.attributes_) {
}

// Move constructor.
template<typename T>
JSNativeObjectFunctionPropertyCallback<T>::JSNativeObjectFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback<T>&& rhs)
		: function_name_(rhs.function_name_)
		, function_name_for_js_static_function_(rhs.function_name_for_js_static_function_)
		, call_as_function_callback_(rhs.call_as_function_callback_)
		, call_as_function_with_this_callback_(rhs.call_as_function_with_this_callback_)
		, attributes_(rhs.attributes_) {
}

// Return true if the two JSNativeObjectFunctionPropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	if (lhs.function_name_ != rhs.function_name_) {
		return false;
	}

	if (lhs.call_as_function_callback_ && !rhs.call_as_function_callback_) {
		return false;
	}
	
	if (!lhs.call_as_function_callback_ && rhs.call_as_function_callback_) {
		return false;
	}

	if (lhs.call_as_function_with_this_callback_ && !rhs.call_as_function_with_this_callback_) {
		return false;
	}
	
	if (!lhs.call_as_function_with_this_callback_ && rhs.call_as_function_with_this_callback_) {
		return false;
	}
	
	return lhs.attributes_ == rhs.attributes_;
}

// Return true if the two JSString are not equal.
template<typename T>
bool operator!=(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSNativeObjectFunctionPropertyCallbacks.
template<typename T>
bool operator<(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	if (lhs.function_name_for_js_static_function_ < rhs.function_name_for_js_static_function_) {
		return true;
	}
	
	return lhs.attributes_ < rhs.attributes_;
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

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _JAVASCRIPTCORECPP_RAII_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_
