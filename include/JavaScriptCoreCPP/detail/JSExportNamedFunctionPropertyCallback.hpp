/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_DETAIL_JSEXPORTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_DETAIL_JSEXPORTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/detail/JSExportPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSExportCallbacks.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP { namespace detail

/*!
  @class

  @discussion A JSExportNamedFunctionPropertyCallback is an RAII
  wrapper around the JavaScriptCore C API JSStaticFunction, which
  describes a function property of a JavaScript object.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
template<typename T>
class JSExportNamedFunctionPropertyCallback final : public JSExportPropertyCallback, public detail::JSPerformanceCounter<JSExportNamedFunctionPropertyCallback<T>> {
#else
class JSExportNamedFunctionPropertyCallback final : public JSExportPropertyCallback {
#endif
  
 public:

  /*!
    @method
    
    @abstract Create a callback to invoke when a JavaScript object is
    called as a function.
    
    @discussion If this callback does not exist, then calling your
    object as a function will throw an exception.
    
    For example, given this class definition:
    
    class Foo {
      JSValue CallAsFunction(const std::vector<JSValue>& arguments, JSObject& this_object);
    };
    
    You would define the callback like this:
    
    CallNamedFunctionCallback callback(&Foo::CallAsFunction);
    
    @param function_name A JSString containing the function's name.
    
    @param function_callback The callback to invoke when calling the
    JavaScript object as a function.
    
    @param attributes The set of JSPropertyAttributes to give to the
    function property.
    
    @result The callback to invoke when a JavaScript object is called
    as a function.

    @throws std::invalid_argument exception under these preconditions:

    1. If function_name is empty.
    
    2. If the function_callback is not provided.
  */
	JSExportNamedFunctionPropertyCallback(const JSString& function_name,
	                                      CallNamedFunctionCallback function_callback,
	                                      const std::unordered_set<JSPropertyAttribute>& attributes);

	CallNamedFunctionCallback function_callback() const {
    return function_callback__;
  }
  
  ~JSExportNamedFunctionPropertyCallback();
  JSExportNamedFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback& rhs);
  JSExportNamedFunctionPropertyCallback(JSExportNamedFunctionPropertyCallback&& rhs);
  JSExportNamedFunctionPropertyCallback& JSExportNamedFunctionPropertyCallback::operator=(const JSExportNamedFunctionPropertyCallback&) = delete;
	JSExportNamedFunctionPropertyCallback& JSExportNamedFunctionPropertyCallback::operator=(JSExportNamedFunctionPropertyCallback&&) = delete;
  JSExportNamedFunctionPropertyCallback& operator=(JSExportNamedFunctionPropertyCallback rhs);
  void swap(JSExportNamedFunctionPropertyCallback& other) noexcept;

 private:
  
	template<typename U>
  friend bool operator==(const JSExportNamedFunctionPropertyCallback<U>& lhs, const JSExportNamedFunctionPropertyCallback<U>& rhs);

	template<typename U>
  friend bool operator<(const JSExportNamedFunctionPropertyCallback<U>& lhs, const JSExportNamedFunctionPropertyCallback<U>& rhs);

  CallNamedFunctionCallback function_callback__ { nullptr };
};

template<typename T>
JSExportNamedFunctionPropertyCallback<T>::JSExportNamedFunctionPropertyCallback(
    const JSString& function_name,
    CallNamedFunctionCallback function_callback,
    const std::unordered_set<JSPropertyAttribute>& attributes)
		: JSExportPropertyCallback(property_name, attributes)
		, function_callback__(function_callback) {
	
	if (!function_callback) {
		ThrowInvalidArgument("JSExportNamedFunctionPropertyCallback", "function_callback is missing");
	}
}

template<typename T>
JSExportNamedFunctionPropertyCallback<T>::~JSExportNamedFunctionPropertyCallback() {
}

template<typename T>
JSExportNamedFunctionPropertyCallback<T>::JSExportNamedFunctionPropertyCallback(const JSExportNamedFunctionPropertyCallback& rhs)
		: JSExportPropertyCallback(rhs)
		, function_callback__(rhs.function_callback__) {
}

template<typename T>
JSExportNamedFunctionPropertyCallback<T>::JSExportNamedFunctionPropertyCallback(JSExportNamedFunctionPropertyCallback&& rhs)
		: JSExportPropertyCallback(rhs)
		, function_callback__(std::move(rhs.function_callback__)) {
}

// Create a copy of another JSExportNamedFunctionPropertyCallback by
// assignment. This is a unified assignment operator that fuses the
// copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
template<typename T>
JSExportNamedFunctionPropertyCallback& JSExportNamedFunctionPropertyCallback<T>::operator=(JSExportNamedFunctionPropertyCallback rhs) {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD;
	JSExportPropertyCallback::operator=(rhs);
	swap(rhs);
	return *this;
}

template<typename T>
void JSExportNamedFunctionPropertyCallback<T>::swap(JSExportNamedFunctionPropertyCallback& other) noexcept {
	JAVASCRIPTCORECPP_DETAIL_JSEXPORTPROPERTYCALLBACK_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(function_callback__, other.function_callback__);
}

template<typename T>
void swap(JSExportNamedFunctionPropertyCallback<T>& first, JSExportNamedFunctionPropertyCallback<T>& second) noexcept {
	first.swap(second);
}

// Return true if the two JSExportNamedFunctionPropertyCallbacks are
// equal.
template<typename T>
bool operator==(const JSExportNamedFunctionPropertyCallback& lhs, const JSExportNamedFunctionPropertyCallback& rhs) {
	if (lhs.function_callback__ && !rhs.function_callback__) {
		return false;
	}
	
	if (!lhs.function_callback__ && rhs.function_callback__) {
		return false;
	}
	
	return static_cast<JSExportPropertyCallback>(lhs) == static_cast<JSExportPropertyCallback>(rhs);
}

// Return true if the two JSExportNamedFunctionPropertyCallbacks are
// not equal.
template<typename T>
bool operator!=(const JSExportNamedFunctionPropertyCallback<T>& lhs, const JSExportNamedFunctionPropertyCallback<T>& rhs) {
  return ! (lhs == rhs);
}

// Define a strict weak ordering for two
// JSExportNamedFunctionPropertyCallbacks.
template<typename T>
bool operator<(const JSExportNamedFunctionPropertyCallback<T>& lhs, const JSExportNamedFunctionPropertyCallback<T>& rhs) {
	return static_cast<JSExportPropertyCallback>(lhs) < static_cast<JSExportPropertyCallback>(rhs);
}

template<typename T>
bool operator>(const JSExportNamedFunctionPropertyCallback<T>& lhs, const JSExportNamedFunctionPropertyCallback<T>& rhs) {
  return rhs < lhs;
}

template<typename T>
bool operator<=(const JSExportNamedFunctionPropertyCallback<T>& lhs, const JSExportNamedFunctionPropertyCallback<T>& rhs) {
  return ! (lhs > rhs);
}

template<typename T>
bool operator>=(const JSExportNamedFunctionPropertyCallback<T>& lhs, const JSExportNamedFunctionPropertyCallback<T>& rhs) {
  return ! (lhs < rhs);
}

}} // namespace JavaScriptCoreCPP { namespace detail

#endif // _JAVASCRIPTCORECPP_DETAIL_JSEXPORTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_
