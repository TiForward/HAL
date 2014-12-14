/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSValue.hpp"

#include "HAL/JSContext.hpp"
#include "HAL/JSString.hpp"

#include "HAL/JSUndefined.hpp"
#include "HAL/JSNull.hpp"
#include "HAL/JSBoolean.hpp"
#include "HAL/JSNumber.hpp"

#include "HAL/JSObject.hpp"
#include "HAL/JSArray.hpp"
#include "HAL/JSDate.hpp"
#include "HAL/JSError.hpp"
#include "HAL/JSFunction.hpp"
#include "HAL/JSRegExp.hpp"

#include "HAL/JSClass.hpp"

#include "HAL/detail/JSUtil.hpp"

#include <sstream>
#include <cassert>

namespace HAL {
  
  
  JSString JSValue::ToJSONString(unsigned indent) {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSStringRef js_string_ref = JSValueCreateJSONString(js_context__, js_value_ref__, indent, &exception);
    if (exception) {
      // If this assert fails then we need to JSStringRelease
      // js_string_ref.
      assert(!js_string_ref);
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    if (js_string_ref) {
      JSString js_string(js_string_ref);
      JSStringRelease(js_string_ref);
      return js_string;
    }
    
    return JSString();
  }
  
  JSValue::operator JSString() const {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSStringRef js_string_ref = JSValueToStringCopy(js_context__, js_value_ref__, &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    assert(js_string_ref);
    JSString js_string(js_string_ref);
    JSStringRelease(js_string_ref);
    
    return js_string;
  }
  
  JSValue::operator std::string() const {
    return operator JSString();
  }
  
  JSValue::operator bool() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueToBoolean(js_context__, js_value_ref__);
  }
  
  JSValue::operator JSBoolean() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return js_context__.CreateBoolean(operator bool());
  }
  
  JSValue::operator double() const {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    const double result = JSValueToNumber(js_context__, js_value_ref__, &exception);
    
    if (exception) {
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    return result;
  }
  
  JSValue::operator int32_t() const {
    return detail::to_int32_t(operator double());
  }
  
  JSValue::operator JSNumber() const {
    HAL_JSVALUE_LOCK_GUARD;
    return js_context__.CreateNumber(operator double());
  }
  
  JSValue::operator JSObject() const {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    JSObjectRef js_object_ref = JSValueToObject(js_context__, js_value_ref__, &exception);
    
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_object_ref.
      assert(!js_object_ref);
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    assert(js_object_ref);
    return JSObject(js_context__, js_object_ref);
  }
  
  JSValue::Type JSValue::GetType() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    auto type = Type::Undefined;
    const JSType js_type = JSValueGetType(js_context__, js_value_ref__);
    switch (js_type) {
      case kJSTypeUndefined:
        type = Type::Undefined;
        break;
        
      case kJSTypeNull:
        type = Type::Null;
        break;
        
      case kJSTypeBoolean:
        type = Type::Boolean;
        break;
        
      case kJSTypeNumber:
        type = Type::Number;
        break;
        
      case kJSTypeString:
        type = Type::String;
        break;
        
      case kJSTypeObject:
        type = Type::Object;
        break;
    }

    return type;
  }
  
