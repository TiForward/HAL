/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/JSError.hpp"
#include "HAL/JSValue.hpp"
#include "HAL/detail/JSUtil.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace HAL {

JSError::JSError(const JSContext& js_context, const std::vector<JSValue>& arguments)
		: JSObject(js_context, MakeError(js_context, arguments), false) {
}

JSObjectRef JSError::MakeError(const JSContext& js_context, const std::vector<JSValue>& arguments) {
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array = detail::to_vector(arguments);
		js_object_ref = JSObjectMakeError(static_cast<JSContextRef>(js_context), arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectMakeError(static_cast<JSContextRef>(js_context), 0, nullptr, &exception);
	}
	
	if (exception) {
		// If this assert fails then we need to JSValueUnprotect
		// js_object_ref.
		assert(!js_object_ref);
		detail::ThrowRuntimeError("JSError", JSValue(js_context, exception));
	}

	return js_object_ref;
}

} // namespace HAL {
