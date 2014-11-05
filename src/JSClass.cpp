/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JSClass.hpp"
#include "JavaScriptCoreCPP/detail/JSClassPimpl.hpp"
#include "JavaScriptCoreCPP/detail/JSClassBuilder.hpp"

#include <JavaScriptCore/JavaScript.h>

namespace JavaScriptCoreCPP {

static JSClass& DefaultJSClass() {
	static std::once_flag of;
	std::call_once(of, [] {
			empty_js_class__ = detail::JSClassBuilder("Default").build();
		});

	return empty_js_class__;
}

JSClass::JSClass(std::shared<detail::JSClassPimpl> js_class_pimpl_ptr)
		: js_class_pimpl_ptr__(js_class_pimpl_ptr) {
}

std::string JSClass::get_name() {
	return js_class_pimpl_ptr__ -> get_name();
}
  
std::uint32_t get_version() {
	return js_class_pimpl_ptr__ -> get_version();
}

JSClass::operator JSClassRef() const {
	return js_class_pimpl_ptr__ -> operator JSClassRef();
}

} // namespace JavaScriptCoreCPP {


// void JSClass::ThrowRuntimeErrorIfJSClassAlreadyExists(const JSString& js_class_name) {
// 	JAVASCRIPTCORECPP_JSCLASS_STATIC_LOCK_GUARD;
	
// 	const auto position = js_class_map_.find(js_class_name);
// 	const bool found    = position != js_class_map_.end();
	
// 	if (found) {
// 		const std::string message = "A JSClass with class name " + static_cast<std::string>(js_class_name) + " already exists.";
// 		JAVASCRIPTCORECPP_LOG_ERROR(message);
// 		ThrowRuntimeError("JSClass", message);
// 	}
// }