  bool JSValue::IsUndefined() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsUndefined(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsNull() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsNull(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsBoolean() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsBoolean(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsNumber() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsNumber(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsString() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsString(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsObject() const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsObject(js_context__, js_value_ref__);
  }
  
  bool JSValue::IsObjectOfClass(const JSClass& js_class) const HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    return JSValueIsObjectOfClass(js_context__, js_value_ref__, js_class);
  }
  
  bool JSValue::IsInstanceOfConstructor(const JSObject& constructor) const {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    const bool result = JSValueIsInstanceOfConstructor(js_context__, js_value_ref__, constructor, &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    return result;
  }
  
  bool JSValue::IsEqualWithTypeCoercion(const JSValue& rhs) const {
    HAL_JSVALUE_LOCK_GUARD;
    JSValueRef exception { nullptr };
    const bool result = JSValueIsEqual(js_context__, js_value_ref__, rhs.js_value_ref__, &exception);
    if (exception) {
      detail::ThrowRuntimeError("JSValue", JSValue(js_context__, exception));
    }
    
    return result;
  }
  
  bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs) {
    return lhs.IsEqualWithTypeCoercion(rhs);
  }
  
  JSValue::~JSValue() HAL_NOEXCEPT {
    HAL_LOG_DEBUG("JSValue:: dtor");
    HAL_LOG_DEBUG("JSValue:: release ", js_value_ref__);
    JSValueUnprotect(js_context__, js_value_ref__);
  }
  
  JSValue::JSValue(const JSValue& rhs) HAL_NOEXCEPT
  : js_context__(rhs.js_context__)
  , js_value_ref__(rhs.js_value_ref__) {
    HAL_LOG_DEBUG("JSValue:: copy ctor");
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
    JSValueProtect(js_context__, js_value_ref__);
  }
  
  JSValue::JSValue(JSValue&& rhs) HAL_NOEXCEPT
  : js_context__(std::move(rhs.js_context__))
  , js_value_ref__(rhs.js_value_ref__) {
    HAL_LOG_DEBUG("JSValue:: move ctor");
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
    JSValueProtect(js_context__, js_value_ref__);
  }
  
  JSValue& JSValue::operator=(const JSValue& rhs) {
    HAL_JSVALUE_LOCK_GUARD;
    HAL_LOG_DEBUG("JSValue:: copy assignment");
    // JSValues can only be copied between contexts within the same
    // context group.
    if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
      detail::ThrowRuntimeError("JSValue", "JSValues must belong to JSContexts within the same JSContextGroup to be shared and exchanged.");
    }
    
    HAL_LOG_DEBUG("JSValue:: release ", js_value_ref__);
    JSValueUnprotect(js_context__, js_value_ref__);
    js_context__   = rhs.js_context__;
    js_value_ref__ = rhs.js_value_ref__;
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
    JSValueProtect(js_context__, js_value_ref__);
    return *this;
  }
  
  JSValue& JSValue::operator=(JSValue&& rhs) {
    HAL_JSVALUE_LOCK_GUARD;
    HAL_LOG_DEBUG("JSValue:: move assignment");
    // JSValues can only be copied between contexts within the same
    // context group.
    if (js_context__.get_context_group() != rhs.js_context__.get_context_group()) {
      detail::ThrowRuntimeError("JSValue", "JSValues must belong to JSContexts within the same JSContextGroup to be shared and exchanged.");
    }

    HAL_LOG_DEBUG("JSValue:: release ", js_value_ref__);
    JSValueUnprotect(js_context__, js_value_ref__);
    js_context__   = std::move(rhs.js_context__);
    js_value_ref__ = rhs.js_value_ref__;
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
    JSValueProtect(js_context__, js_value_ref__);
    return *this;
  }
  
  void JSValue::swap(JSValue& other) HAL_NOEXCEPT {
    HAL_JSVALUE_LOCK_GUARD;
    HAL_LOG_DEBUG("JSValue:: swap");
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context__  , other.js_context__);
    swap(js_value_ref__, other.js_value_ref__);
  }
  
  JSValue::JSValue(const JSContext& js_context, const JSString& js_string, bool parse_as_json)
  : js_context__(js_context) {
    HAL_LOG_DEBUG("JSValue:: ctor");
    if (parse_as_json) {
      js_value_ref__ = JSValueMakeFromJSONString(js_context, js_string);
      if (!js_value_ref__) {
        const std::string message = "Input is not a valid JSON string: " + to_string(js_string);
        detail::ThrowRuntimeError("JSValue", message);
      }
    } else {
      js_value_ref__ = JSValueMakeString(js_context__, js_string);
    }
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSValue::JSValue(const JSContext& js_context, JSValueRef js_value_ref) HAL_NOEXCEPT
  : js_context__(js_context)
  , js_value_ref__(js_value_ref)  {
    HAL_LOG_DEBUG("JSValue:: ctor");
    assert(js_value_ref__);
    HAL_LOG_DEBUG("JSValue:: retain ", js_value_ref__);
    JSValueProtect(js_context__, js_value_ref__);
  }
  
  std::string to_string(const JSValue::Type& js_value_type) HAL_NOEXCEPT {
	  std::string string = "Unknown";
    switch (js_value_type) {
      case JSValue::Type::Undefined:
        string = "Undefined";
        break;
      case JSValue::Type::Null:
        string = "Null";
        break;
      case JSValue::Type::Boolean:
        string = "Boolean";
        break;
      case JSValue::Type::Number:
        string = "Number";
        break;
      case JSValue::Type::String:
        string = "String";
        break;
      case JSValue::Type::Object:
        string = "Object";
        break;
    }
    return string;
  }
  
  bool operator==(const JSValue& lhs, const JSValue& rhs) HAL_NOEXCEPT {
    return JSValueIsStrictEqual(lhs.get_context(), lhs, rhs);
  }
  
  
} // namespace HAL {
