/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSObjectValuePropertyCallback.hpp"

namespace JavaScriptCoreCPP {

JSObjectNamedFunctionPropertyCallback::JSObjectNamedFunctionPropertyCallback(
    const JSString& function_name,
    CallNamedFunctionCallback function_callback,
    const std::unordered_set<JSObjectPropertyAttribute>& attributes)
		: JSObjectPropertyCallback(property_name, attributes)
		, function_callback__(function_callback) {
	
	if (!function_callback) {
		ThrowInvalidArgument("JSObjectNamedFunctionPropertyCallback", "function_callback is missing");
	}
}

JSObjectNamedFunctionPropertyCallback::~JSObjectNamedFunctionPropertyCallback() {
}

JSObjectNamedFunctionPropertyCallback::JSObjectNamedFunctionPropertyCallback(const JSObjectNamedFunctionPropertyCallback& rhs)
		: JSObjectPropertyCallback(rhs)
		, function_callback__(rhs.function_callback__) {
}

JSObjectNamedFunctionPropertyCallback::JSObjectNamedFunctionPropertyCallback(JSObjectNamedFunctionPropertyCallback&& rhs)
		: JSObjectPropertyCallback(rhs)
		, function_callback__(std::move(rhs.function_callback__)) {
}

// Create a copy of another JSObjectNamedFunctionPropertyCallback by
// assignment. This is a unified assignment operator that fuses the
// copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSObjectNamedFunctionPropertyCallback& JSObjectNamedFunctionPropertyCallback::operator=(JSObjectNamedFunctionPropertyCallback rhs) {
	JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD;
	JSObjectPropertyCallback::operator=(rhs);
	swap(rhs);
	return *this;
}

void JSObjectNamedFunctionPropertyCallback::swap(JSObjectNamedFunctionPropertyCallback& other) noexcept {
	JAVASCRIPTCORECPP_JSOBJECTVALUEPROPERTYCALLBACK_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(function_callback__, other.function_callback__);
}

bool JSObjectNamedFunctionPropertyCallback::operator==(const JSObjectNamedFunctionPropertyCallback& lhs, const JSObjectNamedFunctionPropertyCallback& rhs) {
	if (lhs.function_callback__ && !rhs.function_callback__) {
		return false;
	}
	
	if (!lhs.function_callback__ && rhs.function_callback__) {
		return false;
	}
	
	return static_cast<JSObjectPropertyCallback>(lhs) == static_cast<JSObjectPropertyCallback>(rhs);
}

} // namespace JavaScriptCoreCPP {
