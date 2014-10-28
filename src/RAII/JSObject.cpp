/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSPropertyNameAccumulator.hpp"

#include "JavaScriptCoreCPP/RAII/JSUndefined.hpp"
#include "JavaScriptCoreCPP/RAII/JSNull.hpp"
#include "JavaScriptCoreCPP/RAII/JSBoolean.hpp"
#include "JavaScriptCoreCPP/RAII/JSNumber.hpp"

#include "JavaScriptCoreCPP/RAII/detail/JSPropertyNameArray.hpp"
#include "JavaScriptCoreCPP/RAII/detail/JSUtil.hpp"

#include <type_traits>
#include <sstream>
#include <limits>

namespace JavaScriptCoreCPP { namespace RAII {

JSObject::JSObject(const JSContext& js_context, const JSClass& js_class, void* private_data)
		: JSObject(js_context, JSObjectMake(js_context, js_class, private_data)) {
}

JSObject::JSObject(const JSContext& js_context, JSObjectRef js_object_ref)
		: JSValue(js_context, js_object_ref)
		, js_object_ref__(js_object_ref) {
	
	static const std::string log_prefix { "MDL: JSObject::JSObject(const JSContext& js_context, JSObjectRef js_object_ref): " };
	
	if (!js_object_ref__) {
		const std::string message = "js_object_ref can not be nullptr.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	JSValueProtect(get_context(), js_object_ref__);
}

JSObject JSObject::CallAsConstructor(const std::vector<JSValue>&  arguments) {
	static const std::string log_prefix { "MDL: JSObject::CallAsConstructor: " };
	
	if (!IsConstructor()) {
		const std::string message = "This object is not a constructor.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = nullptr;
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_object_ref = JSObjectCallAsConstructor(get_context(), js_object_ref__, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_object_ref = JSObjectCallAsConstructor(get_context(), js_object_ref__, 0, nullptr, &exception);
	}
	
	if (exception) {
		const auto message = static_cast<std::string>(JSValue(get_context(), exception));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}

	// postcondition
	assert(js_object_ref);
	return JSObject(get_context(), js_object_ref);
}

JSValue JSObject::CallAsFunction(const std::vector<JSValue>&  arguments, const JSObject& this_object) {
	static const std::string log_prefix { "MDL: JSObject::CallAsFunction: " };
	
	if (!IsFunction()) {
		const std::string message = "This object is not a function.";
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref { nullptr };
	if (!arguments.empty()) {
		std::vector<JSValueRef> arguments_array;
		std::transform(arguments.begin(), arguments.end(), std::back_inserter(arguments_array), [](const JSValue& js_value) { return static_cast<JSValueRef>(js_value); });
		js_value_ref = JSObjectCallAsFunction(get_context(), js_object_ref__, this_object, arguments_array.size(), &arguments_array[0], &exception);
	} else {
		js_value_ref = JSObjectCallAsFunction(get_context(), js_object_ref__, this_object, 0, nullptr, &exception);
	}
	
	if (exception) {
		const auto message = static_cast<std::string>(JSValue(get_context(), exception));
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		throw std::runtime_error(message);
	}
	
	assert(js_value_ref);
	return JSValue(get_context(), js_value_ref);
}

JSValue JSObject::GetProperty(const JSString& property_name) const {
	static const std::string log_prefix { "MDL: JSObject::GetProperty: " };
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetProperty(get_context(), js_object_ref__, property_name, &exception);
	if (exception) {
		std::ostringstream os;
		os << "Caught exception getting property with name \"" << property_name << "\": " << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;
		
		assert(!js_value_ref);
		
		throw std::runtime_error(message);
	}
	
	if (js_value_ref) {
		JSValue js_value(get_context(), js_value_ref);
		return js_value;
	}
	
	return get_context().CreateUndefined();
}

JSValue JSObject::GetProperty(unsigned property_index) const {
	static const std::string log_prefix { "MDL: JSObject::GetProperty: " };
	
	JSValueRef exception { nullptr };
	JSValueRef js_value_ref = JSObjectGetPropertyAtIndex(get_context(), js_object_ref__, property_index, &exception);
	if (exception) {
		std::ostringstream os;
		os << "Caught exception getting property at index \"" << property_index << "\": " << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;

		assert(!js_value_ref);
		
		throw std::runtime_error(message);
	}
	
	if (js_value_ref) {
		JSValue js_value(get_context(), js_value_ref);
		return js_value;
	}
	
	return get_context().CreateUndefined();
}

void JSObject::SetProperty(const JSString& property_name, const JSValue& property_value, const std::unordered_set<JSPropertyAttribute>& attributes) {
	static const std::string log_prefix { "MDL: JSObject::SetProperty: " };
	
	JSValueRef exception { nullptr };
	JSObjectSetProperty(get_context(), js_object_ref__, property_name, property_value, detail::ToJSPropertyAttributes(attributes), &exception);
	if (exception) {
		std::ostringstream os;
		os << "Caught exception setting property with name \""
		   << property_name
		   << "\" to value \""
		   << property_value
		   << "\": "
		   << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;

		throw std::runtime_error(message);
	}
}

void JSObject::SetProperty(unsigned property_index, const JSValue& property_value) {
	static const std::string log_prefix { "MDL: JSObject::SetPropertyAtIndex: " };
	
	JSValueRef exception { nullptr };
	JSObjectSetPropertyAtIndex(get_context(), js_object_ref__, property_index, property_value, &exception);
	if (exception) {
		std::ostringstream os;
		os << "Caught exception setting property with index \""
		   << property_index
		   << "\" to value \""
		   << property_value
		   << "\": "
		   << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl;

		throw std::runtime_error(message);
	}
}

bool JSObject::DeleteProperty(const JSString& property_name) {
	static const std::string log_prefix { "MDL: JSObject::DeleteProperty: " };
	
	JSValueRef exception { nullptr };
	const bool result = JSObjectDeleteProperty(get_context(), js_object_ref__, property_name, &exception);
	if (exception) {
		std::ostringstream os;
		os << "Caught exception deleting property with name \""
		   << property_name << "\": "
		   << JSValue(get_context(), exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [WARN] " << message << std::endl;
		
		throw std::runtime_error(message);
	}
	
	return result;
}

JSObject::operator JSUndefined() const {
	return get_context().CreateUndefined();
}

JSObject::operator JSNull() const {
	return get_context().CreateNull();
}

JSObject::operator JSBoolean() const {
	return get_context().CreateBoolean(true);
}

JSObject::operator JSNumber() const {
	return get_context().CreateNumber(std::numeric_limits<double>::quiet_NaN());
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
