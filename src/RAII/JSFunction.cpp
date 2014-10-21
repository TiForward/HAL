// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSFunction.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

JSFunction::JSFunction(const JSString& name, const std::vector<JSString>& parameter_names, const JSString& body, const JSContext& js_context, const JSString& source_url, int starting_line_number) : JSObject(js_context) {
	JSValueRef exception { nullptr };
	const JSStringRef source_url_ref = (source_url.length() > 0) ? static_cast<JSStringRef>(source_url) : nullptr;
	JSObjectRef js_object_ref = nullptr;
	if (!parameter_names.empty()) {
		std::vector<JSStringRef> parameter_name_array;
		std::transform(parameter_names.begin(), parameter_names.end(), std::back_inserter(parameter_name_array), [](const JSString& js_string) { return static_cast<JSStringRef>(js_string); });
		js_object_ref = JSObjectMakeFunction(js_context, name, static_cast<unsigned>(parameter_name_array.size()), &parameter_name_array[0], body, source_url_ref, starting_line_number, &exception);
	} else {
		js_object_ref = JSObjectMakeFunction(js_context, name, 0, nullptr, body, source_url_ref, starting_line_number, &exception);
	}
	
	if (exception) {
		// assert(!js_object_ref);
		static const std::string log_prefix { "MDL: JSFunction: " };
		const std::string message = static_cast<std::string>(JSValue(exception, js_context));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	JSValueUnprotect(js_context, js_object_ref_);
	js_object_ref_ = js_object_ref;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
