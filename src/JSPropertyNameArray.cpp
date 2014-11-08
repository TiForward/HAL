/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSPropertyNameArray.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/JSObject.hpp"

#include <algorithm>

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {
  
  std::size_t JSPropertyNameArray::GetCount() const noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    return JSPropertyNameArrayGetCount(js_property_name_array_ref__);
  }
  
  JSString JSPropertyNameArray::GetNameAtIndex(std::size_t index) const noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    return JSString(JSPropertyNameArrayGetNameAtIndex(js_property_name_array_ref__, index));
  }
  
  JSPropertyNameArray::operator std::vector<JSString>() const noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    std::vector<JSString> property_names;
    for (std::size_t i = 0, count = GetCount(); i < count; ++i) {
      property_names.emplace_back(GetNameAtIndex(i));
    }
    
    return property_names;
  }
  
  JSPropertyNameArray::~JSPropertyNameArray() noexcept {
    JSPropertyNameArrayRelease(js_property_name_array_ref__);
  }
  
  JSPropertyNameArray::JSPropertyNameArray(const JSPropertyNameArray& rhs) noexcept
  : js_property_name_array_ref__(rhs.js_property_name_array_ref__) {
    JSPropertyNameArrayRetain(js_property_name_array_ref__);
  }
  
  JSPropertyNameArray::JSPropertyNameArray(JSPropertyNameArray&& rhs) noexcept
    : js_property_name_array_ref__(rhs.js_property_name_array_ref__) {
      JSPropertyNameArrayRetain(js_property_name_array_ref__);
  }
  
  JSPropertyNameArray& JSPropertyNameArray::operator=(const JSPropertyNameArray& rhs) noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    JSPropertyNameArrayRelease(js_property_name_array_ref__);
    js_property_name_array_ref__ = rhs.js_property_name_array_ref__;
    JSPropertyNameArrayRetain(js_property_name_array_ref__);
    return *this;
  }
  
  JSPropertyNameArray& JSPropertyNameArray::operator=(JSPropertyNameArray&& rhs) noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    JSPropertyNameArrayRelease(js_property_name_array_ref__);
    js_property_name_array_ref__ = rhs.js_property_name_array_ref__;
    JSPropertyNameArrayRetain(js_property_name_array_ref__);
    return *this;
  }
  
  void JSPropertyNameArray::swap(JSPropertyNameArray& other) noexcept {
    JAVASCRIPTCORECPP_JSPROPERTYNAMEARRAY_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_property_name_array_ref__, other.js_property_name_array_ref__);
  }
  
  JSPropertyNameArray::JSPropertyNameArray(const JSObject& js_object) noexcept
  : js_property_name_array_ref__(JSObjectCopyPropertyNames(js_object.get_context(), js_object)) {
  }
  
} // namespace JavaScriptCoreCPP {
