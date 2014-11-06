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

namespace JavaScriptCoreCPP {

JSClass& JSClass::EmptyJSClass() {
	static std::once_flag of;
	static JSClass        empty_js_class;
	std::call_once(of, [] {
			empty_js_class.js_class_pimpl_ptr__ = std::make_shared<detail::JSClassPimpl>();
		});
	
	return empty_js_class;
}

JSClass::JSClass(std::shared_ptr<detail::JSClassPimpl> js_class_pimpl_ptr)
		: js_class_pimpl_ptr__(js_class_pimpl_ptr) {
}

std::string JSClass::get_name() const {
	return js_class_pimpl_ptr__ -> get_name();
}
  
std::uint32_t JSClass::get_version() const {
	return js_class_pimpl_ptr__ -> get_version();
}

JSClass::operator JSClassRef() const {
	return js_class_pimpl_ptr__ -> operator JSClassRef();
}

} // namespace JavaScriptCoreCPP {
