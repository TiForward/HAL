/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/detail/JSStaticValue.hpp"
#include "JavaScriptCoreCPP/detail/JSStaticFunction.hpp"

#include <string>
#include <algorithm>

namespace JavaScriptCoreCPP {
  
  JSClass::JSClass() noexcept {
    js_class_ref__ = JSClassCreate(&js_class_definition__.js_class_definition__);
  }
  
  JSClass::JSClass(const JSClassDefinition& js_class_definition) noexcept
  : js_class_definition__(js_class_definition) {
    js_class_ref__ = JSClassCreate(&js_class_definition__.js_class_definition__);
  }

  JSClass::JSClass(JSClassRef js_class_ref) noexcept {
    js_class_ref__ = JSClassRetain(js_class_ref);
  }

  JSClass::~JSClass() noexcept {
    JSClassRelease(js_class_ref__);
  }
  
  JSClass::JSClass(const JSClass& rhs) noexcept
  : js_class_definition__(rhs.js_class_definition__) {
    js_class_ref__ = JSClassCreate(&js_class_definition__.js_class_definition__);
  }
  
  JSClass::JSClass(JSClass&& rhs) noexcept
  : js_class_definition__(std::move(rhs.js_class_definition__)) {
    js_class_ref__ = JSClassCreate(&js_class_definition__.js_class_definition__);
  }
  
  JSClass& JSClass::operator=(const JSClass& rhs) noexcept {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    
    js_class_definition__ = rhs.js_class_definition__;

    JSClassRelease(js_class_ref__);
    js_class_ref__ = JSClassRetain(rhs.js_class_ref__);
    
    return *this;
  }
  
  JSClass& JSClass::operator=(JSClass&& rhs) noexcept {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    
    js_class_definition__ = std::move(rhs.js_class_definition__);

    JSClassRelease(js_class_ref__);
    js_class_ref__ = JSClassRetain(rhs.js_class_ref__);
    
    return *this;
  }
  
  void JSClass::swap(JSClass& other) noexcept {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    using std::swap;
    
    swap(js_class_definition__, other.js_class_definition__);
    swap(js_class_ref__       , other.js_class_ref__);
  }
  
} // namespace JavaScriptCoreCPP {
