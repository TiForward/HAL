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
  
  JSClass::JSClass() JAVASCRIPTCORECPP_NOEXCEPT
  : name__("Default")
  , js_class_ref__(JSClassCreate(&kJSClassDefinitionEmpty)) {
  }
  
  JSClass::JSClass(const JSClassDefinition& js_class_definition) JAVASCRIPTCORECPP_NOEXCEPT
  : name__(js_class_definition.name__)
  , js_class_ref__(JSClassCreate(&js_class_definition.js_class_definition__)) {
  }
  
  JSClass::~JSClass() JAVASCRIPTCORECPP_NOEXCEPT {
    JSClassRelease(js_class_ref__);
  }
  
  JSClass::JSClass(const JSClass& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : name__(rhs.name__)
  , js_class_ref__(rhs.js_class_ref__) {
    JSClassRetain(js_class_ref__);
  }
  
  JSClass::JSClass(JSClass&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : name__(std::move(rhs.name__))
  , js_class_ref__(rhs.js_class_ref__) {
    JSClassRetain(js_class_ref__);
  }
  
  JSClass& JSClass::operator=(const JSClass& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    JSClassRelease(js_class_ref__);
    name__         = rhs.name__;
    js_class_ref__ = rhs.js_class_ref__;
    JSClassRetain(js_class_ref__);
    return *this;
  }
  
  JSClass& JSClass::operator=(JSClass&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    swap(rhs);
    JSClassRetain(js_class_ref__);
    return *this;
  }
  
  void JSClass::swap(JSClass& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
    using std::swap;
    swap(name__        , other.name__);
    swap(js_class_ref__, other.js_class_ref__);
  }
  
} // namespace JavaScriptCoreCPP {
