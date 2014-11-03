/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSArray.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace JavaScriptCoreCPP {

JSArray::JSArray(const JSContext& js_context, const std::vector<JSValue>& arguments)
		: JSObject(js_context, MakeArray(js_context, arguments)) {
}

JSObjectRef JSArray::MakeArray(const JSContext& js_context, const std::vector<JSValue>& arguments) {
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array = detail::ToJSValueRefVector(arguments);
		js_object_ref = JSObjectMakeArray(js_context, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectMakeArray(js_context, 0, nullptr, &exception);
	}
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_object_ref.
		assert(!js_object_ref);
		detail::ThrowRuntimeError("JSArray", JSValue(js_context, exception));
	}

	return js_object_ref;
}

} // namespace JavaScriptCoreCPP {
