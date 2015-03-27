/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSContext.hpp"

#include "HAL/JSClass.hpp"
#include "HAL/JSString.hpp"

#include "HAL/JSValue.hpp"
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

#include "HAL/detail/JSUtil.hpp"

#include <cassert>

namespace HAL {
  
  JSObject JSContext::get_global_object() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSObject(JSContext(js_global_context_ref__), JSContextGetGlobalObject(js_global_context_ref__));
  }
  
  JSValue JSContext::CreateValueFromJSON(const JSString& js_string) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), js_string, true);
  }
  
  JSValue JSContext::CreateString() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), JSString(), false);
  }
  
  JSValue JSContext::CreateString(const JSString& js_string) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), js_string, false);
  }
  
  JSValue JSContext::CreateString(const char* string) const HAL_NOEXCEPT {
    return CreateString(JSString(string));
  }
  
  JSValue JSContext::CreateString(const std::string& string) const HAL_NOEXCEPT {
    return CreateString(JSString(string));
  }
  
  JSUndefined JSContext::CreateUndefined() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSUndefined(JSContext(js_global_context_ref__));
  }
  
  JSNull JSContext::CreateNull() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSNull(JSContext(js_global_context_ref__));
  }
  
  JSBoolean JSContext::CreateBoolean(bool boolean) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSBoolean(JSContext(js_global_context_ref__), boolean);
  }
  
  JSNumber JSContext::CreateNumber(double number) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSNumber JSContext::CreateNumber(int32_t number) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSNumber JSContext::CreateNumber(uint32_t number) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSObject JSContext::CreateObject() const HAL_NOEXCEPT {
    return CreateObject(JSClass());
  }
  
  JSObject JSContext::CreateObject(const JSClass& js_class) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSObject(JSContext(js_global_context_ref__), js_class);
  }

  JSObject JSContext::CreateObject(const std::unordered_map<std::string, JSValue>& properties) const HAL_NOEXCEPT {
    return CreateObject(JSClass(), properties);
  }

  JSObject JSContext::CreateObject(const JSClass& js_class, const std::unordered_map<std::string, JSValue>& properties) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    auto object = CreateObject();
    for (const auto kv : properties) {
      object.SetProperty(kv.first, kv.second);
    }
    return object;
  }

  
  JSArray JSContext::CreateArray() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSArray(JSContext(js_global_context_ref__));
  }
  
  JSArray JSContext::CreateArray(const std::vector<JSValue>& arguments) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSArray(JSContext(js_global_context_ref__), arguments);
  }
  
  JSDate JSContext::CreateDate() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSDate(JSContext(js_global_context_ref__));
  }
  
  JSDate JSContext::CreateDate(const std::vector<JSValue>& arguments) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSDate(JSContext(js_global_context_ref__), arguments);
  }
  
  JSError JSContext::CreateError() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSError(JSContext(js_global_context_ref__));
  }
  
  JSError JSContext::CreateError(const std::vector<JSValue>& arguments) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSError(JSContext(js_global_context_ref__), arguments);
  }
  
  JSRegExp JSContext::CreateRegExp() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSRegExp(JSContext(js_global_context_ref__));
  }
  
  JSRegExp JSContext::CreateRegExp(const std::vector<JSValue>& arguments) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSRegExp(JSContext(js_global_context_ref__), arguments);
  }
  
  JSFunction JSContext::CreateFunction(const JSString& body) const {
    return CreateFunction(body, std::vector<JSString>(), JSString(), JSString());
  }
  
  JSFunction JSContext::CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names) const {
    return CreateFunction(body, parameter_names, JSString(), JSString());
  }
  
  JSFunction JSContext::CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name) const {
    return CreateFunction(body, parameter_names, function_name, JSString());
  }
  
  JSFunction JSContext::CreateFunction(const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name, const JSString& source_url, int starting_line_number) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSFunction(JSContext(js_global_context_ref__), body, parameter_names, function_name, source_url, starting_line_number);
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script) const {
    return JSEvaluateScript(script, get_global_object(), JSString());
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, const JSString& source_url, int starting_line_number) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    return JSEvaluateScript(script, get_global_object(), source_url, starting_line_number);
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, JSObject this_object) const {
    return JSEvaluateScript(script, this_object, JSString());
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, JSObject this_object, const JSString& source_url, int starting_line_number) const {
    HAL_JSCONTEXT_LOCK_GUARD;
    JSValueRef js_value_ref { nullptr };
    const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
    JSValueRef exception { nullptr };
    js_value_ref = ::JSEvaluateScript(js_global_context_ref__, static_cast<JSStringRef>(script), static_cast<JSObjectRef>(this_object), source_url_ref, starting_line_number, &exception);
    
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_value_ref.
      assert(!js_value_ref);
      detail::ThrowRuntimeError("JSContext", JSValue(JSContext(js_global_context_ref__), exception), source_url, starting_line_number);
    }
    
    return JSValue(JSContext(js_global_context_ref__), js_value_ref);
  }
  
  bool JSContext::JSCheckScriptSyntax(const JSString& script) const HAL_NOEXCEPT {
    return JSCheckScriptSyntax(script, JSString());
  }
  
  bool JSContext::JSCheckScriptSyntax(const JSString& script, const JSString& source_url, int starting_line_number) const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
    JSValueRef exception { nullptr };
    bool result = ::JSCheckScriptSyntax(js_global_context_ref__, static_cast<JSStringRef>(script), source_url_ref, starting_line_number, &exception);
    
    if (exception) {
      detail::ThrowRuntimeError("JSContext", JSValue(JSContext(js_global_context_ref__), exception));
    }
    
    return result;
  }
  
  void JSContext::GarbageCollect() const HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    JSGarbageCollect(js_global_context_ref__);
  }
  
