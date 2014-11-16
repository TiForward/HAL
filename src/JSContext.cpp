/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSContext.hpp"

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"

#include "JavaScriptCoreCPP/JSValue.hpp"
#include "JavaScriptCoreCPP/JSUndefined.hpp"
#include "JavaScriptCoreCPP/JSNull.hpp"
#include "JavaScriptCoreCPP/JSBoolean.hpp"
#include "JavaScriptCoreCPP/JSNumber.hpp"

#include "JavaScriptCoreCPP/JSObject.hpp"
#include "JavaScriptCoreCPP/JSArray.hpp"
#include "JavaScriptCoreCPP/JSDate.hpp"
#include "JavaScriptCoreCPP/JSError.hpp"
#include "JavaScriptCoreCPP/JSFunction.hpp"
#include "JavaScriptCoreCPP/JSRegExp.hpp"

#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

#include <cassert>

namespace JavaScriptCoreCPP {
  
  JSObject JSContext::get_global_object() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSObject(JSContext(js_global_context_ref__), JSContextGetGlobalObject(js_global_context_ref__));
  }
  
  JSValue JSContext::CreateValueFromJSON(const JSString& js_string) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), js_string, true);
  }
  
  JSValue JSContext::CreateString() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), JSString(), false);
  }
  
  JSValue JSContext::CreateString(const JSString& js_string) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSValue(JSContext(js_global_context_ref__), js_string, false);
  }
  
  JSValue JSContext::CreateString(const char* string) const JAVASCRIPTCORECPP_NOEXCEPT {
    return CreateString(JSString(string));
  }
  
  JSValue JSContext::CreateString(const std::string& string) const JAVASCRIPTCORECPP_NOEXCEPT {
    return CreateString(JSString(string));
  }
  
  JSUndefined JSContext::CreateUndefined() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSUndefined(JSContext(js_global_context_ref__));
  }
  
  JSNull JSContext::CreateNull() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSNull(JSContext(js_global_context_ref__));
  }
  
  JSBoolean JSContext::CreateBoolean(bool boolean) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSBoolean(JSContext(js_global_context_ref__), boolean);
  }
  
  JSNumber JSContext::CreateNumber(double number) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSNumber JSContext::CreateNumber(int32_t number) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSNumber JSContext::CreateNumber(uint32_t number) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSNumber(JSContext(js_global_context_ref__), number);
  }
  
  JSObject JSContext::CreateObject() const JAVASCRIPTCORECPP_NOEXCEPT {
    return CreateObject(JSClass());
  }
  
  JSObject JSContext::CreateObject(const JSClass& js_class) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSObject(JSContext(js_global_context_ref__), js_class);
  }
  
  JSArray JSContext::CreateArray() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSArray(JSContext(js_global_context_ref__));
  }
  
  JSArray JSContext::CreateArray(const std::vector<JSValue>& arguments) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSArray(JSContext(js_global_context_ref__), arguments);
  }
  
  JSDate JSContext::CreateDate() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSDate(JSContext(js_global_context_ref__));
  }
  
  JSDate JSContext::CreateDate(const std::vector<JSValue>& arguments) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSDate(JSContext(js_global_context_ref__), arguments);
  }
  
  JSError JSContext::CreateError() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSError(JSContext(js_global_context_ref__));
  }
  
  JSError JSContext::CreateError(const std::vector<JSValue>& arguments) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSError(JSContext(js_global_context_ref__), arguments);
  }
  
  JSRegExp JSContext::CreateRegExp() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSRegExp(JSContext(js_global_context_ref__));
  }
  
  JSRegExp JSContext::CreateRegExp(const std::vector<JSValue>& arguments) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
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
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSFunction(JSContext(js_global_context_ref__), body, parameter_names, function_name, source_url, starting_line_number);
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script) const {
    return JSEvaluateScript(script, get_global_object(), JSString());
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, const JSString& source_url, int starting_line_number) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    return JSEvaluateScript(script, get_global_object(), source_url, starting_line_number);
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, JSObject this_object) const {
    return JSEvaluateScript(script, this_object, JSString());
  }
  
  JSValue JSContext::JSEvaluateScript(const JSString& script, JSObject this_object, const JSString& source_url, int starting_line_number) const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    JSValueRef js_value_ref { nullptr };
    const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
    JSValueRef exception { nullptr };
    js_value_ref = ::JSEvaluateScript(js_global_context_ref__, script, this_object, source_url_ref, starting_line_number, &exception);
    
    if (exception) {
      // If this assert fails then we need to JSValueUnprotect
      // js_value_ref.
      assert(!js_value_ref);
      detail::ThrowRuntimeError("JSContext", JSValue(JSContext(js_global_context_ref__), exception));
    }
    
    return JSValue(JSContext(js_global_context_ref__), js_value_ref);
  }
  
  bool JSContext::JSCheckScriptSyntax(const JSString& script) const JAVASCRIPTCORECPP_NOEXCEPT {
    return JSCheckScriptSyntax(script, JSString());
  }
  
  bool JSContext::JSCheckScriptSyntax(const JSString& script, const JSString& source_url, int starting_line_number) const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
    JSValueRef exception { nullptr };
    bool result = ::JSCheckScriptSyntax(js_global_context_ref__, script, source_url_ref, starting_line_number, &exception);
    
    if (exception) {
      detail::ThrowRuntimeError("JSContext", JSValue(JSContext(js_global_context_ref__), exception));
    }
    
    return result;
  }
  
  void JSContext::GarbageCollect() const JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    JSGarbageCollect(js_global_context_ref__);
  }
  
