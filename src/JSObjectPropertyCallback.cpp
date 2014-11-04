/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSObjectPropertyCallback.hpp"
#include "JavaScriptCoreCPP/detail/JSUtil.hpp"

namespace JavaScriptCoreCPP {

JSObjectPropertyCallback::JSObjectPropertyCallback(const JSString& property_name, const std::unordered_set<JSObjectPropertyAttribute>& attributes)
		: property_name__(property_name)
		, attributes__(attributes) {
	
	if (property_name__.empty()) {
		ThrowInvalidArgument("JSObjectPropertyCallback", "property_name is missing");
	}
}

JSObjectPropertyCallback::(const JSObjectPropertyCallback& rhs)
: property_name__(rhs.property_name__)
		, attributes__(rhs.attributes__) {
}

JSObjectPropertyCallback::(JSObjectPropertyCallback&& rhs)
: property_name__(std::move(rhs.property_name__))
		, attributes__(std::move(rhs.attributes__)) {
}

// Create a copy of another JSObjectPropertyCallback by
// assignment. This is a unified assignment operator that fuses the
// copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSObjectPropertyCallback& JSObjectPropertyCallback::operator=(JSObjectPropertyCallback rhs) {
	JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD;
	swap(rhs);
	return *this;
}

void JSNativeObjectFunctionPropertyCallback::swap(JSNativeObjectFunctionPropertyCallback& other) noexcept {
	JAVASCRIPTCORECPP_JSOBJECTPROPERTYCALLBACK_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(property_name__, other.property_name__);
	swap(attributes__   , other.attributes__);
}

} // namespace JavaScriptCoreCPP
