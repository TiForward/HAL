// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSStaticValue.hpp"
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {

/*
// For interoperability with the JavaScriptCore C API.
JSValueRef GetProperty(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name, JSValueRef* exception) {
	JSObject js_object(js_context_ref, js_object_ref);
	return get_property_callback(js_object, property_name);
}

// For interoperability with the JavaScriptCore C API.
bool SetProperty(JSContextRef js_context_ref, JSObjectRef js_object_ref, JSStringRef property_name, JSValueRef js_value_ref, JSValueRef* exception) {
	JSObject js_object(js_context_ref, js_object_ref);
	JSValue  js_value(js_context_ref, js_object_ref);
	return set_property_callback(js_object, property_name, js_value);
}
*/

JSStaticValue::JSStaticValue(const JSString& property_name, JSObjectGetPropertyCallback get_property_callback, JSObjectSetPropertyCallback set_property_callback, const std::set<JSPropertyAttribute> attributes)
		: property_name_(property_name)
		, property_name_for_js_static_value_(property_name)
		, get_property_callback_(get_property_callback)
		, set_property_callback_(set_property_callback)
		, attributes_(attributes) {
	
	static const std::string log_prefix { "MDL: JSStaticValue: " };
	
	if (attributes.find(JSPropertyAttribute::ReadOnly) != attributes.end()) {
		if (!get_property_callback) {
			std::ostringstream os;
			os << "ReadOnly attribute is set but get_property_callback is missing.";
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}

		if (get_property_callback) {
			std::ostringstream os;
			os << "ReadOnly attribute is set but set_property_callback is provided.";
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl;
			throw std::invalid_argument(message);
		}
	}

	if (!get_property_callback && !set_property_callback) {
		std::ostringstream os;
		os << "Both get_property_callback and set_property_callback are missing. At least one callback must be provided.";
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	if (property_name_for_js_static_value_.empty()) {
		std::ostringstream os;
		os << "The property_name is empty. A valid JavaScript property name must be provided.";
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
	
	js_static_value_.name        = property_name_for_js_static_value_.c_str();
	// js_static_value_.getProperty = get_property_callback_;
	// js_static_value_.setProperty = set_property_callback_;
	js_static_value_.attributes   = static_cast<property_attribute_underlying_type>(property_attributes.to_ulong());
}

// Copy constructor.
JSStaticValue::JSStaticValue(const JSStaticValue& rhs)
		: property_name_(rhs.property_name_)
		, property_name_for_js_static_value_(rhs.property_name_)
		, get_property_callback_(rhs.get_property_callback_)
		, set_property_callback_(rhs.set_property_callback_)
		, attributes_(rhs.attributes_)
		, js_static_value_(rhs.js_static_value_) {
	js_static_value_.name = property_name_for_js_static_value_.c_str();
}

// Move constructor.
JSStaticValue::JSStaticValue(JSStaticValue&& rhs)
		: property_name_(rhs.property_name_)
		, property_name_for_js_static_value_(rhs.property_name_)
		, get_property_callback_(rhs.get_property_callback_)
		, set_property_callback_(rhs.set_property_callback_)
		, attributes_(rhs.attributes_)
		, js_static_value_(rhs.js_static_value_) {
	js_static_value_.name = property_name_for_js_static_value_.c_str();
}

void JSStaticValue::swap(JSStaticValue& first, JSStaticValue& second) noexcept {
    // enable ADL (not necessary in our case, but good practice)
    using std::swap;
    
    // by swapping the members of two classes,
    // the two classes are effectively swapped
    swap(first.property_name_                    , second.property_name_);
    swap(first.property_name_for_js_static_value_, second.property_name_for_js_static_value_);
    swap(first.get_property_callback_            , second.get_property_callback_);
    swap(first.set_property_callback_            , second.set_property_callback_);
    swap(first.attributes_                       , second.attributes_);
    swap(first.js_static_value_                  , second.js_static_value_);
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
