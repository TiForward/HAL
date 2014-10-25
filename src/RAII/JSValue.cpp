/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSBoolean.hpp"
#include "JavaScriptCoreCPP/RAII/JSNumber.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JSUtil.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

JSValue::JSValue(const JSContext& js_context, const JSString& js_string, bool parse_as_json) : js_context_(js_context) {
	if (parse_as_json) {
		JSValueRef js_value_ref = JSValueMakeFromJSONString(js_context, js_string);
		if (!js_value_ref) {
			static const std::string log_prefix { "MDL: JSONStringToJSValue: " };
			std::ostringstream os;
			os << "Input is not a valid JSON string: " << js_string;
			const std::string message = os.str();
			std::clog << log_prefix << " [ERROR] " << message << std::endl; 
			throw std::invalid_argument(message);
		}
	} else {
		js_value_ref_ = JSValueMakeString(js_context_, js_string);
	}
}

JSString JSValue::ToJSONString(unsigned indent) {
	JSValueRef exception { nullptr };
	JSStringRef js_string_ref = JSValueCreateJSONString(js_context_, js_value_ref_, indent, &exception);
	if (exception) {
		// assert(!js_string_ref);
		static const std::string log_prefix { "MDL: JSValueCreateJSONString: " };
		std::ostringstream os;
		os << "JSValue could not be serialized to a JSON string: " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	JSString js_string;
	if (js_string_ref) {
		js_string = JSString(js_string_ref);
		JSStringRelease(js_string_ref);
	}
	
	return js_string;
}

JSValue::operator JSString() const {
	JSValueRef exception { nullptr };
	JSStringRef js_string_ref = JSValueToStringCopy(js_context_, js_value_ref_, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: JSValue::operator JSString() const: " };
		std::ostringstream os;
		os << "JSValue could not be converted to a JSString: "<< JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	assert(js_string_ref);
	JSString js_string(js_string_ref);
	JSStringRelease(js_string_ref);
	
	return js_string;
}

JSValue::operator JSBoolean() const {
	return JSBoolean(js_context_, operator bool());
}

JSValue::operator double() const {
	JSValueRef exception { nullptr };
	const double result = JSValueToNumber(js_context_, js_value_ref_, &exception);
	
	if (exception) {
		static const std::string log_prefix { "MDL: JSValue::operator JSNumber(): " };
		std::ostringstream os;
		os << "JSValue could not be converted to a JSNumber: "<< JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	return result;
}

JSValue::operator int32_t() const {
	return ::JavaScriptCoreCPP::detail::to_int32_t(operator double());
}

JSValue::operator JSNumber() const {
	return JSNumber(js_context_, operator double());
}

JSValue::operator JSObject() const {
	JSValueRef exception { nullptr };
	JSObjectRef js_object_ref = JSValueToObject(js_context_, js_value_ref_, &exception);
	
	if (exception) {
		static const std::string log_prefix { "MDL: JSValue::operator JSObject(): " };
		std::ostringstream os;
		os << "JSValue could not be converted to a JSObject: "<< JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	assert(js_object_ref);
	JSObject js_object(js_context_, js_object_ref);
	JSValueUnprotect(js_context_, js_object_ref);
	
	return js_object;
}

JSValue::Type JSValue::GetType() const {
	const JSType js_type = JSValueGetType(js_context_, js_value_ref_);
	switch (js_type) {
		case kJSTypeUndefined:
			return Type::Undefined;
			break;
			
		case kJSTypeNull:
			return Type::Null;
			break;
			
		case kJSTypeBoolean:
			return Type::Boolean;
			break;
			
		case kJSTypeNumber:
			return Type::Number;
			break;
			
		case kJSTypeString:
			return Type::String;
			break;
			
		case kJSTypeObject:
			return Type::Object;
			break;
			
		default:
			static const std::string log_prefix { "MDL: JSValue::get_type() const: " };
			const std::string message = "JSValue could not decode JSType = " + std::to_string(js_type);
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
			throw std::logic_error(message);
	}
}

bool JSValue::IsInstanceOfConstructor(const JSObject& constructor) const {
	JSValueRef exception { nullptr };
	const bool result = JSValueIsInstanceOfConstructor(js_context_, js_value_ref_, constructor, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: JSValue::IsInstanceOfConstructor:" };
		std::ostringstream os;
		os << "JSValue caught exception during JSValueIsInstanceOfConstructor: " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [DEBUG] " << message << std::endl;
		assert(result == false);
	}
	
	return result;
}

bool JSValue::IsEqualWithTypeCoercion(const JSValue& rhs) const {
	JSValueRef exception { nullptr };
	const bool result = JSValueIsEqual(js_context_, js_value_ref_, rhs.js_value_ref_, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: IsEqualWithTypeCoercion: " };
		std::ostringstream os;
		os << "caught exception: " << JSValue(js_context_, exception);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl; 
	}
	
	return result;
}

std::string to_string(const JSValue::Type& js_value_type) {
	switch (js_value_type) {
		case JSValue::Type::Undefined:
		return "Undefined";
		break;

		case JSValue::Type::Null:
		return "Null";
		break;

		case JSValue::Type::Boolean:
		return "Boolean";
		break;

		case JSValue::Type::Number:
		return "Number";
		break;

		case JSValue::Type::String:
		return "String";
		break;

		case JSValue::Type::Object:
		return "Object";
		break;

		default:
			static const std::string log_prefix { "MDL: to_string(JSValue::Type): " };
			std::ostringstream os;
			os << log_prefix
			   << "Could not map JSValue::Type with value "
			   << static_cast<std::underlying_type<JSValue::Type>::type>(js_value_type)
			   << " to a std::string.";
			const std::string message = os.str();
			std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl; 
			throw std::logic_error(message);
	}
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
