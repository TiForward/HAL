/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/detail/JSPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

namespace JavaScriptCoreCPP { namespace detail {
  
  JSPropertyCallback::JSPropertyCallback(const std::string& name, const std::unordered_set<JSPropertyAttribute>& attributes)
  : name__(name)
  , attributes__(attributes) {
    
    if (name__.empty()) {
      ThrowInvalidArgument("JSPropertyCallback", "property_name is missing");
    }
  }
  
  JSPropertyCallback::JSPropertyCallback(const JSPropertyCallback& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : name__(rhs.name__)
  , attributes__(rhs.attributes__) {
  }
  
  JSPropertyCallback::JSPropertyCallback(JSPropertyCallback&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : name__(std::move(rhs.name__))
  , attributes__(std::move(rhs.attributes__)) {
  }
  
  JSPropertyCallback& JSPropertyCallback::operator=(const JSPropertyCallback& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    name__       = rhs.name__;
    attributes__ = rhs.attributes__;
    return *this;
  }
  
  JSPropertyCallback& JSPropertyCallback::operator=(JSPropertyCallback&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    swap(rhs);
    return *this;
  }
  
  void JSPropertyCallback::swap(JSPropertyCallback& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(name__      , other.name__);
    swap(attributes__, other.attributes__);
  }
  
}} // namespace JavaScriptCoreCPP { namespace detail {
