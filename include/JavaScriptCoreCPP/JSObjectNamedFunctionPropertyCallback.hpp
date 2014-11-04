/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_JSOBJECTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_
#define _JAVASCRIPTCORECPP_JSOBJECTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_

#include "JavaScriptCoreCPP/JSObjectPropertyCallback.hpp"
#include "JavaScriptCoreCPP/JSObjectCallbacks.hpp"

#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
#include "JavaScriptCoreCPP/detail/JSPerformanceCounter.hpp"
#endif

namespace JavaScriptCoreCPP

/*!
  @class

  @discussion A JSObjectNamedFunctionPropertyCallback is an RAII
  wrapper around the JavaScriptCore C API JSStaticFunction, which
  describes a function property of a JavaScript object.
*/
#ifdef JAVASCRIPTCORECPP_PERFORMANCE_COUNTER_ENABLE
class JSObjectNamedFunctionPropertyCallback final : public JSObjectPropertyCallback, public detail::JSPerformanceCounter<JSObjectNamedFunctionPropertyCallback> {
#else
class JSObjectNamedFunctionPropertyCallback final : public JSObjectPropertyCallback {
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
    
    @param attributes The set of JSObjectPropertyAttributes to give to
    the function property.
    
    @result The callback to invoke when a JavaScript object is called
    as a function.

    @throws std::invalid_argument exception under these preconditions:

    1. If function_name is empty.
    
    2. If the function_callback is not provided.
  */
	JSObjectNamedFunctionPropertyCallback(const JSString& function_name,
	                                      CallNamedFunctionCallback function_callback,
	                                      const std::unordered_set<JSObjectPropertyAttribute>& attributes);

	CallNamedFunctionCallback function_callback() const {
    return function_callback__;
  }
  
  ~JSObjectNamedFunctionPropertyCallback();
  JSObjectNamedFunctionPropertyCallback(const JSObjectNamedFunctionPropertyCallback& rhs);
  JSObjectNamedFunctionPropertyCallback(JSObjectNamedFunctionPropertyCallback&& rhs);
  JSObjectNamedFunctionPropertyCallback& JSObjectNamedFunctionPropertyCallback::operator=(const JSObjectNamedFunctionPropertyCallback&) = delete;
	JSObjectNamedFunctionPropertyCallback& JSObjectNamedFunctionPropertyCallback::operator=(JSObjectNamedFunctionPropertyCallback&&) = delete;
  JSObjectNamedFunctionPropertyCallback& operator=(JSObjectNamedFunctionPropertyCallback rhs);
  void swap(JSObjectNamedFunctionPropertyCallback& other) noexcept;

 private:
  
  friend bool operator==(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs);
  friend bool operator<(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs);

  CallNamedFunctionCallback function_callback__ { nullptr };
};


// Return true if the two JSObjectNamedFunctionPropertyCallbacks are
// equal.
bool operator==(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs);

// Return true if the two JSObjectNamedFunctionPropertyCallbacks are
// not equal.
inline
bool operator!=(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
  return ! (lhs == rhs);
}

// Define a strict weak ordering for two
// JSObjectNamedFunctionPropertyCallbacks.
inline
bool operator<(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
	return static_cast<JSObjectPropertyCallback>(lhs) < static_cast<JSObjectPropertyCallback>(rhs);
}

inline
bool operator>(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
  return rhs < lhs;
}

inline
bool operator<=(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
  return ! (lhs > rhs);
}

inline
bool operator>=(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
  return ! (lhs < rhs);
}

} // namespace JavaScriptCoreCPP {

namespace std {
using JavaScriptCoreCPP::detail::JSObjectNamedFunctionPropertyCallback;
template<>
void swap<JSObjectNamedFunctionPropertyCallback>(JSObjectNamedFunctionPropertyCallback& first, JSObjectNamedFunctionPropertyCallback& second) noexcept {
	first.swap(second);
}
}  // namespace std

#endif // _JAVASCRIPTCORECPP_JSOBJECTNAMEDFUNCTIONPROPERTYCALLBACK_HPP_
