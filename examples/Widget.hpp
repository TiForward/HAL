/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#ifndef _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_
#define _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_

#include "JavaScriptCoreCPP.hpp"
#include <iostream>
#include <sstream>

using namespace JavaScriptCoreCPP;

/*!
  @class
  
  @discussion This is an example of how to create JavaScript objects
  implemented by a C++ class.
*/
class Widget : public JSExport<Widget> {
	
public:
	
	/*!
	  @method
	  
	  @abstract Only a JSContext can create your JavaScript objects. See
	  the JSContext::CreateObject method for more information.
	  
	  @discussion Your constructor can have as many arguments as you
	  need, but the first two are mandatory and provided by the
	  JSContext that creates you.
	  
	  @param js_context The JavaScriptCore execution context that your
	  JavaScript objects will execute in.
	  
	  @param name An optioanl name for the 'sayHello' JavaScript
	  function.
	  
	  @param number An optioanl number for the 'sayHello' JavaScript
	  function.
	*/
	Widget(const JSContext& js_context)
			: JSExport<Widget>(js_context)
			, name_ (js_context.CreateString("world"))
			, number_ (js_context.CreateNumber(42)) {
	}

	/*!
	  @method
	  
	  @abstract Only a JSContext (by calling the
	  JSContext::CreateObject() method) and other JavaScript objects (by
	  calling 'new Widget(...)') can create your JavaScript objects.
	  
	  @param js_context The JavaScriptCore execution context that your
	  JavaScript object will execute in.

	  @param arguments An rvalue reference to the JSValue array of
	  arguments from the JavaScript 'new' expression.
	*/
	Widget(const JSContext& js_context, std::vector<JSValue>&& arguments)
			: JSExport<Widget>(js_context)
			, name_ (js_context.CreateString("world"))
			, number_ (js_context.CreateNumber(42)) {
	}
	
	/*!
	  @method
	  
	  @abstract Define how your JavaScript objects appear to
	  JavaScriptCore.
	  
	  @discussion JavaScriptCoreCPP will call this function exactly once
	  just before your first JavaScript object is created.
	*/
	static void JSExportInitialize() {
		SetClassName("Widget");
		SetClassVersion(1);
		AddValueProperty("name", &Widget::get_name, &Widget::set_name);
		AddValueProperty("number", &Widget::get_number, &Widget::set_number);
		AddValueProperty("pi", &Widget::pi);
		AddFunctionProperty("sayHello", &Widget::sayHello);
	}
	
	/*!
	  @method

	  @abstract This is callback is invoked when your JavaScript object
	  is used as a constructor in a 'new' expression.
	*/
	JSObject Constructor(const std::vector<JSValue>&& arguments) {
		// Use the arguments to initialize yourself as required.
		return *this;
	}

	JSValue get_name() const {
		return name_;
	}

	bool set_name(const JSValue&& value) {
		name_ = value;
		return true;
	}

	JSValue get_number() const {
		return number_;
	}

	bool set_number(const JSValue&& value) {
		number_ = value;
		return true;
	}

	JSValue pi() const {
		return pi_;
	}

	JSValue sayHello(const std::vector<JSValue>&& arguments, JSObject&& this_object) {
		std::ostringstream os;
		os << "Hello";

		JSString name = static_cast<JSString>(name_);
		if (!name.empty()) {
			os << ", " << name;
		}

		os << ". Your number is " << number_ << ".";

		return get_context().CreateString(os.str());
	}

 private:

	JSValue        name_;
	JSNumber       number_;
	const JSNumber pi_ { get_context().CreateNumber(3.141592653589793) };

};
	
#endif // _JAVASCRIPTCORECPP_EXAMPLES_WIDGET_HPP_
