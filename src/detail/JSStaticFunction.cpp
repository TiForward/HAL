/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSStaticFunction.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <string>

namespace JavaScriptCoreCPP { namespace detail {
  
  JSStaticFunction::JSStaticFunction(const ::JSStaticFunction& js_static_function)
  : JSPropertyCallback(js_static_function.name, FromJSPropertyAttributes(js_static_function.attributes))
  , function_callback__(js_static_function.callAsFunction) {
    
    if (!function_callback__) {
      ThrowInvalidArgument("JSStaticFunction", "function_callback is missing");
    }
  }
  
  JSStaticFunction::JSStaticFunction(const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSPropertyCallback(rhs)
  , function_callback__(rhs.function_callback__) {
  }
  
  
  JSStaticFunction::JSStaticFunction(JSStaticFunction&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSPropertyCallback(rhs)
  , function_callback__(std::move(rhs.function_callback__)) {
  }
  
  
  JSStaticFunction& JSStaticFunction::operator=(const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    JSPropertyCallback::operator=(rhs);
    function_callback__ = rhs.function_callback__;
    return *this;
  }
  
  
  JSStaticFunction& JSStaticFunction::operator=(JSStaticFunction&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    JSPropertyCallback::operator=(rhs);
    function_callback__ = std::move(rhs.function_callback__);
    return *this;
  }
  
  
  void JSStaticFunction::swap(JSStaticFunction& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(function_callback__, other.function_callback__);
  }
  
  
  // Return true if the two JSStaticFunctions are
  // equal.
  
  bool operator==(const JSStaticFunction& lhs, const JSStaticFunction& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    if (lhs.function_callback__ && !rhs.function_callback__) {
      return false;
    }
    
    if (!lhs.function_callback__ && rhs.function_callback__) {
      return false;
    }
    
    return static_cast<JSPropertyCallback>(lhs) == static_cast<JSPropertyCallback>(rhs);
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {
