/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSExportObject.hpp"
#include "JavaScriptCoreCPP/JSUndefined.hpp"
#include <utility>

namespace JavaScriptCoreCPP {
  
  void JSExportObject::JSExportInitialize() {
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::JSExportInitialize");
    SetClassVersion(1);
    AddHasPropertyCallback(std::mem_fn(&JSExportObject::HasProperty));
    AddGetPropertyCallback(std::mem_fn(&JSExportObject::GetProperty));
    AddSetPropertyCallback(std::mem_fn(&JSExportObject::SetProperty));
    AddDeletePropertyCallback(std::mem_fn(&JSExportObject::DeleteProperty));
    AddGetPropertyNamesCallback(std::mem_fn(&JSExportObject::GetPropertyNames));
    AddCallAsFunctionCallback(std::mem_fn(&JSExportObject::CallAsFunction));
  }
  
  bool JSExportObject::HasProperty(const JSString& property_name) const JAVASCRIPTCORECPP_NOEXCEPT {
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    //JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::HasProperty: ", property_name, " = ", std::to_string(js_object__.HasProperty(property_name)));
    return js_object__.HasProperty(property_name);
  }
  
  JSValue JSExportObject::GetProperty(const JSString& property_name) const JAVASCRIPTCORECPP_NOEXCEPT {
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    //JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::GetProperty: ", property_name, " = ", to_string(js_object__.GetProperty(property_name)));
    return js_object__.GetProperty(property_name);
  }
  
  bool JSExportObject::SetProperty(const JSString& property_name, const JSValue& property_value) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
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
    
    //JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::SetProperty: property_set = ", std::to_string(property_set), " for ", property_name, " = ", to_string(property_value));
    
    // TODO: Provide the virtual function
    //
    // virtual bool DidSetProperty(property_name, previous_value, property_value);
    //
    // that derived classes can implement for their own implementation
    // concerns.
    
    return property_set;
  }
  
  bool JSExportObject::DeleteProperty(const JSString& property_name) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
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
    
    //JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::DeleteProperty: property_deleted = ", std::to_string(property_deleted), " for ", property_name);
    
    return property_deleted;
  }
  
  std::vector<JSString> JSExportObject::GetEnumerablePropertyNames() const JAVASCRIPTCORECPP_NOEXCEPT {
    return static_cast<std::vector<JSString>>(js_object__.GetPropertyNames());
  }
  
  JSValue JSExportObject::CallAsFunction(const std::vector<JSValue>& arguments, JSObject this_object) {
    return get_context().CreateUndefined();
  }
  
  JSContext JSExportObject::get_context() const JAVASCRIPTCORECPP_NOEXCEPT {
    return js_object__.get_context();
  }
  
  JSExportObject::JSExportObject(const JSContext& js_context) JAVASCRIPTCORECPP_NOEXCEPT
  : js_object__(js_context.CreateObject()) {
  }
  
  JSExportObject::JSExportObject(const JSExportObject& rhs, const std::vector<JSValue>& arguments) JAVASCRIPTCORECPP_NOEXCEPT
  : js_object__(rhs.js_object__) {
  }
  
  JSExportObject::~JSExportObject() JAVASCRIPTCORECPP_NOEXCEPT {
  }
  
  void JSExportObject::GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const JAVASCRIPTCORECPP_NOEXCEPT {
    for (const auto& entry : GetEnumerablePropertyNames()) {
      accumulator.AddName(entry);
    }
  }
  
  void JSExportObject::swap(JSExportObject& other) JAVASCRIPTCORECPP_NOEXCEPT {
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_object__  , other.js_object__);
  }
  
} // namespace JavaScriptCoreCPP {
