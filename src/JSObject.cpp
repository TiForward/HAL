/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSObject.hpp"
#include "HAL/JSValue.hpp"

#include "HAL/JSClass.hpp"

#include "HAL/JSUndefined.hpp"
#include "HAL/JSNull.hpp"
#include "HAL/JSBoolean.hpp"
#include "HAL/JSNumber.hpp"
#include "HAL/JSError.hpp"
#include "HAL/JSArray.hpp"

#include "HAL/detail/JSPropertyNameAccumulator.hpp"
#include "HAL/detail/JSUtil.hpp"

#include <algorithm>
#include <type_traits>
#include <sstream>
#include <limits>

namespace HAL {
  
  bool JSObject::HasProperty(const JSString& property_name) const HAL_NOEXCEPT {
    return JSObjectHasProperty(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSStringRef>(property_name));
  }
  
  JSValue JSObject::GetProperty(const JSString& property_name) const {
    HAL_JSOBJECT_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref = JSObjectGetProperty(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSStringRef>(property_name), &exception);
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
    HAL_JSOBJECT_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(static_cast<JSContextRef>(js_context__), js_object_ref__, property_index, &exception);
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
    HAL_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    JSObjectSetProperty(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSStringRef>(property_name), static_cast<JSValueRef>(property_value), detail::ToJSPropertyAttributes(attributes), &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
  }
  
  void JSObject::SetProperty(unsigned property_index, const JSValue& property_value) {
    HAL_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    JSObjectSetPropertyAtIndex(static_cast<JSContextRef>(js_context__), js_object_ref__, property_index, static_cast<JSValueRef>(property_value), &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
  }
  
  bool JSObject::DeleteProperty(const JSString& property_name) {
    HAL_JSOBJECT_LOCK_GUARD;
    
    JSValueRef exception { nullptr };
    const bool result = JSObjectDeleteProperty(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSStringRef>(property_name), &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSObject", JSValue(js_context__, exception));
    }
    
    return result;
  }
  
  JSPropertyNameArray JSObject::GetPropertyNames() const HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;
    return JSPropertyNameArray(*this);
  }

  std::unordered_map<std::string, JSValue> JSObject::GetProperties() const HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;
    std::unordered_map<std::string, JSValue> properties;
    for (const auto& property_name : static_cast<std::vector<JSString>>(GetPropertyNames())) {
      properties.emplace(property_name, GetProperty(property_name));
    }
    return properties;
  }
  
  bool JSObject::IsFunction() const HAL_NOEXCEPT {
    return JSObjectIsFunction(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }

  bool JSObject::IsArray() const HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;

    JSObject global_object = js_context__.get_global_object();
    JSValue array_value = global_object.GetProperty("Array");
    if (!array_value.IsObject()) {
      return false;
    }
    
    JSObject array = static_cast<JSObject>(array_value);
    JSValue isArray_value = array.GetProperty("isArray");
    if (!isArray_value.IsObject()) {
      return false;
    }
    JSObject isArray = static_cast<JSObject>(isArray_value);
    if (!isArray.IsFunction()) {
      return false;
    }

    JSValue self = static_cast<JSValue>(*this);
    JSValue result = isArray(self, global_object);
    if (result.IsBoolean()) {
        return static_cast<bool>(result);
    }
    
    return false;
  }
  
