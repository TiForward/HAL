/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_WIDGET_HPP_
#define _JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_WIDGET_HPP_

#include "JavaScriptCoreCPP.hpp"
#include <iostream>
#include <sstream>

namespace JavaScriptCoreCPP {

/*!
  @class
  
  @discussion This is an example of how to create a JSNativeObject.
*/
class Widget final : public JSNativeObject<Widget> {

 public:

	virtual ~Widget() {
	}
	
	JSObject Constructor(const std::vector<JSValue>& arguments) {
		return *this;
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

	JSValue SayHello(const std::vector<JSValue>& arguments, JSObject& this_object) {
		std::ostringstream os;
		os << "Hello";

		const auto name = static_cast<JSString>(name_);
		if (!name.empty()) {
			os << ", " << name;
		}

		os << ". Your number is " << number_ << ".";

		return get_context().CreateString(os.str());
	}

 protected:

	// Only a JSContext can create a native object.
	friend JSContext;

	// Your constructor can have as many arguments as you like, but the
	// first one must be the JSContext that your native object will
	// execute in.
	Widget(const JSContext& js_context, JSString name = {"world"}, int number = 42)
			: JSNativeObject<Widget>(js_context, JSExport())
			, name_(js_context.CreateString(name))
			, number_(js_context.CreateNumber(number)) {
	}

	void Initialize() {
	}
	
	void Finalize() {
	}

private:

	JSValue        name_;
	JSNumber       number_;
	const JSNumber pi_ = get_context().CreateNumber(3.141592653589793);

	static JSNativeClass<Widget> JSExport() {
		static JSNativeClass<Widget> js_native_class("Widget");
		static std::once_flag of;
		std::call_once(of, [] {
				JSNativeClassBuilder<Widget> builder(js_native_class);
				js_native_class = builder
						.Initialize(&Widget::Initialize)
						.Finalize(&Widget::Finalize)
						.Constructor(&Widget::Constructor)
						.AddValueProperty("name", &Widget::GetName, &Widget::SetName)
						.AddValueProperty("number", &Widget::GetNumber, &Widget::SetNumber)
						.AddValueProperty("pi", &Widget::GetPi)
						.AddFunctionProperty("sayHello", &Widget::SayHello)
						.build();
			});
		
		return js_native_class;
	}
	
};
	
} // namespace JavaScriptCoreCPP {

#endif // _JAVASCRIPTCORECPP_XCODE_JAVASCRIPTCORECPPTESTS_RAII_WIDGET_HPP_
