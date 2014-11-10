/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSStaticValue.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <string>

namespace JavaScriptCoreCPP { namespace detail {
  
  JSStaticValue::JSStaticValue(const ::JSStaticValue& js_static_value)
  : JSPropertyCallback(js_static_value.name, FromJSPropertyAttributes(js_static_value.attributes))
  , get_callback__(js_static_value.getProperty)
  , set_callback__(js_static_value.setProperty) {
    
    if (!get_callback__ && !set_callback__) {
      ThrowInvalidArgument("JSStaticValue", "Both get_callback and set_callback are missing. At least one callback must be provided");
    }
    
    if (attributes__.find(JSPropertyAttribute::ReadOnly) != attributes__.end()) {
      if (!get_callback__) {
        ThrowInvalidArgument("JSStaticValue", "ReadOnly attribute is set but get_callback is missing");
      }
      
      if (set_callback__) {
        ThrowInvalidArgument("JSStaticValue", "ReadOnly attribute is set but set_callback is provided");
      }
    }
    
    // Force the ReadOnly attribute if only the get_callback is
    // provided.
    if (get_callback__ && !set_callback__) {
      attributes__.insert(JSPropertyAttribute::ReadOnly);
    }
  }
  
  JSStaticValue::JSStaticValue(const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSPropertyCallback(rhs)
  , get_callback__(rhs.get_callback__)
  , set_callback__(rhs.set_callback__) {
  }
  
  
  JSStaticValue::JSStaticValue(JSStaticValue&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSPropertyCallback(rhs)
  , get_callback__(std::move(rhs.get_callback__))
  , set_callback__(std::move(rhs.set_callback__)) {
  }
  
  
  JSStaticValue& JSStaticValue::operator=(const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    JSPropertyCallback::operator=(rhs);
    get_callback__ = rhs.get_callback__;
    set_callback__ = rhs.set_callback__;
    return *this;
  }
  
  JSStaticValue& JSStaticValue::operator=(JSStaticValue&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    JSPropertyCallback::operator=(rhs);
    get_callback__ = std::move(rhs.get_callback__);
    set_callback__ = std::move(rhs.set_callback__);
    return *this;
  }
  
  
  void JSStaticValue::swap(JSStaticValue& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(get_callback__, other.get_callback__);
    swap(set_callback__, other.set_callback__);
  }
  
  
  bool operator==(const JSStaticValue& lhs, const JSStaticValue& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    // get_callback__
    if (lhs.get_callback__ && !rhs.get_callback__) {
      return false;
    }
    
    if (!lhs.get_callback__ && rhs.get_callback__) {
      return false;
    }
    
    // set_callback__
    if (lhs.set_callback__ && !rhs.set_callback__) {
      return false;
    }
    
    if (!lhs.set_callback__ && rhs.set_callback__) {
      return false;
    }
    
    return static_cast<JSPropertyCallback>(lhs) == static_cast<JSPropertyCallback>(rhs);
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {
