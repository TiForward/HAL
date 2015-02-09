/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSExportObject.hpp"
#include "HAL/JSUndefined.hpp"
#include <utility>

namespace HAL {
  
  void JSExportObject::JSExportInitialize() {
    HAL_LOG_DEBUG("JSExportObject::JSExportInitialize");
    SetClassVersion(1);
  }
  
  JSContext JSExportObject::get_context() const HAL_NOEXCEPT {
    return js_context__;
  }
  
  JSObject JSExportObject::get_object() HAL_NOEXCEPT {
    return JSObject::FindJSObjectFromPrivateData(get_context(), this);
  }
  
  JSExportObject::JSExportObject(const JSContext& js_context, const std::vector<JSValue>& arguments) HAL_NOEXCEPT
  : js_context__(js_context) {
    HAL_LOG_DEBUG("JSExportObject:: ctor ", this);
  }
  
  JSExportObject::~JSExportObject() HAL_NOEXCEPT {
    HAL_LOG_DEBUG("JSExportObject:: dtor ", this);
    JSObject::UnRegisterPrivateData(this);
  }
  
  void JSExportObject::swap(JSExportObject& other) HAL_NOEXCEPT {
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context__  , other.js_context__);
  }
  
} // namespace HAL {