  bool JSObject::IsError() const HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;
    const auto global_object = js_context__.get_global_object();
    const auto error_value = global_object.GetProperty("Error");
    if (!error_value.IsObject()) {
      return false;
    }
    const auto error = static_cast<JSObject>(error_value);
    const auto self  = static_cast<JSValue>(*this);
    return static_cast<std::string>(self) == "[object Error]" || self.IsInstanceOfConstructor(error);
  }
  
  JSValue JSObject::operator()(                                        JSObject this_object) { return CallAsFunction(std::vector<JSValue>()                      , this_object); }
  JSValue JSObject::operator()(JSValue&                     argument , JSObject this_object) { return CallAsFunction({argument}                                  , this_object); }
  JSValue JSObject::operator()(const JSString&              argument , JSObject this_object) { return CallAsFunction(detail::to_vector(js_context__, {argument}) , this_object); }
  JSValue JSObject::operator()(const std::vector<JSValue>&  arguments, JSObject this_object) { return CallAsFunction(arguments                                   , this_object); }
  JSValue JSObject::operator()(const std::vector<JSString>& arguments, JSObject this_object) { return CallAsFunction(detail::to_vector(js_context__, arguments)  , this_object); }
  
  bool JSObject::IsConstructor() const HAL_NOEXCEPT {
    return JSObjectIsConstructor(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }
  
  JSObject JSObject::CallAsConstructor(                                      ) { return CallAsConstructor(std::vector<JSValue>  {}        ); }
  JSObject JSObject::CallAsConstructor(const JSValue&               argument ) { return CallAsConstructor(std::vector<JSValue>  {argument}); }
  JSObject JSObject::CallAsConstructor(const JSString&              argument ) { return CallAsConstructor(std::vector<JSString> {argument}); }
  JSObject JSObject::CallAsConstructor(const std::vector<JSString>& arguments) { return CallAsConstructor(detail::to_vector(js_context__, arguments)); }
  JSObject JSObject::CallAsConstructor(const std::vector<JSValue>&  arguments) {
    HAL_JSOBJECT_LOCK_GUARD;
    
    if (!IsConstructor()) {
      detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a constructor.");
    }
    
    JSValueRef exception { nullptr };
    JSObjectRef js_object_ref = nullptr;
    if (!arguments.empty()) {
      const auto arguments_array = detail::to_vector(arguments);
      js_object_ref = JSObjectCallAsConstructor(static_cast<JSContextRef>(js_context__), js_object_ref__, arguments_array.size(), &arguments_array[0], &exception);
    } else {
      js_object_ref = JSObjectCallAsConstructor(static_cast<JSContextRef>(js_context__), js_object_ref__, 0, nullptr, &exception);
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
  
  JSValue JSObject::GetPrototype() const HAL_NOEXCEPT {
    return JSValue(js_context__, JSObjectGetPrototype(static_cast<JSContextRef>(js_context__), js_object_ref__));
  }
  
  void JSObject::SetPrototype(const JSValue& js_value) HAL_NOEXCEPT {
    JSObjectSetPrototype(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSValueRef>(js_value));
  }
  
  void* JSObject::GetPrivate() const HAL_NOEXCEPT {
    return JSObjectGetPrivate(js_object_ref__);
  }
  
  bool JSObject::SetPrivate(void* data) const HAL_NOEXCEPT {
    UnRegisterPrivateData(GetPrivate());
    RegisterPrivateData(js_object_ref__, data);
    return JSObjectSetPrivate(js_object_ref__, data);
  }
  
  JSObject::~JSObject() HAL_NOEXCEPT {
    HAL_LOG_TRACE("JSObject:: dtor ", this);
    HAL_LOG_TRACE("JSObject:: release ", js_object_ref__, " for ", this);
    JSValueUnprotect(static_cast<JSContextRef>(js_context__), js_object_ref__);
    UnRegisterJSContext(js_object_ref__);
  }
  
  JSObject::JSObject(const JSObject& rhs) HAL_NOEXCEPT
  : js_context__(rhs.js_context__)
  , js_object_ref__(rhs.js_object_ref__) {
    HAL_LOG_TRACE("JSObject:: copy ctor ", this);
    HAL_LOG_TRACE("JSObject:: retain ", js_object_ref__, " for ", this);
    JSValueProtect(static_cast<JSContextRef>(js_context__), js_object_ref__);
    RegisterJSContext(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }
  
  JSObject::JSObject(JSObject&& rhs) HAL_NOEXCEPT
  : js_context__(std::move(rhs.js_context__))
  , js_object_ref__(rhs.js_object_ref__) {
    HAL_LOG_TRACE("JSObject:: move ctor ", this);
    HAL_LOG_TRACE("JSObject:: retain ", js_object_ref__, " for ", this);
    JSValueProtect(static_cast<JSContextRef>(js_context__), js_object_ref__);
    RegisterJSContext(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }
  
  JSObject& JSObject::operator=(JSObject rhs) {
    HAL_JSOBJECT_LOCK_GUARD;
    HAL_LOG_TRACE("JSObject:: assignment ", this);
    // JSValues can only be copied between contexts within the same
    // context group.
    if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
      detail::ThrowRuntimeError("JSObject", "JSObjects must belong to JSContexts within the same JSContextGroup to be shared and exchanged.");
    }
    
    swap(rhs);
    return *this;
  }
  
  void JSObject::swap(JSObject& other) HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;
    HAL_LOG_TRACE("JSObject:: swap ", this);
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context__   , other.js_context__);
    swap(js_object_ref__, other.js_object_ref__);
  }
  
  JSObject::JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data)
  : js_context__(js_context)
  , js_object_ref__(JSObjectMake(static_cast<JSContextRef>(js_context), static_cast<JSClassRef>(js_class), private_data)) {
    HAL_LOG_TRACE("JSObject:: ctor 1 ", this);
    HAL_LOG_TRACE("JSObject:: retain ", js_object_ref__, " (implicit) for ", this);
    JSValueProtect(static_cast<JSContextRef>(js_context__), js_object_ref__);
    RegisterJSContext(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }

  // For interoperability with the JavaScriptCore C API.
  JSObject::JSObject(const JSContext& js_context, JSObjectRef js_object_ref)
  : js_context__(js_context)
  , js_object_ref__(js_object_ref) {
    HAL_LOG_TRACE("JSObject:: ctor 2 ", this);
    HAL_LOG_TRACE("JSObject:: retain ", js_object_ref__, " for ", this);
    JSValueProtect(static_cast<JSContextRef>(js_context__), js_object_ref__);
    RegisterJSContext(static_cast<JSContextRef>(js_context__), js_object_ref__);
  }
  
  JSObject::operator JSValue() const {
    return JSValue(js_context__, js_object_ref__);
  }
  
  JSObject::operator JSArray() const {
    return JSArray(js_context__, js_object_ref__);
  }

  JSObject::operator JSError() const {
    return JSError(js_context__, js_object_ref__);
  }
  
  JSValue JSObject::CallAsFunction(const std::vector<JSValue>&  arguments, JSObject this_object) {
    HAL_JSOBJECT_LOCK_GUARD;
    
    if (!IsFunction()) {
      detail::ThrowRuntimeError("JSObject", "This JavaScript object is not a function.");
    }
    
    JSValueRef exception { nullptr };
    JSValueRef js_value_ref { nullptr };
    if (!arguments.empty()) {
      const auto arguments_array = detail::to_vector(arguments);
      js_value_ref = JSObjectCallAsFunction(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSObjectRef>(this_object), arguments_array.size(), &arguments_array[0], &exception);
    } else {
      js_value_ref = JSObjectCallAsFunction(static_cast<JSContextRef>(js_context__), js_object_ref__, static_cast<JSObjectRef>(this_object), 0, nullptr, &exception);
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
  
  void JSObject::GetPropertyNames(const JSPropertyNameAccumulator& accumulator) const HAL_NOEXCEPT {
    HAL_JSOBJECT_LOCK_GUARD;
    for (const auto& property_name : static_cast<std::vector<JSString>>(GetPropertyNames())) {
      accumulator.AddName(property_name);
    }
  }
  
  std::unordered_map<std::intptr_t, std::tuple<std::intptr_t, std::size_t>> JSObject::js_object_ref_to_js_context_ref_map__;
  
  void JSObject::RegisterJSContext(JSContextRef js_context_ref, JSObjectRef js_object_ref) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    const auto key   = reinterpret_cast<std::intptr_t>(js_object_ref);
    const auto value = reinterpret_cast<std::intptr_t>(js_context_ref);
    
    const auto position = js_object_ref_to_js_context_ref_map__.find(key);
    const bool found    = position != js_object_ref_to_js_context_ref_map__.end();
    
    if (found) {
      auto tuple = position -> second;
      ++std::get<1>(tuple);
      js_object_ref_to_js_context_ref_map__.emplace(key, tuple);
      
      HAL_LOG_DEBUG("JSObject::RegisterJSContext: JSObjectRef = ", js_object_ref, ", JSContextRef = ", js_context_ref, " count = ", std::get<1>(tuple));
    } else {
      const auto insert_result = js_object_ref_to_js_context_ref_map__.emplace(key, std::make_tuple(value, 1));
      const bool inserted      = insert_result.second;
      
      // postcondition
      assert(inserted);
      HAL_LOG_DEBUG("JSObject::RegisterJSContext: JSObjectRef = ", js_object_ref, ", JSContextRef = ", js_context_ref, " count = 1");
    }
  }
  
  void JSObject::UnRegisterJSContext(JSObjectRef js_object_ref) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    const auto key      = reinterpret_cast<std::intptr_t>(js_object_ref);
    const auto position = js_object_ref_to_js_context_ref_map__.find(key);
    const bool found    = position != js_object_ref_to_js_context_ref_map__.end();
    
    // precondition
    if (found) {
      auto tuple = position -> second;
      JSContextRef js_context_ref = reinterpret_cast<JSContextRef>(std::get<0>(tuple));
      static_cast<void>(js_context_ref); // just meant to suppress "unused" compiler warning
      --std::get<1>(tuple);
      if (std::get<1>(tuple) == 0) {
        js_object_ref_to_js_context_ref_map__.erase(key);
      } else {
        js_object_ref_to_js_context_ref_map__.emplace(key, tuple);
      }
      HAL_LOG_DEBUG("JSObject::UnRegisterJSContext: JSObjectRef = ", js_object_ref, ", JSContextRef = ", js_context_ref, " count = ", std::get<1>(tuple));
    } else {
      HAL_LOG_DEBUG("JSObject::UnRegisterJSContext: JSObjectRef = ", js_object_ref, " not registered");
    }
  }

  JSObject JSObject::FindJSObject(JSContextRef js_context_ref, JSObjectRef js_object_ref) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    const auto key      = reinterpret_cast<std::intptr_t>(js_object_ref);
    const auto position = js_object_ref_to_js_context_ref_map__.find(key);
    const bool found    = position != js_object_ref_to_js_context_ref_map__.end();
    
    // precondition
    if (found) {
      js_context_ref = reinterpret_cast<JSContextRef>(std::get<0>(position -> second));
    }
    
    HAL_LOG_TRACE("JSObject::FindJSObject: found = ", found, " for JSObjectRef ", js_object_ref, ", JSContextRef = ", js_context_ref);
    
    return JSObject(JSContext(js_context_ref), js_object_ref);
  }

  std::unordered_map<std::intptr_t, std::intptr_t> JSObject::js_private_data_to_js_object_ref_map__;
  
  void JSObject::RegisterPrivateData(JSObjectRef js_object_ref, void* private_data) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    // we won't store nullptr
    if (private_data == nullptr) {
      return;
    }
    const auto key   = reinterpret_cast<std::intptr_t>(private_data);
    const auto value = reinterpret_cast<std::intptr_t>(js_object_ref);
    
    const auto position = js_private_data_to_js_object_ref_map__.find(key);
    const bool found    = position != js_private_data_to_js_object_ref_map__.end();
    
    if (found) {
      // private data should not be shared by multiple JSObjectRef
      assert((reinterpret_cast<JSObjectRef>(position -> second) == js_object_ref));
    } else {
      const auto insert_result = js_private_data_to_js_object_ref_map__.emplace(key, value);
      const bool inserted      = insert_result.second;
      
      // postcondition
      assert(inserted);
      
      HAL_LOG_DEBUG("JSObject::RegisterPrivateData: JSObjectRef = ", js_object_ref, ", data = ", private_data);
    }
  }
  
  void JSObject::UnRegisterPrivateData(void* private_data) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    // we won't store nullptr
    if (private_data == nullptr) {
      return;
    }
    const auto key      = reinterpret_cast<std::intptr_t>(private_data);
    const auto position = js_private_data_to_js_object_ref_map__.find(key);
    const bool found    = position != js_private_data_to_js_object_ref_map__.end();
    
    if (found) {
      JSObjectRef js_object_ref = reinterpret_cast<JSObjectRef>(position -> second);
      static_cast<void>(js_object_ref); // just meant to suppress "unused" compiler warning
      js_private_data_to_js_object_ref_map__.erase(key);
      HAL_LOG_DEBUG("JSObject::UnRegisterPrivateData: data = ", private_data, ", JSObjectRef = ", js_object_ref);
    } else {
      HAL_LOG_DEBUG("JSObject::UnRegisterPrivateData: data = ", private_data, " not registered");
    }
  }

  JSObject JSObject::FindJSObjectFromPrivateData(JSContext js_context, void* private_data) {
    HAL_JSOBJECT_LOCK_GUARD_STATIC;
    const auto key      = reinterpret_cast<std::intptr_t>(private_data);
    const auto position = js_private_data_to_js_object_ref_map__.find(key);
    const bool found    = position != js_private_data_to_js_object_ref_map__.end();

    // This could happen when owner object is gargabe collected while executing async operation.
    // This Error object will be only used internally to see if object is found or not.
    if (!found) {
      return js_context.CreateError();
    }

    JSObjectRef js_object_ref = reinterpret_cast<JSObjectRef>(position -> second);
    HAL_LOG_TRACE("JSObject::FindJSObjectFromPrivateData: found = ", found, " for data = ", key, ", JSObjectRef = ", js_object_ref);

    return FindJSObject(static_cast<JSContextRef>(js_context), js_object_ref);
  }

} // namespace HAL {
