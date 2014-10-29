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

	NativeObject(const JSContext& js_context)
			: JSNativeObject<NativeObject>(js_context, JSExport()) {
	}

	virtual ~NativeObject() {
	}
	
	virtual void Initialize() {
	}
	
	void Finalize() {
	}

	JSObject Constructor(const std::vector<JSValue>& arguments) {
		return get_context().CreateObject();
	}

	JSValue GetName() const {
		return name_;
	}

	bool SetName(const JSValue& value) {
		name_ = value;
		return true;
	}

	JSValue GetNumber() const {
		return number_;
	}

	bool SetNumber(const JSValue& value) {
		number_ = value;
		return true;
	}

	JSValue GetPi() const {
		return pi_;
	}

	JSValue Hello(const std::vector<JSValue>& arguments, const JSObject& this_object) {
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

	JSValue  name_   = get_context().CreateString();
	JSNumber number_ = get_context().CreateNumber(42);
	JSNumber pi_     = get_context().CreateNumber(3.141592653589793);

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
