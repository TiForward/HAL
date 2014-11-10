/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSValue.hpp"

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSPropertyNameArray.hpp"

#include "JavaScriptCoreCPP/JSUndefined.hpp"
#include "JavaScriptCoreCPP/JSNull.hpp"
#include "JavaScriptCoreCPP/JSBoolean.hpp"
#include "JavaScriptCoreCPP/JSNumber.hpp"

#include "JavaScriptCoreCPP/detail/JSPropertyNameAccumulator.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <algorithm>
#include <type_traits>
#include <sstream>
#include <limits>

namespace JavaScriptCoreCPP {
  
  bool JSObject::HasProperty(const JSString& property_name) const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSObjectHasProperty(js_context__, js_object_ref__, property_name);
  }
  
  JSValue JSObject::GetProperty(const JSString& property_name) const {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref = JSObjectGetProperty(js_context__, js_object_ref__, property_name, &exception);
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_value_ref.
      assert(!js_value_ref);
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    assert(js_value_ref);
    return JSValue(js_context__, js_value_ref);
  }
  
  JSValue JSObject::GetProperty(unsigned property_index) const {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(js_context__, js_object_ref__, property_index, &exception);
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_value_ref.
      assert(!js_value_ref);
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    assert(js_value_ref);
    return JSValue(js_context__, js_value_ref);
  }
  
  void JSObject::SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute>& attributes) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    JSObjectSetProperty(js_context__, js_object_ref__, property_name, property_value, detail::ToJSPropertyAttributes(attributes), &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
  }
  
  void JSObject::SetProperty(unsigned property_index, const JSValue& property_value) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    JSObjectSetPropertyAtIndex(js_context__, js_object_ref__, property_index, property_value, &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
  }
  
  bool JSObject::DeleteProperty(const JSString& property_name) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    const bool result = JSObjectDeleteProperty(js_context__, js_object_ref__, property_name, &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    return result;
  }
  
  JSPropertyNameArray JSObject::CopyPropertyNames() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    return JSPropertyNameArray(*this);
  }
  
  bool JSObject::IsConstructor() const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSObjectIsConstructor(js_context__, js_object_ref__);
  }
  
  JSObject JSObject::CallAsConstructor(                                      ) { return CallAsConstructor(std::vector<JSValue>  {}        ); }
  JSObject JSObject::CallAsConstructor(const JSValue&               argument ) { return CallAsConstructor(std::vector<JSValue>  {argument}); }
  JSObject JSObject::CallAsConstructor(const JSString&              argument ) { return CallAsConstructor(std::vector<JSString> {argument}); }
  JSObject JSObject::CallAsConstructor(const std::vector<JSString>& arguments) { return CallAsConstructor(detail::to_vector(js_context__, arguments)); }
  JSObject JSObject::CallAsConstructor(const std::vector<JSValue>&  arguments) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    
    if (!IsConstructor()) {
      detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a constructor.");
    }
    
    JSValueRef exception { nullptr };
    JSObjectRef js_object_ref = nullptr;
    if (!arguments.empty()) {
      const auto arguments_array = detail::to_vector(arguments);
      js_object_ref = JSObjectCallAsConstructor(js_context__, js_object_ref__, arguments_array.size(), &arguments_array[0], &exception);
    } else {
      js_object_ref = JSObjectCallAsConstructor(js_context__, js_object_ref__, 0, nullptr, &exception);
    }
    
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_object_ref.
      assert(!js_object_ref);
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    // postcondition
    assert(js_object_ref);
    return JSObject(js_context__, js_object_ref);
  }
  
  bool JSObject::IsFunction() const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSObjectIsFunction(js_context__, js_object_ref__);
  }
  
  JSValue JSObject::operator()(                                                            ) { return CallAsFunction(std::vector<JSValue>()                      , *this      ); }
  JSValue JSObject::operator()(const JSValue&               argument                       ) { return CallAsFunction({argument}                                  , *this      ); }
  JSValue JSObject::operator()(const JSString&              argument                       ) { return CallAsFunction(detail::to_vector(js_context__, {argument}) , *this      ); }
  JSValue JSObject::operator()(const std::vector<JSValue>&  arguments                      ) { return CallAsFunction(arguments                                   , *this      ); }
  JSValue JSObject::operator()(const std::vector<JSString>& arguments                      ) { return CallAsFunction(detail::to_vector(js_context__, arguments)  , *this      ); }
  JSValue JSObject::operator()(                                        JSObject this_object) { return CallAsFunction(std::vector<JSValue>()                      , this_object); }
  JSValue JSObject::operator()(const JSValue&               argument , JSObject this_object) { return CallAsFunction({argument}                                  , this_object); }
  JSValue JSObject::operator()(const JSString&              argument , JSObject this_object) { return CallAsFunction(detail::to_vector(js_context__, {argument}) , this_object); }
  JSValue JSObject::operator()(const std::vector<JSValue>&  arguments, JSObject this_object) { return CallAsFunction(arguments                                   , this_object); }
  JSValue JSObject::operator()(const std::vector<JSString>& arguments, JSObject this_object) { return CallAsFunction(detail::to_vector(js_context__, arguments)  , this_object); }
  
  JSValue JSObject::GetPrototype() const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSValue(js_context__, JSObjectGetPrototype(js_context__, js_object_ref__));
  }
  
  void JSObject::SetPrototype(const JSValue& js_value) JAVASCRIPTCORECPP_NOEXCEPT {
    JSObjectSetPrototype(js_context__, js_object_ref__, js_value);
  }
  
  void* JSObject::GetPrivate() const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSObjectGetPrivate(js_object_ref__);
  }
  
  bool JSObject::SetPrivate(void* data) const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSObjectSetPrivate(js_object_ref__, data);
  }
  
  JSObject::~JSObject() JAVASCRIPTCORECPP_NOEXCEPT {
    JSValueUnprotect(js_context__, js_object_ref__);
  }
  
  JSObject::JSObject(const JSObject& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context__(rhs.js_context__)
  , js_object_ref__(rhs.js_object_ref__) {
    JSValueProtect(js_context__, js_object_ref__);
  }
  
  JSObject::JSObject(JSObject&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context__(std::move(rhs.js_context__))
  , js_object_ref__(rhs.js_object_ref__) {
    JSValueProtect(js_context__, js_object_ref__);
  }
  
  JSObject& JSObject::operator=(const JSObject& rhs) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    // JSValues can only be copied between contexts within the same
    // context group.
    if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
      detail::ThrowRuntimeError("JSObject", "JSObjects must belong to JSContexts within the same JSContextGroup to be shared and exchanged.");
    }
    
    js_context__ = rhs.js_context__;
    JSValueUnprotect(js_context__, js_object_ref__);
    js_object_ref__ = rhs.js_object_ref__;
    JSValueProtect(js_context__, js_object_ref__);
    
    return *this;
  }
  
  JSObject& JSObject::operator=(JSObject&& rhs) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    // JSValues can only be copied between contexts within the same
    // context group.
    if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
      detail::ThrowRuntimeError("JSObject", "JSObjects must belong to JSContexts within the same JSContextGroup to be shared and exchanged.");
    }
    
    js_context__ = std::move(rhs.js_context__);
    JSValueUnprotect(js_context__, js_object_ref__);
    js_object_ref__ = rhs.js_object_ref__;
    JSValueProtect(js_context__, js_object_ref__);
    
    return *this;
  }
  
  void JSObject::swap(JSObject& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context__   , other.js_context__);
    swap(js_object_ref__, other.js_object_ref__);
  }
  
  JSObject::JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data)
  : JSObject(js_context, JSObjectMake(js_context, js_class, private_data)) {
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSObject::JSObject(const JSContext& js_context, JSObjectRef js_object_ref)
  : js_context__(js_context)
  , js_object_ref__(js_object_ref) {
    JSValueProtect(js_context__, js_object_ref__);
  }
  
  JSObject::operator JSValue() const {
    return JSValue(js_context__, js_object_ref__);
  }
  
  JSValue JSObject::CallAsFunction(const std::vector<JSValue>&  arguments, JSObject this_object) {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    
    if (!IsFunction()) {
      detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a function.");
    }
    
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref { nullptr };
    if (!arguments.empty()) {
      const auto arguments_array = detail::to_vector(arguments);
      js_value_ref = JSObjectCallAsFunction(js_context__, js_object_ref__, this_object, arguments_array.size(), &arguments_array[0], &exception);
    } else {
      js_value_ref = JSObjectCallAsFunction(js_context__, js_object_ref__, this_object, 0, nullptr, &exception);
    }
    
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_value_ref.
      assert(!js_value_ref);
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    assert(js_value_ref);
    return JSValue(js_context__, js_value_ref);
  }
  
  void JSObject::GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSOBJECT_LOCK_GUARD;
    for (const auto& property_name : static_cast<std::vector<JSString>>(CopyPropertyNames())) {
      accumulator.AddName(property_name);
    }
  }
  
} // namespace JavaScriptCoreCPP {
