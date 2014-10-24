/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/RAII/JSDate.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

JSDate::JSDate(const JSContext& js_context, const std::vector<JSValue>& arguments) : JSObject(js_context) {
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_object_ref = JSObjectMakeDate(js_context, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectMakeDate(js_context, 0, nullptr, &exception);
	}
	
	if (exception) {
		// assert(!js_object_ref);
		static const std::string log_prefix { "MDL: JSDate(const std::vector<Value>& arguments, const JSContext& js_context): " };
		const std::string message = static_cast<std::string>(JSValue(js_context, exception));
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	JSValueUnprotect(js_context, js_object_ref_);
	js_object_ref_ = js_object_ref;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
