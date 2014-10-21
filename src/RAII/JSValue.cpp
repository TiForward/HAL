// -*- mode: c++ -*-
//
//  Author: Matt Langston
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include <stdexcept>
#include <sstream>
#include <cassert>

namespace JavaScriptCoreCPP { namespace RAII {

JSValue::JSValue(const JSString& js_string, const JSContext& js_context, bool parse_as_json) : js_context_(js_context) {
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
		os << "JSValue could not be serialized to a JSON string: " << JSValue(exception, js_context_);
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
		os << "JSValue could not be converted to a JSString: "<< JSValue(exception, js_context_);
		const std::string message = os.str();
		std::clog << log_prefix << " [LOGIC ERROR] " << message << std::endl;
		throw std::logic_error(message);
	}
	
	assert(js_string_ref);
	JSString js_string(js_string_ref);
	JSStringRelease(js_string_ref);
	
	return js_string;
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

bool JSValue::IsInstanceOfConstructor(const JSObject& constructor) {
	JSValueRef exception { nullptr };
	const bool result = JSValueIsInstanceOfConstructor(js_context_, js_value_ref_, constructor, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: JSValue::IsInstanceOfConstructor:" };
		std::ostringstream os;
		os << "JSValue caught exception during JSValueIsInstanceOfConstructor: " << JSValue(exception, js_context_);
		const std::string message = os.str();
		std::clog << log_prefix << " [DEBUG] " << message << std::endl;
		assert(result == false);
	}
	
	return result;
}

bool IsEqualWithTypeCoercion(const JSValue& lhs, const JSValue& rhs) {
	JSValueRef exception { nullptr };
	//const bool result = JSValueIsEqual(lhs.js_context_, lhs.js_value_ref_, rhs.js_value_ref_, &exception);
	const bool result = JSValueIsEqual(lhs, lhs, rhs, &exception);
	if (exception) {
		static const std::string log_prefix { "MDL: IsEqualWithTypeCoercion: " };
		std::ostringstream os;
		os << "caught exception: " << JSValue(exception, lhs.js_context_);
		const std::string message = os.str();
		std::clog << log_prefix << " [ERROR] " << message << std::endl; 
	}
	
	return result;
}

}} // namespace JavaScriptCoreCPP { namespace RAII {
