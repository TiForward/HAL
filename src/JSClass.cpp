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
//    HAL_LOG_DEBUG("JSClass:: ctor");
//    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
  }
  
  JSClass::JSClass(const JSClassDefinition& js_class_definition) HAL_NOEXCEPT
  : name__(js_class_definition.name__)
  , js_class_ref__(JSClassCreate(&js_class_definition.js_class_definition__)) {
    HAL_LOG_DEBUG("JSClass:: ctor");
    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
  }
  
  JSClass::~JSClass() HAL_NOEXCEPT {
//    HAL_LOG_DEBUG("JSClass:: dtor");
//    HAL_LOG_DEBUG("JSClass:: release ", js_class_ref__);
    JSClassRelease(js_class_ref__);
  }
  
  JSClass::JSClass(const JSClass& rhs) HAL_NOEXCEPT
  : name__(rhs.name__)
  , js_class_ref__(rhs.js_class_ref__) {
    HAL_LOG_DEBUG("JSClass:: copy ctor");
    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
    JSClassRetain(js_class_ref__);
  }
  
  JSClass::JSClass(JSClass&& rhs) HAL_NOEXCEPT
  : name__(std::move(rhs.name__))
  , js_class_ref__(rhs.js_class_ref__) {
    HAL_LOG_DEBUG("JSClass:: move ctor");
    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
    JSClassRetain(js_class_ref__);
  }
  
  JSClass& JSClass::operator=(const JSClass& rhs) HAL_NOEXCEPT {
    HAL_JSCLASS_LOCK_GUARD;
    HAL_LOG_DEBUG("JSClass:: copy assignment");
    HAL_LOG_DEBUG("JSClass:: release ", js_class_ref__);
    JSClassRelease(js_class_ref__);
    name__         = rhs.name__;
    js_class_ref__ = rhs.js_class_ref__;
    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
    JSClassRetain(js_class_ref__);
    return *this;
  }
  
  JSClass& JSClass::operator=(JSClass&& rhs) HAL_NOEXCEPT {
    HAL_JSCLASS_LOCK_GUARD;
    HAL_LOG_DEBUG("JSClass:: move assignment");
    HAL_LOG_DEBUG("JSClass:: release ", js_class_ref__);
    JSClassRelease(js_class_ref__);
    name__         = std::move(rhs.name__);
    js_class_ref__ = rhs.js_class_ref__;
    HAL_LOG_DEBUG("JSClass:: retain ", js_class_ref__);
    JSClassRetain(js_class_ref__);
    return *this;
  }
  
  void JSClass::swap(JSClass& other) HAL_NOEXCEPT {
    HAL_JSCLASS_LOCK_GUARD;
    HAL_LOG_DEBUG("JSClass:: swap");
    using std::swap;
    swap(name__        , other.name__);
    swap(js_class_ref__, other.js_class_ref__);
  }
  
} // namespace HAL {
