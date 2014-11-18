/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"
#include "JavaScriptCoreCPP/JSClass.hpp"

#include <cassert>

namespace JavaScriptCoreCPP {
  
  JSContextGroup::JSContextGroup() JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group_ref__(JSContextGroupCreate()) {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: ctor");
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
  }
  
  JSContext JSContextGroup::CreateContext() const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSContext(*this, JSClass());
  }
  
  JSContext JSContextGroup::CreateContext(const JSClass& global_object_class) const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSContext(*this, global_object_class);
  }
  
  JSContextGroup::JSContextGroup(JSContextGroupRef js_context_group_ref) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group_ref__(js_context_group_ref) {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: ctor");
    assert(js_context_group_ref__);
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup::~JSContextGroup() JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: dtor");
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: release ", js_context_group_ref__);
    JSContextGroupRelease(js_context_group_ref__);
  }
  
  JSContextGroup::JSContextGroup(const JSContextGroup& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group_ref__(rhs.js_context_group_ref__) {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: copy ctor");
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup::JSContextGroup(JSContextGroup&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group_ref__(rhs.js_context_group_ref__) {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: move ctor");
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
    JSContextGroupRetain(js_context_group_ref__);
  }
  
  JSContextGroup& JSContextGroup::operator=(const JSContextGroup& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXTGROUP_LOCK_GUARD;
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: copy assignment");
    JSContextGroupRelease(js_context_group_ref__);
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: release ", js_context_group_ref__);
    js_context_group_ref__ = rhs.js_context_group_ref__;
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
    JSContextGroupRetain(js_context_group_ref__);
    return *this;
  }
  
  JSContextGroup& JSContextGroup::operator=(JSContextGroup&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXTGROUP_LOCK_GUARD;
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: move assignment");
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: release ", js_context_group_ref__);
    JSContextGroupRelease(js_context_group_ref__);
    js_context_group_ref__ = rhs.js_context_group_ref__;
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: retain ", js_context_group_ref__);
    JSContextGroupRetain(js_context_group_ref__);
    return *this;
  }
  
  void JSContextGroup::swap(JSContextGroup& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXTGROUP_LOCK_GUARD;
    JAVASCRIPTCORECPP_LOG_DEBUG("JSContextGroup:: swap");
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context_group_ref__, other.js_context_group_ref__);
  }
  
} // namespace JavaScriptCoreCPP {
