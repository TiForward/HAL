/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSClass.hpp"
#include "HAL/detail/JSStaticValue.hpp"
#include "HAL/detail/JSStaticFunction.hpp"

#include <string>
#include <algorithm>

namespace HAL {
  
  JSClass::JSClass() HAL_NOEXCEPT
  : name__("Empty")
  , js_class_ref__(JSClassCreate(&kJSClassDefinitionEmpty)) {
    HAL_LOG_TRACE("JSClass:: ctor ", this);
    HAL_LOG_TRACE("JSClass:: retain ", js_class_ref__, " (implicit) for ", this);
  }
  
  JSClass::JSClass(const JSClassDefinition& js_class_definition) HAL_NOEXCEPT
  : name__(js_class_definition.name__)
  , js_class_ref__(JSClassCreate(&js_class_definition.js_class_definition__)) {
    HAL_LOG_TRACE("JSClass:: ctor ", this);
    HAL_LOG_TRACE("JSClass:: retain ", js_class_ref__, " for ", this);
  }
  
  JSClass::~JSClass() HAL_NOEXCEPT {
    HAL_LOG_TRACE("JSClass:: dtor ", this);
    HAL_LOG_TRACE("JSClass:: release ", js_class_ref__, " for ", this);
    JSClassRelease(js_class_ref__);
  }
  
  JSClass::JSClass(const JSClass& rhs) HAL_NOEXCEPT
  : name__(rhs.name__)
  , js_class_ref__(rhs.js_class_ref__) {
    HAL_LOG_TRACE("JSClass:: copy ctor ", this);
    HAL_LOG_TRACE("JSClass:: retain ", js_class_ref__, " for ", this);
    JSClassRetain(js_class_ref__);
  }
  
  JSClass::JSClass(JSClass&& rhs) HAL_NOEXCEPT
  : name__(std::move(rhs.name__))
  , js_class_ref__(rhs.js_class_ref__) {
    HAL_LOG_TRACE("JSClass:: move ctor ", this);
    HAL_LOG_TRACE("JSClass:: retain ", js_class_ref__, " for ", this);
    JSClassRetain(js_class_ref__);
  }
  
  JSClass& JSClass::operator=(JSClass rhs) HAL_NOEXCEPT {
    HAL_JSCLASS_LOCK_GUARD;
    HAL_LOG_TRACE("JSClass:: assignment ", this);
    swap(rhs);
    return *this;
  }
  
  void JSClass::swap(JSClass& other) HAL_NOEXCEPT {
    HAL_JSCLASS_LOCK_GUARD;
    HAL_LOG_TRACE("JSClass:: swap ", this);
    using std::swap;
    swap(name__        , other.name__);
    swap(js_class_ref__, other.js_class_ref__);
  }
  
} // namespace HAL {
