// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSStaticFunction.hpp"
#include <iterator>
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {

/*
// For interoperability with the JavaScriptCore C API.
JSValueRef CallAsFunction(JSContextRef js_context_ref, JSObjectRef function_ref, JSObjectRef this_object_ref, size_t argument_count, const JSValueRef arguments_array[], JSValueRef* exception) {
	JSContext js_context(js_context_ref);

	JSObject function(js_context, js_context);
	JSObject this_object(js_context, this_object_ref);
	
	std::vector<JSValue> arguments;
	std::transform(arguments_array, arguments_array + argument_count, std::back_inserter(arguments), [&js_context](JSValueRef js_value_ref) { return JSValue(js_context, js_value_ref); });
	
	
	return function(arguments, this_object);
}
*/

JSStaticFunction::JSStaticFunction(const JSString& function_name, JSObjectCallAsFunctionCallback call_as_function_callback, const std::set<JSPropertyAttribute> attributes)
		: function_name_(function_name)
		, function_name_for_js_static_value_(function_name)
		, call_as_function_callback_(call_as_function_callback)
		, attributes_(attributes) {

	static const std::string log_prefix { "MDL: JSStaticFunction: " };
	
	if (function_name_for_js_static_value_.empty()) {
		std::ostringstream os;
		os << "The function_name is empty. A valid JavaScript property name must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}

	using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
	std::bitset<4> property_attributes;
	for (auto property_attribute : attributes) {
		const auto bit_position = static_cast<property_attribute_underlying_type>(property_attribute);
		property_attributes.set(bit_position);
	}
	
	js_static_function_.name           = function_name_for_js_static_value_.empty() ? nullptr : function_name_for_js_static_value_.c_str();
	// js_static_function_.callAsFunction = call_as_function_callback_;
	js_static_function_.attributes     = static_cast<property_attribute_underlying_type>(property_attributes.to_ulong());
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
