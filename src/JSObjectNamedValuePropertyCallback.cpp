/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSObjectNamedValuePropertyCallback.hpp"

namespace JavaScriptCoreCPP {

JSObjectNamedValuePropertyCallback::JSObjectNamedValuePropertyCallback(
    const JSString& property_name,
    GetNamedPropertyCallback get_callback,
    SetNamedPropertyCallback set_callback,
    const std::unordered_set<JSObjectPropertyAttribute>& attributes)
		: JSObjectPropertyCallback(property_name, attributes)
		, get_callback__(get_callback)
		, set_callback__(set_callback) {
	
	if (!get_callback && !set_callback) {
		ThrowInvalidArgument("JSObjectNamedValuePropertyCallback", "Both get_callback and set_callback are missing. At least one callback must be provided");
	}
	
	if (attributes.find(JSObjectPropertyAttribute::ReadOnly) != attributes.end()) {
		if (!get_callback) {
			ThrowInvalidArgument("JSObjectNamedValuePropertyCallback", "ReadOnly attribute is set but get_callback is missing");
		}
		
		if (set_callback) {
			ThrowInvalidArgument("JSObjectNamedValuePropertyCallback", "ReadOnly attribute is set but set_callback is provided");
		}
	}
	
	// Force the ReadOnly attribute if only the get_callback is
	// provided.
	if (get_callback && !set_callback) {
		attributes__.insert(JSObjectPropertyAttribute::ReadOnly);
	}
}

JSObjectNamedValuePropertyCallback::JSObjectNamedValuePropertyCallback(const JSObjectNamedValuePropertyCallback& rhs)
		: JSObjectPropertyCallback(rhs)
		, get_callback__(rhs.get_callback__)
		, set_callback__(rhs.set_callback__) {
}

JSObjectNamedValuePropertyCallback::JSObjectNamedValuePropertyCallback(JSObjectNamedValuePropertyCallback&& rhs)
		: JSObjectPropertyCallback(rhs)
		, get_callback__(std::move(rhs.get_callback__))
		, set_callback__(std::move(rhs.set_callback__)) {
}

// Create a copy of another JSObjectFunctionPropertyCallback by
// assignment. This is a unified assignment operator that fuses the
// copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSObjectNamedValuePropertyCallback& JSObjectNamedValuePropertyCallback::operator=(JSObjectNamedValuePropertyCallback rhs) {
	JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD;
	JSObjectPropertyCallback::operator=(rhs);
	swap(rhs);
	return *this;
}

void JSObjectNamedValuePropertyCallback::swap(JSNativeObjectFunctionPropertyCallback& other) noexcept {
	JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(get_callback__, other.get_callback__);
	swap(set_callback__, other.set_callback__);
}

bool JSObjectNamedValuePropertyCallback::operator==(const JSObjectNamedValuePropertyCallback& lhs, const JSObjectNamedValuePropertyCallback& rhs) {
	// get_callback__
	if (lhs.get_callback__ && !rhs.get_callback__) {
		return false;
	}
	
	if (!lhs.get_callback__ && rhs.get_callback__) {
		return false;
	}
	
	// set_callback__
	if (lhs.set_callback__ && !rhs.set_callback__) {
		return false;
	}
	
	if (!lhs.set_callback__ && rhs.set_callback__) {
		return false;
	}
	
	return static_cast<JSObjectPropertyCallback>(lhs) == static_cast<JSObjectPropertyCallback>(rhs);
}

} // namespace JavaScriptCoreCPP {
