/**
 * HAL
 * Author: Matthew D. Langston
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
    AddHasPropertyCallback(std::mem_fn(&JSExportObject::HasProperty));
    AddGetPropertyCallback(std::mem_fn(&JSExportObject::GetProperty));
    AddSetPropertyCallback(std::mem_fn(&JSExportObject::SetProperty));
    AddDeletePropertyCallback(std::mem_fn(&JSExportObject::DeleteProperty));
    AddGetPropertyNamesCallback(std::mem_fn(&JSExportObject::GetPropertyNames));
    AddCallAsFunctionCallback(std::mem_fn(&JSExportObject::CallAsFunction));
    AddValueProperty("prototype", std::mem_fn(&JSExportObject::GetPrototype), std::mem_fn(&JSExportObject::SetPrototype));
  }
  
  bool JSExportObject::HasProperty(const JSString& property_name) const HAL_NOEXCEPT {
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    //HAL_LOG_DEBUG("JSExportObject::HasProperty: ", property_name, " = ", std::to_string(js_object__.HasProperty(property_name)));
    return js_object__.HasProperty(property_name);
  }
  
  JSValue JSExportObject::GetProperty(const JSString& property_name) const HAL_NOEXCEPT {
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    //HAL_LOG_DEBUG("JSExportObject::GetProperty: ", property_name, " = ", to_string(js_object__.GetProperty(property_name)));
    return js_object__.GetProperty(property_name);
  }
  
  bool JSExportObject::SetProperty(const JSString& property_name, const JSValue& property_value) HAL_NOEXCEPT {
    HAL_JSEXPORTOBJECT_LOCK_GUARD;
    
    // TODO: Provide the virtual function
    //
    // virtual bool WillSetProperty(property_name, property_value);
    //
    // that derived classes can implement for property validation and
    // custom access control (e.g. to enforce security policies)."
    
    if (js_object__.HasProperty(property_name)) {
      JSValue previous_value = js_object__.GetProperty(property_name);
      
      // TODO: Provide the virtual function
      //
      // virtual bool WillReplaceProperty(property_name, previous_value, property_value);
      //
      // that derived classes can implement for property validation
      // and custom access control (e.g. to enforce security
      // policies)."
    }
    
    bool property_set = false;
    try {
      js_object__.SetProperty(property_name, property_value);
      property_set = true;
    } catch (...) {
    }
    
    //HAL_LOG_DEBUG("JSExportObject::SetProperty: property_set = ", std::to_string(property_set), " for ", property_name, " = ", to_string(property_value));
    
    // TODO: Provide the virtual function
    //
    // virtual bool DidSetProperty(property_name, previous_value, property_value);
    //
    // that derived classes can implement for their own implementation
    // concerns.
    
    return property_set;
  }
  
  bool JSExportObject::DeleteProperty(const JSString& property_name) HAL_NOEXCEPT {
    HAL_JSEXPORTOBJECT_LOCK_GUARD;
    
    bool property_deleted = false;
    
    if (js_object__.HasProperty(property_name)) {
      JSValue current_value = js_object__.GetProperty(property_name);
      
      // TODO: Provide the virtual function
      //
      // virtual bool WillDeleteProperty(property_name, current_value);
      //
      // that derived classes can implement for custom access control
      // (e.g. to enforce security policies)."
      
      try {
        js_object__.DeleteProperty(property_name);
        property_deleted = true;
      } catch (...) {
      }
    }
    
    //HAL_LOG_DEBUG("JSExportObject::DeleteProperty: property_deleted = ", std::to_string(property_deleted), " for ", property_name);
    
    return property_deleted;
  }
  
  std::vector<JSString> JSExportObject::GetEnumerablePropertyNames() const HAL_NOEXCEPT {
    return static_cast<std::vector<JSString>>(js_object__.GetPropertyNames());
  }
  
  JSValue JSExportObject::CallAsFunction(const std::vector<JSValue>& arguments, JSObject this_object) {
    return get_context().CreateUndefined();
  }
  
  JSValue JSExportObject::GetPrototype() const HAL_NOEXCEPT {
    return js_object__.GetPrototype();
  }
  
  bool JSExportObject::SetPrototype(const JSValue& js_value) HAL_NOEXCEPT {
    js_object__.SetPrototype(js_value);
    return true;
  }
  
  JSContext JSExportObject::get_context() const HAL_NOEXCEPT {
    return js_object__.get_context();
  }
  
  JSExportObject::JSExportObject(const JSContext& js_context) HAL_NOEXCEPT
  : js_object__(js_context.CreateObject()) {
  }
  
  JSExportObject::JSExportObject(const JSExportObject& rhs, const std::vector<JSValue>& arguments) HAL_NOEXCEPT
  : js_object__(rhs.js_object__) {
  }
  
  JSExportObject::~JSExportObject() HAL_NOEXCEPT {
  }
  
  void JSExportObject::GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const HAL_NOEXCEPT {
    for (const auto& entry : GetEnumerablePropertyNames()) {
      accumulator.AddName(entry);
    }
  }
  
  void JSExportObject::swap(JSExportObject& other) HAL_NOEXCEPT {
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_object__  , other.js_object__);
  }
  
} // namespace HAL {