#ifdef DEBUG
  extern "C" void JSSynchronousGarbageCollectForDebugging(JSContextRef);
  
  void JSContext::SynchronousGarbageCollectForDebugging() const {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    JSSynchronousGarbageCollectForDebugging(js_global_context_ref__);
  }
#endif
  
  JSContext::~JSContext() JAVASCRIPTCORECPP_NOEXCEPT {
    JSGlobalContextRelease(const_cast<JSGlobalContextRef>(js_global_context_ref__));
  }
  
  JSContext::JSContext(const JSContext& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group__(rhs.js_context_group__)
  , js_global_context_ref__(rhs.js_global_context_ref__) {
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
  JSContext::JSContext(JSContext&& rhs) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group__(std::move(rhs.js_context_group__))
  , js_global_context_ref__(rhs.js_global_context_ref__) {
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
  JSContext& JSContext::operator=(const JSContext& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    JSGlobalContextRelease(js_global_context_ref__);
    js_context_group__      = rhs.js_context_group__;
    js_global_context_ref__ = rhs.js_global_context_ref__;
    JSGlobalContextRetain(js_global_context_ref__);
    return *this;
  }
  
  JSContext& JSContext::operator=(JSContext&& rhs) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    JSGlobalContextRetain(js_global_context_ref__);
    return *this;
  }
  
  void JSContext::swap(JSContext& other) JAVASCRIPTCORECPP_NOEXCEPT {
    JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
    using std::swap;
    
    // By swapping the members of two classes, the two classes are
    // effectively swapped.
    swap(js_context_group__     , other.js_context_group__);
    swap(js_global_context_ref__, other.js_global_context_ref__);
  }
  
  JSContext::JSContext(const JSContextGroup& js_context_group, const JSClass& global_object_class) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group__(js_context_group)
  , js_global_context_ref__(JSGlobalContextCreateInGroup(js_context_group, global_object_class)) {
  }
  
  JSContext::JSContext(JSContextRef js_context_ref) JAVASCRIPTCORECPP_NOEXCEPT
  : JSContext(const_cast<JSGlobalContextRef>(js_context_ref)) {
  }
  
  // For interoperability with the JavaScriptCore C API.
  JSContext::JSContext(JSGlobalContextRef js_global_context_ref) JAVASCRIPTCORECPP_NOEXCEPT
  : js_context_group__(JSContextGetGroup(js_global_context_ref))
  , js_global_context_ref__(js_global_context_ref) {
    assert(js_global_context_ref__);
    JSGlobalContextRetain(js_global_context_ref__);
  }
  
} // namespace JavaScriptCoreCPP {
