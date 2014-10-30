/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSContext.hpp"

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

#include <vector>
#include <algorithm>
#include <stdexcept>
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

JSValue JSContext::CreateValueFromJSON(const JSString& js_string) const {
	return JSValue(*this, js_string, true);
}

JSValue JSContext::CreateString() const {
	return CreateString(JSString());
}

JSValue JSContext::CreateString(const JSString& js_string) const {
	return JSValue(*this, js_string, false);
}

JSValue JSContext::CreateString(const char* string) const {
	return CreateString(JSString(string));
}

JSValue JSContext::CreateString(const std::string& string) const {
	return CreateString(JSString(string));
}

JSUndefined JSContext::CreateUndefined() const {
	return JSUndefined(*this);
}

JSNull JSContext::CreateNull() const {
	return JSNull(*this);
}

JSBoolean JSContext::CreateBoolean(bool boolean) const {
	return JSBoolean(*this, boolean);
}

JSNumber JSContext::CreateNumber() const {
	return JSNumber(*this);
}

JSNumber JSContext::CreateNumber(double number) const {
	return JSNumber(*this, number);
}

JSNumber JSContext::CreateNumber(int32_t number) const {
	return JSNumber(*this, number);
}

JSNumber JSContext::CreateNumber(uint32_t number) const {
	return JSNumber(*this, number);
}

JSObject JSContext::CreateObject() const {
	return JSObject(*this);
}
	
JSObject JSContext::CreateObject(const JSClass& js_class, void* private_data) const {
	return JSObject(*this, js_class, private_data);
}

JSArray JSContext::CreateArray() const {
	return JSArray(*this);
}
	
JSArray JSContext::CreateArray(const std::vector<JSValue>& arguments) const {
	return JSArray(*this, arguments);
}

JSDate JSContext::CreateDate() const {
	return JSDate(*this);
}
	
JSDate JSContext::CreateDate(const std::vector<JSValue>& arguments) const {
	return JSDate(*this, arguments);
}

JSError JSContext::CreateError() const {
	return JSError(*this);
}
	
JSError JSContext::CreateError(const std::vector<JSValue>& arguments) const {
	return JSError(*this, arguments);
}

JSRegExp JSContext::CreateRegExp() const {
	return JSRegExp(*this);
}
	
JSRegExp JSContext::CreateRegExp(const std::vector<JSValue>& arguments) const {
	return JSRegExp(*this, arguments);
}

JSFunction JSContext::CreateFunction(const JSString& function_name, const std::vector<JSString>& parameter_names, const JSString& body, const JSString& source_url, int starting_line_number) const {
	return JSFunction(*this, function_name, parameter_names, body, source_url, starting_line_number);
}

JSValue JSContext::JSEvaluateScript(const JSString& script, const JSString& source_url, int starting_line_number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	JSValueRef js_value_ref { nullptr };
	const JSObjectRef this_object { nullptr };
	const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSValueRef exception { nullptr };
	js_value_ref = ::JSEvaluateScript(js_context_ref__, script, this_object, source_url_ref, starting_line_number, &exception);
	
	if (exception) {
		// assert(!js_object_ref);
		static const std::string log_prefix { "MDL: JSEvaluateScript: " };
		const std::string message = static_cast<std::string>(JSValue(*this, exception));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}

	JSValue result(*this, js_value_ref);
	return result;
}

JSValue JSContext::JSEvaluateScript(const JSString& script, const JSObject& this_object, const JSString& source_url, int starting_line_number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	JSValueRef js_value_ref { nullptr };
	const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSValueRef exception { nullptr };
	js_value_ref = ::JSEvaluateScript(js_context_ref__, script, this_object, source_url_ref, starting_line_number, &exception);
	
	if (exception) {
		// assert(!js_object_ref);
		static const std::string log_prefix { "MDL: JSEvaluateScript: " };
		const std::string message = static_cast<std::string>(JSValue(*this, exception));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}

	JSValue result(*this, js_value_ref);
	return result;
}

bool JSContext::JSCheckScriptSyntax(const JSString& script, const JSString& source_url, int starting_line_number) const {
	JAVASCRIPTCORECPP_JSCONTEXT_LOCK_GUARD;
	const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSValueRef exception { nullptr };
	bool result = ::JSCheckScriptSyntax(js_context_ref__, script, source_url_ref, starting_line_number, &exception);
	
	if (exception) {
		// assert(!js_object_ref);
		static const std::string log_prefix { "MDL: JSCheckScriptSyntax: " };
		const std::string message = static_cast<std::string>(JSValue(*this, exception));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}

	return result;
}

JSObject JSContext::get_global_object() const {
	return JSObject(*this, JSContextGetGlobalObject(*this));
}


#ifdef JAVASCRIPTCORECPP_JSCONTEXT_ENABLE_CONTEXT_ID
// Definition of class static memner;
std::atomic<long> JSContext::js_context_id_counter_;
#endif

} // namespace JavaScriptCoreCPP {
