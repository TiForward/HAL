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

JSContext::JSContext(const JSContextGroup& js_context_group, const JSClass& global_object_class)
		: js_context_group__(js_context_group)
		, js_context_ref__(JSGlobalContextCreateInGroup(js_context_group, global_object_class)) {
	JSGlobalContextRetain(*this);
}

// For interoperability with the JavaScriptCore C API.
JSContext::JSContext(JSContextRef js_context_ref)
		: js_context_group__(JSContextGetGroup(js_context_ref))
		, js_context_ref__(js_context_ref) {
	JSGlobalContextRetain(*this);
}


JSContext::~JSContext() {
	JSGlobalContextRelease(*this);
}
	
JSContext::JSContext(const JSContext& rhs)
		: js_context_group__(rhs.js_context_group__)
		, js_context_ref__(rhs.js_context_ref__) {
	JSGlobalContextRetain(*this);
}
	
JSContext::JSContext(JSContext&& rhs)
		: js_context_group__(std::move(rhs.js_context_group__))
		, js_context_ref__(rhs.js_context_ref__) {
	JSGlobalContextRetain(*this);
}
	
JSContext& JSContext::operator=(const JSContext& rhs) {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	
	JSGlobalContextRelease(*this);
	js_context_group__ = rhs.js_context_group__;
	js_context_ref__   = rhs.js_context_ref__;
	JSGlobalContextRetain(*this);
	return *this;
}

JSContext& JSContext::operator=(JSContext&& rhs) {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	
	JSGlobalContextRelease(*this);
	js_context_group__ = std::move(rhs.js_context_group__);
	js_context_ref__   = rhs.js_context_ref__;
	JSGlobalContextRetain(*this);
	return *this;
}

void JSContext::swap(JSContext& other) noexcept {
	  JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	  using std::swap;
    
	  // By swapping the members of two classes, the two classes are
	  // effectively swapped.
	  swap(js_context_group__, other.js_context_group__);
	  swap(js_context_ref__  , other.js_context_ref__);
}

JSContext::operator JSGlobalContextRef() const {
	//return JSContextGetGlobalContext(js_context_ref__);
	return const_cast<JSGlobalContextRef>(js_context_ref__);
}

JSObject JSContext::get_global_object() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSObject(*this, JSContextGetGlobalObject(*this));
}

JSValue JSContext::CreateValueFromJSON(const JSString& js_string) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSValue(*this, js_string, true);
}

JSValue JSContext::CreateString() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSValue(*this, JSString(), false);
}

JSValue JSContext::CreateString(const JSString& js_string) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSValue(*this, js_string, false);
}

JSValue JSContext::CreateString(const char* string) const {
	return CreateString(JSString(string));
}

JSValue JSContext::CreateString(const std::string& string) const {
	return CreateString(JSString(string));
}

JSUndefined JSContext::CreateUndefined() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSUndefined(*this);
}

JSNull JSContext::CreateNull() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSNull(*this);
}

JSBoolean JSContext::CreateBoolean(bool boolean) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSBoolean(*this, boolean);
}

JSNumber JSContext::CreateNumber(double number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSNumber(*this, number);
}

JSNumber JSContext::CreateNumber(int32_t number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSNumber(*this, number);
}

JSNumber JSContext::CreateNumber(uint32_t number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSNumber(*this, number);
}

JSObject JSContext::CreateObject() const {
	return CreateObject(JSClass());
}

JSObject JSContext::CreateObject(const JSClass& js_class) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSObject(*this, js_class);
}

JSArray JSContext::CreateArray() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSArray(*this);
}

JSArray JSContext::CreateArray(const std::vector<JSValue>& arguments) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSArray(*this, arguments);
}

JSDate JSContext::CreateDate() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSDate(*this);
}

JSDate JSContext::CreateDate(const std::vector<JSValue>& arguments) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSDate(*this, arguments);
}

JSError JSContext::CreateError() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSError(*this);
}

JSError JSContext::CreateError(const std::vector<JSValue>& arguments) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSError(*this, arguments);
}

JSRegExp JSContext::CreateRegExp() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSRegExp(*this);
}

JSRegExp JSContext::CreateRegExp(const std::vector<JSValue>& arguments) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	return JSRegExp(*this, arguments);
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
	return JSFunction(*this, body, parameter_names, function_name, source_url, starting_line_number);
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
	js_value_ref = ::JSEvaluateScript(js_context_ref__, script, this_object, source_url_ref, starting_line_number, &exception);
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_value_ref.
		assert(!js_value_ref);
		detail::ThrowRuntimeError("JSContext", JSValue(*this, exception));
	}

	return JSValue(*this, js_value_ref);
}

bool JSContext::JSCheckScriptSyntax(const JSString& script) const {
	return JSCheckScriptSyntax(script, JSString());
}

bool JSContext::JSCheckScriptSyntax(const JSString& script, const JSString& source_url, int starting_line_number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSValueRef exception { nullptr };
	bool result = ::JSCheckScriptSyntax(js_context_ref__, script, source_url_ref, starting_line_number, &exception);
	
	if (exception) {
		detail::ThrowRuntimeError("JSContext", JSValue(*this, exception));
	}

	return result;
}

void JSContext::GarbageCollect() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	JSGarbageCollect(js_context_ref__);
}

#ifdef DEBUG
extern "C" void JSSynchronousGarbageCollectForDebugging(JSContextRef);

void JSContext::SynchronousGarbageCollectForDebugging() const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	JSSynchronousGarbageCollectForDebugging(js_context_ref__);
}
#endif

} // namespace JavaScriptCoreCPP {
