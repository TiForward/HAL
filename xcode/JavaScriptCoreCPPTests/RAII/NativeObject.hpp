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
#include <sstream>

namespace JavaScriptCoreCPP { namespace RAII {


class NativeObject final : public JSNativeObject<NativeObject> {

 public:

	NativeObject(const JSContext& js_context) : JSNativeObject<NativeObject>(js_context, JSExport()) {
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

	JSObject Constructor(const std::vector<JSValue>& arguments) {
		static const std::string log_prefix { "MDL: NativeObject::Constructor: " };

		std::clog << log_prefix << "called with " << arguments.size() << "." << std::endl;
		
		return get_context().CreateObject();
	}

	JSValue GetName() const {
		static const std::string log_prefix { "MDL: NativeObject::GetName: " };
		std::clog << log_prefix << " name = " << name_ << "." << std::endl;
		return name_;
	}

	bool SetName(const JSValue& value) {
		static const std::string log_prefix { "MDL: NativeObject::SetName: " };
		name_ = value;
		std::clog << log_prefix << " name = " << name_ << "." << std::endl;
		return true;
	}

	JSValue GetNumber() const {
		static const std::string log_prefix { "MDL: NativeObject::GetNumber: " };
		std::clog << log_prefix << " number = " << number_ << "." << std::endl;
		return number_;
	}

	bool SetNumber(const JSValue& value) {
		static const std::string log_prefix { "MDL: NativeObject::SetNumber: " };
		number_ = value;
		std::clog << log_prefix << " number = " << number_ << "." << std::endl;
		return true;
	}

	JSValue GetPi() const {
		static const std::string log_prefix { "MDL: NativeObject::GetPi: " };
		std::clog << log_prefix << " pi = " << number_ << "." << std::endl;
		return pi_;
	}

	JSValue Hello(const std::vector<JSValue>& arguments, const JSObject& this_object) {
		static const std::string log_prefix { "MDL: NativeObject::FooFunction: " };

		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< " arguments with this_object = "
				<< this_object
				<< "."
				<< std::endl;

		std::ostringstream os;
		os << "Hello";

		const auto name = static_cast<JSString>(name_);
		if (!name.empty()) {
			os << ", " << name;
		}

		os << ". Your number is " << number_ << ".";

		return get_context().CreateString(os.str());
	}

	JSValue Goodbye(const std::vector<JSValue>& arguments, const JSObject& this_object) {
		static const std::string log_prefix { "MDL: NativeObject::BarFunction: " };

		std::clog
				<< log_prefix
				<< "called with "
				<< arguments.size()
				<< " arguments with this_object = "
				<< this_object
				<< std::endl;

		std::ostringstream os;
		os << "Goodbye";
		
		const auto name = static_cast<JSString>(name_);
		if (!name.empty()) {
			os << ", " << name;
		}

		os << ". Your number was " << number_ << ".";
		
		return get_context().CreateString(os.str());
	}

private:

	std::unordered_map<JSString, JSValue> properties_;
	JSValue                               name_   = get_context().CreateString();
	JSNumber                              number_ = get_context().CreateNumber(42);
	JSNumber                              pi_     = get_context().CreateNumber(3.141592653589793);

	static JSNativeClass<NativeObject> JSExport() {
		static JSNativeClass<NativeObject> js_native_class("MyClass");
		static std::once_flag of;
		std::call_once(of, [] {
				JSNativeClassBuilder<NativeObject> builder(js_native_class);
				js_native_class = builder
						.Initialize(&NativeObject::Initialize)
						.Finalize(&NativeObject::Finalize)
						// .Constructor(&NativeObject::Constructor)
						.AddValueProperty("name", &NativeObject::GetName, &NativeObject::SetName)
						.AddValueProperty("number", &NativeObject::GetNumber, &NativeObject::SetNumber)
						.AddValueProperty("pi", &NativeObject::GetPi)
						.AddFunctionProperty("hello", &NativeObject::Hello)
						.AddFunctionProperty("goodbye", &NativeObject::Goodbye)
						.build();
			});
		
		return js_native_class;
	}
	
};
	
}} // namespace JavaScriptCoreCPP { namespace RAII {

#endif // _TITANIUM_MOBILE_WINDOWS_JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_DERIVEDJSOBJECT_HPP_
