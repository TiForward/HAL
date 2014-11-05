/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSContextGroup.hpp"
#include "JavaScriptCoreCPP/JSContext.hpp"
#include <cassert>
#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

JSContextGroup::JSContextGroup()
		: js_context_group_ref__(JSContextGroupCreate()) {
}

JSContext JSContextGroup::CreateContext(const JSClass& global_object_class) const {
	return JSContext(*this, global_object_class);
}

// For interoperability with the JavaScriptCore C API.
JSContextGroup::JSContextGroup(JSContextGroupRef js_context_group_ref)
		: js_context_group_ref__(js_context_group_ref) {
	assert(js_context_group_ref__);
	JSContextGroupRetain(js_context_group_ref__);
}

JSContextGroup::~JSContextGroup() {
	JSContextGroupRelease(js_context_group_ref__);
}
	
JSContextGroup::JSContextGroup(const JSContextGroup& rhs)
		: js_context_group_ref__(rhs.js_context_group_ref__) {
	JSContextGroupRetain(js_context_group_ref__);
}
	
JSContextGroup::JSContextGroup(JSContextGroup&& rhs)
		: js_context_group_ref__(rhs.js_context_group_ref__) {
	JSContextGroupRetain(js_context_group_ref__);
}

// Create a copy of another JSContextGroup by assignment. This is a
// unified assignment operator that fuses the copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSContextGroup& JSContextGroup::operator=(JSContextGroup rhs) {
	JAVASCRIPTCORECPP_JSCONTEXTGROUP_LOCK_GUARD;
	swap(rhs);
	return *this;
}
  
JSContextGroup::operator JSContextGroupRef() const {
	return js_context_group_ref__;
}

void swap(JSContextGroup& other) noexcept {
	JAVASCRIPTCORECPP_JSCONTEXTGROUP_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(js_context_group_ref__, other.js_context_group_ref__);
}

} // namespace JavaScriptCoreCPP {