#ifdef DEBUG
  extern "C" void JSSynchronousGarbageCollectForDebugging(JSContextRef);
  extern "C" void JSSynchronousEdenCollectForDebugging(JSContextRef);
  
  void JSContext::SynchronousGarbageCollectForDebugging() const {
    HAL_JSCONTEXT_LOCK_GUARD;
    JSSynchronousGarbageCollectForDebugging(js_global_context_ref__);
  }

  void JSContext::SynchronousEdenCollectForDebugging() const {
    HAL_JSCONTEXT_LOCK_GUARD;
    JSSynchronousEdenCollectForDebugging(js_global_context_ref__);
  }
#endif
  
  JSContext::~JSContext() HAL_NOEXCEPT {
    HAL_LOG_TRACE("JSContext:: dtor ", this);
    HAL_LOG_TRACE("JSContext:: release ", js_global_context_ref__, " for ", this);
    JSGlobalContextRelease(js_global_context_ref__);
  }
  
  JSContext::JSContext(const JSContext& rhs) HAL_NOEXCEPT
  : js_context_group__(rhs.js_context_group__)
  , js_global_context_ref__(rhs.js_global_context_ref__) {
    HAL_LOG_TRACE("JSContext:: copy ctor ", this);
    HAL_LOG_TRACE("JSContext:: retain ", js_global_context_ref__, " for ", this);
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
  JSContext::JSContext(JSContext&& rhs) HAL_NOEXCEPT
  : js_context_group__(std::move(rhs.js_context_group__))
  , js_global_context_ref__(rhs.js_global_context_ref__) {
    HAL_LOG_TRACE("JSContext:: move ctor ", this);
    HAL_LOG_TRACE("JSContext:: retain ", js_global_context_ref__, " for ", this);
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
  JSContext& JSContext::operator=(JSContext rhs) HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    HAL_LOG_TRACE("JSContext:: assignment ", this);
    swap(rhs);
    return *this;
  }
  
  void JSContext::swap(JSContext& other) HAL_NOEXCEPT {
    HAL_JSCONTEXT_LOCK_GUARD;
    HAL_LOG_TRACE("JSContext:: swap ", this);
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context_group__     , other.js_context_group__);
    swap(js_global_context_ref__, other.js_global_context_ref__);
  }
  
  JSContext::JSContext(const JSContextGroup& js_context_group, const JSClass& global_object_class) HAL_NOEXCEPT
  : js_context_group__(js_context_group)
  , js_global_context_ref__(JSGlobalContextCreateInGroup(static_cast<JSContextGroupRef>(js_context_group), static_cast<JSClassRef>(global_object_class))) {
    HAL_LOG_TRACE("JSContext:: ctor 1 ", this);
    HAL_LOG_TRACE("JSContext:: retain ", js_global_context_ref__, " (implicit) for ", this);
  }
  
  JSContext::JSContext(JSContextRef js_context_ref) HAL_NOEXCEPT
  : JSContext(JSContextGetGlobalContext(js_context_ref)) {
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSContext::JSContext(JSGlobalContextRef js_global_context_ref) HAL_NOEXCEPT
  : js_context_group__(JSContextGetGroup(js_global_context_ref))
  , js_global_context_ref__(js_global_context_ref) {
    HAL_LOG_TRACE("JSContext:: ctor 2 ", this);
    assert(js_global_context_ref__);
    HAL_LOG_TRACE("JSContext:: retain ", js_global_context_ref__, " for ", this);
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
} // namespace HAL {
