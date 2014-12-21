/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSContextGroup.hpp"
#include "HAL/JSContext.hpp"
#include "HAL/JSClass.hpp"

#include <cassert>

namespace HAL {
  
  JSContextGroup::JSContextGroup() HAL_NOEXCEPT
  : js_context_group_ref__(JSContextGroupCreate()) {
    HAL_LOG_TRACE("JSContextGroup:: ctor 1 ", this);
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " (implicit) for ", this);
  }
  
  JSContext JSContextGroup::CreateContext() const HAL_NOEXCEPT {
    return JSContext(*this, JSClass());
  }
  
  JSContext JSContextGroup::CreateContext(const JSClass& global_object_class) const HAL_NOEXCEPT {
    return JSContext(*this, global_object_class);
  }
  
  JSContextGroup::JSContextGroup(JSContextGroupRef js_context_group_ref) HAL_NOEXCEPT
  : js_context_group_ref__(js_context_group_ref) {
    HAL_LOG_TRACE("JSContextGroup:: ctor 2 ", this);
    assert(js_context_group_ref__);
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " for ", this);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup::~JSContextGroup() HAL_NOEXCEPT {
    HAL_LOG_TRACE("JSContextGroup:: dtor ", this);
    HAL_LOG_TRACE("JSContextGroup:: release ", js_context_group_ref__, " for ", this);
    JSContextGroupRelease(js_context_group_ref__);
  }
  
  JSContextGroup::JSContextGroup(const JSContextGroup& rhs) HAL_NOEXCEPT
  : js_context_group_ref__(rhs.js_context_group_ref__) {
    HAL_LOG_TRACE("JSContextGroup:: copy ctor ", this);
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " for ", this);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup::JSContextGroup(JSContextGroup&& rhs) HAL_NOEXCEPT
  : js_context_group_ref__(rhs.js_context_group_ref__) {
    HAL_LOG_TRACE("JSContextGroup:: move ctor ", this);
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " for ", this);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup& JSContextGroup::operator=(const JSContextGroup& rhs) HAL_NOEXCEPT {
    HAL_JSCONTEXTGROUP_LOCK_GUARD;
    HAL_LOG_TRACE("JSContextGroup:: copy assignment ", this);
    JSContextGroupRelease(js_context_group_ref__);
    HAL_LOG_TRACE("JSContextGroup:: release ", js_context_group_ref__, " for ", this);
    js_context_group_ref__ = rhs.js_context_group_ref__;
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " for ", this);
    JSContextGroupRetain(js_context_group_ref__);
    return *this;
  }
  
  JSContextGroup& JSContextGroup::operator=(JSContextGroup&& rhs) HAL_NOEXCEPT {
    HAL_JSCONTEXTGROUP_LOCK_GUARD;
    HAL_LOG_TRACE("JSContextGroup:: move assignment ", this);
    HAL_LOG_TRACE("JSContextGroup:: release ", js_context_group_ref__, " for ", this);
    JSContextGroupRelease(js_context_group_ref__);
    js_context_group_ref__ = rhs.js_context_group_ref__;
    HAL_LOG_TRACE("JSContextGroup:: retain ", js_context_group_ref__, " for ", this);
    JSContextGroupRetain(js_context_group_ref__);
    return *this;
  }
  
  void JSContextGroup::swap(JSContextGroup& other) HAL_NOEXCEPT {
    HAL_JSCONTEXTGROUP_LOCK_GUARD;
    HAL_LOG_TRACE("JSContextGroup:: swap ", this);
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context_group_ref__, other.js_context_group_ref__);
  }
  
} // namespace HAL {
