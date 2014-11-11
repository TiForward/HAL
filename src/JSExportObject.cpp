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
  
  bool JSExportObject::HasProperty(const JSString& property_name) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    return js_property_map__.count(property_name) > 0;
  }
  
  JSValue JSExportObject::GetProperty(const JSString& property_name) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
    // TODO: Provide the virtual function
    //
    // virtual bool WillGetProperty(property_name);
    //
    // that derived classes can implement for custom access control
    // (e.g. to enforce security policies)."
    
    const auto position = js_property_map__.find(property_name);
    
    if (position == js_property_map__.end()) {
      return get_context().CreateUndefined();
    }
    
    return position -> second;
  }
  
  bool JSExportObject::SetProperty(const JSString& property_name, const JSValue& property_value) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
    // TODO: Provide the virtual function
    //
    // virtual bool WillSetProperty(property_name, property_value);
    //
    // that derived classes can implement for property validation and
    // custom access control (e.g. to enforce security policies)."
    
    const auto position = js_property_map__.find(property_name);
    
    if (position != js_property_map__.end()) {
      // The map already contains this key, so replace it.
      
      JSValue previous_value = position -> second;
      
      // TODO: Provide the virtual function
      //
      // virtual bool WillReplaceProperty(property_name, previous_value, property_value);
      //
      // that derived classes can implement for property validation
      // and custom access control (e.g. to enforce security
      // policies)."
      
      const auto number_of_elements_removed = js_property_map__.erase(property_name);
      
      // postcondition
      assert(number_of_elements_removed == 1);
    }
    
    const auto property_emplace_result = js_property_map__.emplace(std::make_pair(property_name, property_value));
    const bool property_emplaced       = property_emplace_result.second;
    
    JAVASCRIPTCORECPP_LOG_DEBUG("JSExportObject::SetProperty", ": property ", property_name, " emplace = ", property_emplaced);
    // postcondition
    assert(property_emplaced);
    
    // TODO: Provide the virtual function
    //
    // virtual bool DidSetProperty(property_name, previous_value, property_value);
    //
    // that derived classes can implement for their own implementation
    // concerns.
    
    return property_emplaced;
  }
  
  bool JSExportObject::DeleteProperty(const JSString& property_name) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    
    bool property_deleted = false;
    const auto position = js_property_map__.find(property_name);
    
    if (position != js_property_map__.end()) {
      JSValue current_value = position -> second;
      
      // TODO: Provide the virtual function
      //
      // virtual bool WillDeleteProperty(property_name, current_value);
      //
      // that derived classes can implement for custom access control
      // (e.g. to enforce security policies)."
      
      const auto number_of_elements_removed = js_property_map__.erase(property_name);
      property_deleted                      = (number_of_elements_removed == 1);
      
      // postcondition
      assert(property_deleted);
    }
    
    return property_deleted;
  }
  
  std::size_t JSExportObject::GetPropertyMapCount() const JAVASCRIPTCORECPP_NOEXCEPT {
    return js_property_map__.size();
  }
  
  JSPropertyMap_t JSExportObject::GetPropertyMap() const JAVASCRIPTCORECPP_NOEXCEPT {
    return js_property_map__;
  }
  
  bool JSExportObject::IsFunction() const JAVASCRIPTCORECPP_NOEXCEPT {
    return is_function__;
  }
  
  bool JSExportObject::IsConstructor() const JAVASCRIPTCORECPP_NOEXCEPT {
    return is_constructor__;
  }
  
  JSExportObject::JSExportObject(const JSContext& js_context) JAVASCRIPTCORECPP_NOEXCEPT
  : JSExport<JSExportObject>(js_context) {
  }
  
  JSExportObject::~JSExportObject() JAVASCRIPTCORECPP_NOEXCEPT {
  }
  
  JSExportObject::JSExportObject(const JSExportObject& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSExport<JSExportObject>(rhs)
  , js_property_map__(rhs.js_property_map__)
  , is_function__(rhs.is_function__)
  , is_constructor__(rhs.is_constructor__) {
  }
  
  JSExportObject::JSExportObject(JSExportObject&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : JSExport<JSExportObject>(rhs)
  , js_property_map__(std::move(rhs.js_property_map__))
  , is_function__(rhs.is_function__)
  , is_constructor__(rhs.is_constructor__) {
  }
  
  JSExportObject& JSExportObject::operator=(const JSExportObject& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    JSExport<JSExportObject>::operator=(rhs);
    
    is_function__     = rhs.is_function__;
    is_constructor__  = rhs.is_constructor__;
    js_property_map__ = rhs.js_property_map__;
    
    return *this;
  }
  
  JSExportObject& JSExportObject::operator=(JSExportObject&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    swap(rhs);
    return *this;
  }
  
  void JSExportObject::swap(JSExportObject& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSEXPORTOBJECT_LOCK_GUARD;
    JSExport<JSExportObject>::swap(other);
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(is_function__    , other.is_function__);
    swap(is_constructor__ , other.is_constructor__);
    swap(js_property_map__, other.js_property_map__);
  }
  
} // namespace JavaScriptCoreCPP {
