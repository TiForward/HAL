// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <bitset>
#include <type_traits>
#include <sstream>
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyNameAccumulator.hpp"
#include "JSPropertyNameArray.hpp"

namespace JavaScriptCoreCPP { namespace RAII {

JSObject::JSObject(const JSClass& js_class, const JSContext& js_context, void* private_data) : JSObject(js_context, JSObjectMake(js_context, js_class, private_data)) {
}

// For interoperability with the JavaScriptCore C API.
JSObject::JSObject(JSGlobalContextRef js_context_ref, JSObjectRef js_object_ref)
		: JSValue(js_context_ref, js_object_ref)
		, js_context_(js_context_ref)
		, js_object_ref_(js_object_ref) {

	static const std::string log_prefix { "MDL: JSObject(JSObjectRef js_object_ref, const JSContext& js_context): " };

	if (!js_object_ref_) {
		const std::string message = "js_object_ref can not be nullptr.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	JSValueProtect(js_context_, js_object_ref_);
}

std::vector<JSString> JSObject::GetPropertyNames() const {
	return JSPropertyNameArray(*this);
}

std::unordered_map<JSString, JSValue> JSObject::GetProperties() const {
	auto property_names = GetPropertyNames();
	std::unordered_map<JSString, JSValue> properties(property_names.size());
	for (const auto& property_name : property_names) {
		properties.emplace(property_name, GetProperty(property_name));
	}
	
	return properties;
}

JSValue JSObject::GetProperty(const JSString& property_name) const {
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetProperty(js_context_, js_object_ref_, property_name, &exception);
	if (exception) {
		// assert(!js_value_ref);
		static const std::string log_prefix { "MDL: JSObject::GetProperty: " };
		std::ostringstream os;
		os << "Caught exception getting property with name \"" << property_name << "\": " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
	}
	
	JSValue js_value(js_context_, js_value_ref);
	JSValueUnprotect(js_value, js_value_ref);
	
	return js_value;
}

JSValue JSObject::GetPropertyAtIndex(unsigned property_index) const {
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(js_context_, js_object_ref_, property_index, &exception);
	if (exception) {
		// assert(!js_value_ref);
		static const std::string log_prefix { "MDL: JSObject::GetPropertyAtIndex: " };
		std::ostringstream os;
		os << "Caught exception getting property at index \"" << property_index << "\": " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
	}
	
	JSValue js_value(js_context_, js_value_ref);
	JSValueUnprotect(js_value, js_value_ref);
	
	return js_value;
}

void JSObject::SetProperty(const JSString& property_name, const JSValue& property_value, const std::set<JSPropertyAttribute> attributes) {
	using property_attribute_underlying_type = std::underlying_type<JSPropertyAttribute>::type;
	std::bitset<4> property_attributes;
	for (auto property_attribute : attributes) {
		const auto bit_position = static_cast<property_attribute_underlying_type>(property_attribute);
		property_attributes.set(bit_position);
	}
	JSValueRef exception { nullptr };
	JSObjectSetProperty(js_context_, js_object_ref_, property_name, property_value, static_cast<property_attribute_underlying_type>(property_attributes.to_ulong()), &exception);
	if (exception) {
		// assert(!js_value_ref);
		static const std::string log_prefix { "MDL: JSObject::SetProperty: " };
		std::ostringstream os;
		os << "Caught exception setting property with name \""
		   << property_name
		   << "\" to value \""
		   << property_value
		   << "\": "
		   << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
	}
}

void JSObject::SetPropertyAtIndex(unsigned property_index, const JSValue& property_value) {
	JSValueRef exception { nullptr };
	JSObjectSetPropertyAtIndex(js_context_, js_object_ref_, property_index, property_value, &exception);
	if (exception) {
		// assert(!js_value_ref);
		static const std::string log_prefix { "MDL: JSObject::SetPropertyAtIndex: " };
		std::ostringstream os;
		os << "Caught exception setting property with index \""
		   << property_index
		   << "\" to value \""
		   << property_value
		   << "\": "
		   << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
	}
}

bool JSObject::DeleteProperty(const JSString& property_name) const {
	JSValueRef exception { nullptr };
	const bool result = JSObjectDeleteProperty(js_context_, js_object_ref_, property_name, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: JSObject::DeleteProperty: " };
		std::ostringstream os;
		os << "Caught exception deleting property with name \"" << property_name << "\": " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [WARN] " << message << std::endl;
	}
	
	return result;
}

JSValue JSObject::CallAsFunction(const std::vector<JSValue>& arguments) const {
	static const std::string log_prefix { "MDL: JSObject::CallAsFunction: " };
	
	if (!IsFunction()) {
		const std::string message = "This object is not a function.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = nullptr;
	const JSObjectRef this_object = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, 0, nullptr, &exception);
	}
	
	if (exception) {
		// assert(!js_value_ref);
		const std::string message = JSValue(js_context_, exception);
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	assert(js_value_ref);
	JSValue js_value(js_context_, js_value_ref);
	JSValueUnprotect(js_context_, js_value_ref);
	
	return js_value;
}

JSValue JSObject::CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) const {
	static const std::string log_prefix { "MDL: JSObject::CallAsFunction: " };
	
	if (!IsFunction()) {
		const std::string message = "This object is not a function.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_value_ref = JSObjectCallAsFunction(js_context_, js_object_ref_, this_object, 0, nullptr, &exception);
	}
	
	if (exception) {
		// assert(!js_value_ref);
		const std::string message = JSValue(js_context_, exception);
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	assert(js_value_ref);
	JSValue js_value(js_context_, js_value_ref);
	JSValueUnprotect(js_context_, js_value_ref);
	
	return js_value;
}

JSObject JSObject::CallAsConstructor(const std::vector<JSValue>& arguments) const {
	static const std::string log_prefix { "MDL: JSObject::CallAsConstructor: " };
	
	if (!IsConstructor()) {
		const std::string message = "This object is not a constructor.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_object_ref = JSObjectCallAsConstructor(js_context_, js_object_ref_, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectCallAsConstructor(js_context_, js_object_ref_, 0, nullptr, &exception);
	}
	
	if (exception) {
		// assert(!js_object_ref);
		const std::string message = JSValue(js_context_, exception);
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	assert(js_object_ref);
	JSObject js_object(js_context_, js_object_ref);
	JSValueUnprotect(js_context_, js_object_ref);
	
	return js_object;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {


/*!
  @method
  @abstract       Create a JavaScript object from the given JavaScript value.
  @param js_value The JSValue to convert.
  @result         The JSObject result of conversion.
  @throws         std::invalid_argument if the given JavaScript value could not be converted to a JavaScript object.
*/
//JSObject(const JSValue& js_value);

/*
JSObject::JSObject(const JSValue& js_value)
		: JSValue(js_value)
		, js_context_(js_value.js_context_) {

	static const std::string log_prefix { "MDL: JSObject(const JSValue& js_value) " };
	
	if (!IsObject()) {
		const std::string message = "JSValue is not an object";
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = JSValueToObject(js_context_, js_value_ref_, &exception);
	if (exception) {
		std::ostringstream os;
		os << "JSValue could not be converted to a JSObject: "<< JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::invalid_argument(message);
	}
	
	assert(js_object_ref);
	js_object_ref_ = js_object_ref;
	JSValueProtect(js_context_, js_object_ref_);
}
*/
