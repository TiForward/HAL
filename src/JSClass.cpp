/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/JSLogger.hpp"

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

JSClass::JSClass(const JSClassDefinition& js_class_definition)
		: js_class_ref__(JSClassCreate(&js_class_definition)) {
	if (js_class_definition.name) {
		name__    = js_class_definition.name;
		version__ = js_class_definition.version;
	}
}

virtual ~JSClass::JSClass() {
	JSClassRelease(js_class_ref__);
}

JSClass::JSClass(const JSClass& rhs)
		: js_class_ref__(rhs.js_class_ref__) {
	JSClassRetain(js_class_ref__);
}

JSClass::JSClass(JSClass&& rhs)
		: js_class_ref__(rhs.js_class_ref__) {
	JSClassRetain(js_class_ref__);
}

// Create a copy of another JSClass by assignment. This is a unified
// assignment operator that fuses the copy assignment operator
//
// X& X::operator=(const X&)
//
// and the move assignment operator
//
// X& X::operator=(X&&);
JSClass& JSClass::operator=(JSClass rhs) {
	JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
	swap(rhs);
	return *this;
}

void JSClass::swap(JSClass& other) noexcept {
	JAVASCRIPTCORECPP_JSCLASS_LOCK_GUARD;
	using std::swap;
	
	// By swapping the members of two classes, the two classes are
	// effectively swapped.
	swap(js_class_ref__, other.js_class_ref__);
	swap(name__        , other.name__);
	swap(version__     , other.version__);
}

JSClass::empty_js_class_;
JSClass::js_class_map_;

#ifdef  JAVASCRIPTCORECPP_THREAD_SAFE
std::recursive_mutex JSClass::static_mutex__;
#endif  // JAVASCRIPTCORECPP_THREAD_SAFE

static JSClass EmptyJSClass() {
	static std::once_flag of;
	std::call_once(of, [] {
			JSClassDefinition js_class_definition = kJSClassDefinitionEmpty;
			empty_js_class_ = JSClass(&js_class_definition);
		});
}

void JSClass::ThrowRuntimeErrorIfJSClassAlreadyExists(const JSString& js_class_name) {
	JAVASCRIPTCORECPP_JSCLASS_STATIC_LOCK_GUARD;
	
	const auto position = js_class_map_.find(js_class_name);
	const bool found    = position != js_class_map_.end();
	
	if (found) {
		const std::string message = "A JSClass with class name " + static_cast<std::string>(js_class_name) + " already exists.";
		JAVASCRIPTCORECPP_LOG_ERROR(message);
		ThrowRuntimeError("JSClass", message);
	}
}

} // namespace JavaScriptCoreCPP {
