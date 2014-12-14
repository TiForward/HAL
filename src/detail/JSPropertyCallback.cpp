/**
 * HAL
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/detail/JSPropertyCallback.hpp"
#include "HAL/detail/JSUtil.hpp"

namespace HAL { namespace detail {
  
  JSPropertyCallback::JSPropertyCallback(const std::string& name, const std::unordered_set<JSPropertyAttribute>& attributes)
  : name__(name)
  , attributes__(attributes) {
    
    if (name__.empty()) {
      ThrowInvalidArgument("JSPropertyCallback", "property_name is missing");
    }
  }
  
  JSPropertyCallback::JSPropertyCallback(const JSPropertyCallback& rhs) HAL_NOEXCEPT
  : name__(rhs.name__)
  , attributes__(rhs.attributes__) {
  }
  
  JSPropertyCallback::JSPropertyCallback(JSPropertyCallback&& rhs) HAL_NOEXCEPT
  : name__(std::move(rhs.name__))
  , attributes__(std::move(rhs.attributes__)) {
  }
  
  JSPropertyCallback& JSPropertyCallback::operator=(const JSPropertyCallback& rhs) HAL_NOEXCEPT {
    HAL_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    name__       = rhs.name__;
    attributes__ = rhs.attributes__;
    return *this;
  }
  
  JSPropertyCallback& JSPropertyCallback::operator=(JSPropertyCallback&& rhs) HAL_NOEXCEPT {
    HAL_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    swap(rhs);
    return *this;
  }
  
  void JSPropertyCallback::swap(JSPropertyCallback& other) HAL_NOEXCEPT {
    HAL_DETAIL_JSPROPERTYCALLBACK_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(name__      , other.name__);
    swap(attributes__, other.attributes__);
  }
  
}} // namespace HAL { namespace detail {
