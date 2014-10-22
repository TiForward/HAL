/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSClass.hpp"
#include "JavaScriptCoreCPP/RAII/JSUndefined.hpp"
#include <iostream>

namespace JavaScriptCoreCPP { namespace RAII {

class DerivedJSObject final : public JSObject {

 public:

	DerivedJSObject(const JSClass& js_class, const JSContext& js_context)
			: JSObject(js_class, js_context)
			, js_context_(js_context)
			, js_class_(js_class)	{
	}
	
	void Initialize() {
		static const std::string log_prefix { "MDL: DerivedJSObject::Initialize: " };
		std::clog << log_prefix << std::endl;
	}
	
	void Finalize() {
		static const std::string log_prefix { "MDL: DerivedJSObject::Finalize: " };
		std::clog << log_prefix << std::endl;
	}

	bool HasProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: DerivedJSObject::HasProperty: " };
		std::clog << log_prefix << property_name << std::endl;
		return false;
	}

	JSValue GetProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: DerivedJSObject::GetProperty: " };
		std::clog << log_prefix << property_name << std::endl;
		return JSUndefined(js_context_);
	}

	bool SetProperty(const JSString& property_name, const JSValue& value) {
		static const std::string log_prefix { "MDL: DerivedJSObject::SetProperty: " };
		std::clog << log_prefix << property_name << " = " << value << std::endl;
		return false;
	}

	bool DeleteProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: DerivedJSObject::DeleteProperty: " };
		std::clog << log_prefix << property_name << std::endl;
		return false;
	}

	void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) {
		static const std::string log_prefix { "MDL: DerivedJSObject::GetPropertyNames: " };
		std::clog << log_prefix << std::endl;
	}

	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) {
		static const std::string log_prefix { "MDL: DerivedJSObject::CallAsFunction: " };
		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< " arguments with this_object = "
				<< this_object
				<< std::endl;
		return JSUndefined(js_context_);
	}

	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) {
		static const std::string log_prefix { "MDL: DerivedJSObject::CallAsConstructor: " };
		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< std::endl;
		return JSObject(js_context_);
	}

	bool HasInstance(const JSValue& possible_instance) {
		static const std::string log_prefix { "MDL: DerivedJSObject::HasInstance: " };
		std::clog << log_prefix << possible_instance << std::endl;
		return false;
	}
	
	JSValue ConvertToType(const JSValue::Type& js_value_type) {
		static const std::string log_prefix { "MDL: DerivedJSObject::ConvertToType: " };
		std::clog << log_prefix << js_value_type << std::endl;
		return JSUndefined(js_context_);
	}

private:

	JSContext js_context_;
	JSClass   js_class_;
	
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_
