/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSFunction.hpp"
#include "JavaScriptCoreCPP/JSString.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace JavaScriptCoreCPP {

JSFunction::JSFunction(const JSContext& js_context, const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name, const JSString& source_url, int starting_line_number)
		: JSObject(js_context, MakeFunction(js_context, body, parameter_names, function_name, source_url, starting_line_number)) {
}

JSObjectRef JSFunction::MakeFunction(const JSContext& js_context, const JSString& body, const std::vector<JSString>& parameter_names, const JSString& function_name, const JSString& source_url, int starting_line_number) {
	JSValueRef exception { nullptr };
	JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSObjectRef js_object_ref = nullptr;
	if (!parameter_names.empty()) {
		std::vector<JSStringRef> parameter_name_array = detail::to_vector(parameter_names);
		js_object_ref = JSObjectMakeFunction(js_context, function_name, static_cast<unsigned>(parameter_name_array.size()), &parameter_name_array[0], body, source_url_ref, starting_line_number, &exception);
	} else {
		js_object_ref = JSObjectMakeFunction(js_context, function_name, 0, nullptr, body, source_url_ref, starting_line_number, &exception);
	}
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_object_ref.
		assert(!js_object_ref);
		detail::ThrowRuntimeError("JSFunction", JSValue(js_context, exception));
	}

	return js_object_ref;
}

} // namespace JavaScriptCoreCPP {
