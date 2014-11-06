/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSString.hpp"

namespace JavaScriptCoreCPP {

// For interoperability with the JavaScriptCore C API.
JSString::JSString(const JSStringRef& js_string_ref)
		: js_string_ref__(js_string_ref) {
	assert(js_string_ref__);
	JSStringRetain(js_string_ref__);
}

JSString::JSString()
		: js_string_ref__(JSStringCreateWithUTF8CString(nullptr)) {
}

JSString::JSString(const char* string)
		: js_string_ref__(JSStringCreateWithUTF8CString(string)) {
}

const std::size_t JSString::length() const {
	JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
	return JSStringGetLength(js_string_ref__);
}

JSString::operator std::u16string() const {
	JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
	const JSChar* string_ptr = JSStringGetCharactersPtr(js_string_ref__);
	return std::u16string(string_ptr, string_ptr + length());
}

JSString::~JSString() {
	JSStringRelease(js_string_ref__);
}

JSString::JSString(const JSString& rhs)
		: js_string_ref__(rhs.js_string_ref__) {
	JSStringRetain(js_string_ref__);
}

JSString::JSString(JSString&& rhs)
		: js_string_ref__(rhs.js_string_ref__) {
	JSStringRetain(js_string_ref__);
}

JSString& JSString::operator=(const JSString& rhs) {
	JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
	JSStringRelease(js_string_ref__);
	js_string_ref__ = rhs.js_string_ref__;
	JSStringRetain(js_string_ref__);
	return *this;
}

JSString& JSString::operator=(JSString&& rhs) {
	JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
	JSStringRelease(js_string_ref__);
	js_string_ref__ = rhs.js_string_ref__;
	JSStringRetain(js_string_ref__);
	return *this;
}

void JSString::swap(JSString& other) noexcept {
	JAVASCRIPTCORECPP_JSSTRING_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(js_string_ref__, other.js_string_ref__);
}

bool operator==(const JSString& lhs, const JSString& rhs) {
	return JSStringIsEqual(lhs, rhs);
}

} // namespace JavaScriptCoreCPP {
