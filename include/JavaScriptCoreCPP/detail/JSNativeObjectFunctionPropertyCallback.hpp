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
#include "JavaScriptCoreCPP/detail/JSNativeObjectPropertyCallback.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP { namespace detail {

/*!
  @class

  @discussion A JSNativeObjectFunctionPropertyCallback is an RAII
  wrapper around the JavaScriptCore C API JSStaticFunction, which
  describes a function property of a JavaScript object.
*/
template<typename T>
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSNativeObjectFunctionPropertyCallback final : public JSNativeObjectPropertyCallback, public detail::JSPerformanceCounter<JSNativeObjectFunctionPropertyCallback<T>> {
#else
class JSNativeObjectFunctionPropertyCallback final : public JSNativeObjectPropertyCallback {
#endif
  
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
      JSValue CallAsFunction(const std::vector<JSValue>& arguments, JSObject& this_object);
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

  CallAsFunctionCallback<T> get_call_as_function_callback() const {
    return call_as_function_callback__;
  }
  
  ~JSNativeObjectFunctionPropertyCallback() {
  }
  
  JSNativeObjectFunctionPropertyCallback(const JSNativeObjectFunctionPropertyCallback& rhs)
		  : JSNativeObjectPropertyCallback(rhs)
		  , call_as_function_callback__(rhs.call_as_function_callback__) {
  }
  
  JSNativeObjectFunctionPropertyCallback(JSNativeObjectFunctionPropertyCallback&& rhs)
		  : JSNativeObjectPropertyCallback(rhs)
		  , call_as_function_callback__(std::move(rhs.call_as_function_callback__)) {
  }
  
  // Create a copy of another JSNativeObjectFunctionPropertyCallback
  // by assignment. This is a unified assignment operator that fuses
  // the copy assignment operator
  //
  // X& X::operator=(const X&)
  //
  // and the move assignment operator
  //
  // X& X::operator=(X&&);
  JSNativeObjectFunctionPropertyCallback& operator=(JSNativeObjectFunctionPropertyCallback rhs) {
    JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTPROPERTYCALLBACK_LOCK_GUARD;
    JSNativeObjectPropertyCallback::operator=(rhs);
    swap(rhs);
    return *this;
  }

  void swap(JSNativeObjectFunctionPropertyCallback& other) noexcept {
	  JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTVALUEPROPERTYCALLBACK_LOCK_GUARD;
	  using std::swap;
	  
	  // By swapping the members of two classes, the two classes are
	  // effectively swapped.
	  swap(call_as_function_callback__, other.call_as_function_callback__);
  }

  friend void swap(JSNativeObjectFunctionPropertyCallback& first, JSNativeObjectFunctionPropertyCallback& second) noexcept {
	  first.swap(second);
  }

 private:
  
  template<typename U>
  friend bool operator==(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

  template<typename U>
  friend bool operator<(const JSNativeObjectFunctionPropertyCallback<U>& lhs, const JSNativeObjectFunctionPropertyCallback<U>& rhs);

  CallAsFunctionCallback<T> call_as_function_callback__ { nullptr };
};

template<typename T>
JSNativeObjectFunctionPropertyCallback<T>::JSNativeObjectFunctionPropertyCallback(const JSString& function_name, CallAsFunctionCallback<T> call_as_function_callback, const std::unordered_set<JSPropertyAttribute>& attributes)
		: JSNativeObjectPropertyCallback(property_name, attributes)
		, call_as_function_callback__(call_as_function_callback) {

	if (!call_as_function_callback) {
		ThrowInvalidArgument("JSNativeObjectFunctionPropertyCallback", "call_as_function_callback is missing");
	}
}

// Return true if the two JSNativeObjectFunctionPropertyCallbacks are equal.
template<typename T>
bool operator==(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
  if (lhs.call_as_function_callback__ && !rhs.call_as_function_callback__) {
    return false;
  }
  
  if (!lhs.call_as_function_callback__ && rhs.call_as_function_callback__) {
    return false;
  }

  return static_cast<JSNativeObjectPropertyCallback>(lhs) == static_cast<JSNativeObjectPropertyCallback>(rhs);
}

// Return true if the two JSString are not equal.
template<typename T>
bool operator!=(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
  return ! (lhs == rhs);
}

// Define a strict weak ordering for two JSNativeObjectFunctionPropertyCallbacks.
template<typename T>
bool operator<(const JSNativeObjectFunctionPropertyCallback<T>& lhs, const JSNativeObjectFunctionPropertyCallback<T>& rhs) {
	return static_cast<JSNativeObjectPropertyCallback>(lhs) < static_cast<JSNativeObjectPropertyCallback>(rhs);
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

#endif // _JAVASCRIPTCORECPP_DETAIL_JSNATIVEOBJECTFUNCTIONPROPERTYCALLBACK_HPP_
