/**
 * Appcelerator Titanium Mobile
 * Copyright (c) 2009-2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_
#define _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#include <iostream>

namespace JavaScriptCoreCPP { namespace RAII {


class NativeObject final {

 public:

	NativeObject(const JSContext& js_context) : js_context_(js_context) {
	}
	
	virtual ~NativeObject() {
	}
	
	virtual void Initialize() {
		static const std::string log_prefix { "MDL: NativeObject::Initialize: " };
		std::clog << log_prefix << std::endl;
	}
	
	void Finalize() {
		static const std::string log_prefix { "MDL: NativeObject::Finalize: " };
		std::clog << log_prefix << std::endl;
	}

	bool HasProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: NativeObject::HasProperty: " };
		const bool has_property = properties_.count(property_name) > 1;
		std::clog << log_prefix
		          << property_name
		          << " = "
		          << std::boolalpha
		          << has_property
		          << std::endl;
		return has_property;
	}

	JSValue GetProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: NativeObject::GetProperty: " };
		const auto position = properties_.find(property_name);
		JSValue result = position != properties_.end() ? position -> second : js_context_.CreateUndefined();
		std::clog << log_prefix
		          << property_name
		          << " = "
		          << result;

		if (position == properties_.end()) {
			// The property did not exist.
			std::clog << ", property does not exist";
		}
		
		std::clog << "." << std::endl;
		
		return result;
	}

	bool SetProperty(const JSString& property_name, const JSValue& value) {
		static const std::string log_prefix { "MDL: NativeObject::SetProperty: " };
		const auto previous_position = properties_.find(property_name);
		if (previous_position != properties_.end()) {
			// The property already exists, so remove the old value.
			properties_.erase(previous_position);
		}
		
		const auto insert_result = properties_.insert(std::make_pair(property_name, value));
		
		std::clog << log_prefix
		          << property_name
		          << " = "
		          << value
		          << ", inserted = "
		          << std::boolalpha
		          << insert_result . second;
		
		if (previous_position != properties_.end()) {
			// The property already existed, so log this fact.
			std::clog << ", previous value was "
			          << previous_position -> second;
		}
		
		std::clog << "." << std::endl;
		
		return false;
	}

	bool DeleteProperty(const JSString& property_name) {
		static const std::string log_prefix { "MDL: NativeObject::DeleteProperty: " };
		
		const auto previous_position = properties_.find(property_name);
		if (previous_position != properties_.end()) {
			// The property already exists, so remove the old value.
			properties_.erase(previous_position);
		}

		std::clog << log_prefix
		          << property_name;
		
		if (previous_position != properties_.end()) {
			// The property already existed, so log this fact.
			std::clog << ", previous value was "
			          << previous_position -> second;
		}
		
		std::clog << "." << std::endl;
		
		return previous_position != properties_.end();
	}

	void GetPropertyNames(const JSPropertyNameAccumulator& accumulator) {
		static const std::string log_prefix { "MDL: NativeObject::GetPropertyNames: " };

		for (const auto& property : properties_) {
			accumulator.AddName(property.first);
		}

		std::clog << log_prefix
		          << " accumulated "
		          << properties_.size()
		          << " property names"
		          << std::endl;
	}

	JSValue CallAsFunction(const std::vector<JSValue>& arguments, const JSObject& this_object) {
		static const std::string log_prefix { "MDL: NativeObject::CallAsFunction: " };

		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< " arguments with this_object = "
				<< this_object
				<< std::endl;

		return js_context_.CreateUndefined();
	}

	JSObject CallAsConstructor(const std::vector<JSValue>& arguments) {
		static const std::string log_prefix { "MDL: NativeObject::CallAsConstructor: " };

		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< std::endl;
		
		return js_context_.CreateObject();
	}

	bool HasInstance(const JSValue& possible_instance) {
		static const std::string log_prefix { "MDL: NativeObject::HasInstance: " };

		std::clog << log_prefix << possible_instance;
		
		bool has_instance = false;
		std::string message;
		try {
			dynamic_cast<const NativeObject&>(possible_instance);
			has_instance = true;
		} catch (const std::bad_cast& exception) {
			// Expected exception if possible_instance is not of our class
			// type.
		} catch (const std::exception& exception) {
			// Unexpected exception.
			message = exception.what();
		} catch (...) {
			// Unexpected and unknown exception.
			message = "Unknown exception";
		}
		
		std::clog << std::boolalpha << has_instance;
		
		if (!message.empty()) {
			std::clog << ", (caught exception: " << message << ")";
		}
		
		std::clog << "." << std::endl;
		
		return has_instance;
	}
	
	JSValue ConvertToType(const JSValue::Type& js_value_type) {
		static const std::string log_prefix { "MDL: NativeObject::ConvertToType: " };

		std::clog << log_prefix
		          << "Don't know how to convert to type "
		          << js_value_type
		          << std::endl;
		
		return js_context_.CreateUndefined();
	}

private:

	JSContext                             js_context_;
	std::unordered_map<JSString, JSValue> properties_;
};

}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_