/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/HAL.hpp"
#include "Widget.hpp"
#include "OtherWidget.hpp"
#include <typeinfo>
#include <iostream>
#import <XCTest/XCTest.h>

using namespace HAL;

@interface JSExportTests : XCTestCase
@end

@implementation JSExportTests {
	JSContextGroup js_context_group;
}

- (void)setUp
{
	[super setUp];
	// Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
	// Put teardown code here. This method is called after the invocation of each test method in the class.
	[super tearDown];
}

- (void)testTypeId
{
	std::cout << "MDL: typeid(Widget).name() = " << typeid(Widget).name() << '\n';
}

- (void)testJSPropertyAttribute
{
	std::unordered_set<JSClassAttribute> attributes;
	XCTAssertEqual(0, attributes.count(JSClassAttribute::None));
	XCTAssertEqual(0, attributes.count(JSClassAttribute::NoAutomaticPrototype));

	const auto insert_result = attributes.insert(JSClassAttribute::NoAutomaticPrototype);
	XCTAssertTrue(insert_result.second);
	XCTAssertEqual(1, attributes.count(JSClassAttribute::NoAutomaticPrototype));
	XCTAssertEqual(*insert_result.first, JSClassAttribute::NoAutomaticPrototype);

	XCTAssertEqual(1, attributes.size());
}

- (void)testValuePropertyCallback
{
	detail::JSExportNamedValuePropertyCallback<Widget> name_callback("name", &Widget::js_get_name, &Widget::js_set_name, {JSPropertyAttribute::DontDelete});
	detail::JSExportNamedValuePropertyCallback<Widget> number_callback("number", &Widget::js_get_number, &Widget::js_set_number, {JSPropertyAttribute::DontDelete});
	detail::JSExportNamedValuePropertyCallback<Widget> pi_callback("pi", &Widget::js_get_pi, nullptr, {JSPropertyAttribute::DontDelete});

	XCTAssertEqual(1, name_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
	XCTAssertEqual(0, name_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));

	XCTAssertEqual(1, number_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
	XCTAssertEqual(0, number_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));

	XCTAssertEqual(1, pi_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
	XCTAssertEqual(1, pi_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));
}

- (void)testFunctionPropertyCallbacks
{
	detail::JSExportNamedFunctionPropertyCallback<Widget> sayHello_callback("sayHello", &Widget::js_sayHello, {JSPropertyAttribute::DontDelete});
}

- (void)testJSExportClassDefinitionBuilder
{
	detail::JSExportClassDefinitionBuilder<Widget> builder("Widget");
	builder
	    .AddValueProperty("name", &Widget::js_get_name, &Widget::js_set_name)
	    .AddValueProperty("number", &Widget::js_get_number, &Widget::js_set_number)
	    .AddValueProperty("pi", &Widget::js_get_pi)
	    .AddFunctionProperty("sayhello", &Widget::js_sayHello)
	    // .Function(&Widget::CallAsFunction)
	    // .ConvertType(&Widget::ConvertToType);
	    ;

	auto native_class = builder.build();
}

- (void)testJSExport
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget);
	XCTAssertTrue(global_object.HasProperty("Widget"));

	for (const auto& property_name : static_cast<std::vector<JSString>>(global_object.GetPropertyNames())) {
		//SetProperty(property_name, props.GetProperty(property_name));
		std::clog << "MDL: property_name = " << property_name << std::endl;
	}

	JSValue result = js_context.JSEvaluateScript("typeof Widget;");
	XCTAssertEqual("object", static_cast<std::string>(result));

	result = js_context.JSEvaluateScript("Widget.name;");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("world", static_cast<std::string>(result));

	result = js_context.JSEvaluateScript("Widget.number;");
	XCTAssertTrue(result.IsNumber());
	XCTAssertEqual(42, static_cast<std::uint32_t>(result));

	result = js_context.JSEvaluateScript("Widget.sayHello();");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, world. Your number is 42.", static_cast<std::string>(result));

	result = js_context.JSEvaluateScript("Widget.name = 'foo';");
	result = js_context.JSEvaluateScript("Widget.name;");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("foo", static_cast<std::string>(result));

	result = js_context.JSEvaluateScript("Widget.number = 3 * 7;");
	result = js_context.JSEvaluateScript("Widget.number;");
	XCTAssertTrue(result.IsNumber());
	XCTAssertEqual(21, static_cast<std::uint32_t>(result));

	result = js_context.JSEvaluateScript("Widget.sayHello();");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, foo. Your number is 21.", static_cast<std::string>(result));

	// Test getting access to the underlying C++ object that implements the
	// JavaScript object.
	auto widget_ptr = widget.GetPrivate<Widget>();
	XCTAssertNotEqual(nullptr, widget_ptr);

	widget_ptr->set_name("bar");
	result = js_context.JSEvaluateScript("Widget.name;");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("bar", static_cast<std::string>(result));

	widget_ptr->set_number(4 * 8);
	result = js_context.JSEvaluateScript("Widget.number;");
	XCTAssertTrue(result.IsNumber());
	XCTAssertEqual(32, static_cast<std::uint32_t>(result));

	result = js_context.JSEvaluateScript("Widget.sayHello();");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, bar. Your number is 32.", static_cast<std::string>(result));

	result = js_context.JSEvaluateScript("var widget = new Widget('baz', 999);");
	result = js_context.JSEvaluateScript("widget.sayHello();");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, baz. Your number is 999.", static_cast<std::string>(result));

	// FIXME
	auto string_ptr = widget.GetPrivate<std::string>();
	//XCTAssertEqual(nullptr, string_ptr);
}

- (void)testJSExportGetPrivate
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();
	
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	JSObject otherWidget = js_context.CreateObject(JSExport<OtherWidget>::Class());
	
	// Test getting access to the underlying C++ object
	auto widget_ptr = widget.GetPrivate<Widget>();
	XCTAssertNotEqual(nullptr, widget_ptr);
	
	auto other_widget_ptr = otherWidget.GetPrivate<OtherWidget>();
	XCTAssertNotEqual(nullptr, other_widget_ptr);
	
	// Test getting access to wrong C++ object
	auto wrong_widget_ptr = widget.GetPrivate<OtherWidget>();
	XCTAssertEqual(nullptr, wrong_widget_ptr);
	
	// Test getting access to wrong C++ object
	auto wrong_widget_ptr2 = otherWidget.GetPrivate<Widget>();
	XCTAssertEqual(nullptr, wrong_widget_ptr2);
}

- (void)testJSExportConstructorCount
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();

  // reset count
  Widget::constructor_count__ = 0;
  XCTAssertEqual(0, Widget::constructor_count__);
  
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  
  // Test getting access to the underlying C++ object
  auto widget_ptr = widget.GetPrivate<Widget>();
  XCTAssertNotEqual(nullptr, widget_ptr);

  XCTAssertEqual(1, Widget::constructor_count__);
}

- (void)testJSExportConstructorCountForCallAsConstructor
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();

  // reset count
  Widget::constructor_count__ = 0;
  XCTAssertEqual(0, Widget::constructor_count__);
  
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  
  // Test getting access to the underlying C++ object
  auto widget_ptr = widget.GetPrivate<Widget>();
  XCTAssertNotEqual(nullptr, widget_ptr);

  XCTAssertEqual(1, Widget::constructor_count__);

  const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
  JSObject new_widget = widget.CallAsConstructor(args);

  XCTAssertEqual(2, Widget::constructor_count__);
}

/*
 * Call Widget.sayHello() through operator()
 */
- (void)testCallAsFunction
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget);
	XCTAssertTrue(global_object.HasProperty("Widget"));

	widget.SetProperty("name", js_context.CreateString("bar"));

	XCTAssertTrue(widget.HasProperty("sayHello"));
	JSValue widget_sayHello_property = widget.GetProperty("sayHello");
	XCTAssertTrue(widget_sayHello_property.IsObject());
	JSObject widget_sayHello = static_cast<JSObject>(widget_sayHello_property);
	XCTAssertTrue(widget_sayHello.IsFunction());
	JSValue hello = widget_sayHello(widget);
	XCTAssertTrue(hello.IsString());
	XCTAssertEqual("Hello, bar. Your number is 42.", static_cast<std::string>(hello));
}

/*
 * Call new Widget('baz', 999).sayHello() through operator()
 */
- (void)testNew
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget);
	XCTAssertTrue(global_object.HasProperty("Widget"));

	JSValue result = js_context.JSEvaluateScript("new Widget('baz', 999);");
	XCTAssertTrue(result.IsObject());
	widget = static_cast<JSObject>(result);

	XCTAssertTrue(widget.HasProperty("sayHello"));
	JSValue widget_sayHello_property = widget.GetProperty("sayHello");
	XCTAssertTrue(widget_sayHello_property.IsObject());
	JSObject widget_sayHello = static_cast<JSObject>(widget_sayHello_property);
	XCTAssertTrue(widget_sayHello.IsFunction());
	result = widget_sayHello(widget);
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, baz. Your number is 999.", static_cast<std::string>(result));
}

- (void)testExportObjectToJSArray
{
	JSContext js_context = js_context_group.CreateContext();

	std::vector<JSValue> args = { 
		js_context.CreateObject(),
		js_context.CreateObject(JSExport<Widget>::Class()),
		js_context.CreateObject(JSExport<Widget>::Class()),
		js_context.CreateObject(JSExport<Widget>::Class())
	};

	JSObject js_object = js_context.CreateArray(args);
	XCTAssertTrue(js_object.IsArray());
	JSArray js_array = static_cast<JSArray>(js_object);
	XCTAssertTrue(js_array.IsArray());

	auto export_items = js_array.GetPrivateItems<JSExportObject>();
	XCTAssertEqual(4, export_items.size());
	XCTAssertEqual(nullptr, export_items.at(0));
	XCTAssertNotEqual(nullptr, export_items.at(1));
	XCTAssertNotEqual(nullptr, export_items.at(2));
	XCTAssertNotEqual(nullptr, export_items.at(3));

	XCTAssertEqual(static_cast<JSObject>(args.at(1)).GetPrivate<Widget>().get(), export_items.at(1).get());
	XCTAssertEqual(static_cast<JSObject>(args.at(2)).GetPrivate<Widget>().get(), export_items.at(2).get());
	XCTAssertEqual(static_cast<JSObject>(args.at(3)).GetPrivate<Widget>().get(), export_items.at(3).get());
}

- (void)testInitializeWithProperties
{
	JSContext js_context = js_context_group.CreateContext();

	std::unordered_map<std::string, JSValue> properties = { 
		{ "str",    js_context.CreateString("Hello") },
		{ "num",    js_context.CreateNumber(123) },
		{ "bool",   js_context.CreateBoolean(true) },
		{ "object", js_context.CreateObject() }
	};

	JSObject js_object = js_context.CreateObject(JSExport<Widget>::Class(), properties);

	auto js_properties = js_object.GetProperties();

	XCTAssertTrue(js_properties.find("str")    != js_properties.end());
	XCTAssertTrue(js_properties.find("num")    != js_properties.end());
	XCTAssertTrue(js_properties.find("bool")   != js_properties.end());
	XCTAssertTrue(js_properties.find("object") != js_properties.end());

	XCTAssertTrue(js_properties.at("str").IsString());
	XCTAssertTrue(js_properties.at("num").IsNumber());
	XCTAssertTrue(js_properties.at("bool").IsBoolean());
	XCTAssertTrue(js_properties.at("object").IsObject());
}

/*
 * Call sayHello() through operator() over CallAsConstructor-ed object
 */
- (void)testCallAsConstructor
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget);
	XCTAssertTrue(global_object.HasProperty("Widget"));

	const std::vector<JSValue> args = {js_context.CreateString("foo"), js_context.CreateNumber(123)};
	JSObject js_widget = widget.CallAsConstructor(args);
	XCTAssertTrue(js_widget.HasProperty("sayHello"));
	JSValue widget_sayHello_property = js_widget.GetProperty("sayHello");
	XCTAssertTrue(widget_sayHello_property.IsObject());
	JSObject widget_sayHello = static_cast<JSObject>(widget_sayHello_property);
	XCTAssertTrue(widget_sayHello.IsFunction());
	JSValue result = widget_sayHello(js_widget);
	XCTAssertTrue(result.IsString());
	XCTAssertEqual("Hello, foo. Your number is 123.", static_cast<std::string>(result));
}

- (void)testToString
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget);
	XCTAssertTrue(global_object.HasProperty("Widget"));

	JSValue result = js_context.JSEvaluateScript("Widget.toString();");
	XCTAssertTrue(result.IsString());
	XCTAssertEqual(std::string("[object ") + typeid(Widget).name() + "]", static_cast<std::string>(result));
	//std::clog << "MDL: result = " << static_cast<std::string>(result) << std::endl;
}

- (void)testJSExportFinalize
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    js_context.JSEvaluateScript(R"JS(
      for (var i=0; i<2000;i++) {
        var widget = new Widget('newbar', 123);
      }
    )JS");
    js_context.JSEvaluateScript("Widget = null;");
  }
}

- (void)testJSExportFinalize2
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	for (int i = 0; i < 2000; i++) {
		{
			XCTAssertFalse(global_object.HasProperty("Widget"));
			JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
			global_object.SetProperty("Widget", widget);
			XCTAssertTrue(global_object.HasProperty("Widget"));

			{
				const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
				JSObject js_widget = widget.CallAsConstructor(args);
				global_object.SetProperty("widget", js_widget);
			}
		}

		XCTAssertTrue(global_object.HasProperty("Widget"));
		XCTAssertTrue(global_object.HasProperty("widget"));

		global_object.DeleteProperty("Widget");
		global_object.DeleteProperty("widget");

		XCTAssertFalse(global_object.HasProperty("Widget"));
		XCTAssertFalse(global_object.HasProperty("widget"));

		js_context.GarbageCollect();
	}
}

- (void)testJSExportFinalize3
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberObjectProperty() != 'ok') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize4
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberArrayProperty() != 'works') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize5
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testCallAsFunction() != 'it works fine') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize6
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      var widget = new Widget('newbar', 123);
      var callback = widget.testCallAsFunction;
      (callback() == 'not on Widget');
    )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}


- (void)testJSExportFinalize7
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberNullProperty() !== null) {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize8
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberUndefinedProperty() !== undefined) {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize9
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberBooleanProperty() !== true) {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize10
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberNumberProperty() != 123) {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize11
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (W.testMemberStringProperty() != 'js string') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize12
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (typeof W.testMemberDateProperty() != 'object') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize13
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (typeof W.testMemberErrorProperty() != 'object') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testJSExportFinalize14
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", widget);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    auto result = js_context.JSEvaluateScript(R"JS(
      try {
        var W = new Widget('test', 123);
        for (var i=0; i<2000;i++) {
          var widget = new Widget('newbar', 123);
          if (typeof W.testMemberRegExpProperty() != 'object') {
            throw new Error('assertion failed');
          }
        }
        true;
      } catch (E) {
        false;
      }
      )JS");
    XCTAssertTrue(result.IsBoolean());
    XCTAssertTrue(static_cast<bool>(result));
  }
}

- (void)testEvaluateNewWidgetProperty
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
	XCTAssertTrue(global_object.HasProperty("Widget"));

	// This innocent looking line mysteriously causes the crash. The unit test
	// will pass if you comment it out.
	JSValue test = widget.GetProperty("name");

	const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
	JSObject js_widget = widget.CallAsConstructor(args);
	XCTAssertTrue(js_widget.HasProperty("name"));
	JSValue widget_sayHello_property = js_widget.GetProperty("name");
	XCTAssertTrue(widget_sayHello_property.IsString());
	XCTAssertEqual("newbar", static_cast<std::string>(widget_sayHello_property));
}

- (void)testEvaluateNewWidgetPropertyByEval
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
	XCTAssertTrue(global_object.HasProperty("Widget"));

	// This innocent looking line mysteriously causes the crash. The unit test
	// will pass if you comment it out.
	JSValue test = widget.GetProperty("name");

	JSValue result = js_context.JSEvaluateScript("var widget = new Widget(); widget.name = 'newbar'; widget");
	XCTAssertTrue(result.IsObject());
	JSObject new_widget = static_cast<JSObject>(result);

	JSValue name = new_widget.GetProperty("name");
	XCTAssertTrue(name.IsString());
	XCTAssertEqual("newbar", static_cast<std::string>(name));
}

- (void)testEvaluateNewWidgetPropertyByEval2
{
	JSContext js_context = js_context_group.CreateContext();
	JSObject global_object = js_context.get_global_object();

	XCTAssertFalse(global_object.HasProperty("Widget"));
	JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
	global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
	XCTAssertTrue(global_object.HasProperty("Widget"));

	// This innocent looking line mysteriously causes the crash. The unit test
	// will pass if you comment it out.
	JSValue test = js_context.JSEvaluateScript("Widget.name;");

	JSValue result = js_context.JSEvaluateScript("var widget = new Widget(); widget.name = 'newbar'; widget");
	XCTAssertTrue(result.IsObject());
	JSObject new_widget = static_cast<JSObject>(result);

	JSValue name = new_widget.GetProperty("name");
	XCTAssertTrue(name.IsString());
	XCTAssertEqual("newbar", static_cast<std::string>(name));
}

/* Create multiple instances and see if properties are set correctly */
- (void)testEvaluateMultipleNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget1"));
  JSObject widget1 = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget1", widget1, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
  XCTAssertTrue(global_object.HasProperty("Widget1"));
  
  XCTAssertFalse(global_object.HasProperty("Widget2"));
  JSObject widget2 = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget2", widget2, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
  XCTAssertTrue(global_object.HasProperty("Widget2"));
  
  JSValue test1 = js_context.JSEvaluateScript("Widget1.name = 'bar'; Widget1.sayHello();");
  JSValue test2 = js_context.JSEvaluateScript("Widget2.name = 'baz'; Widget2.sayHello();");
  
  XCTAssertTrue(test1.IsString());
  XCTAssertTrue(test2.IsString());
  
  XCTAssertEqual("Hello, bar. Your number is 42.", static_cast<std::string>(test1));
  XCTAssertEqual("Hello, baz. Your number is 42.", static_cast<std::string>(test2));

  JSValue test3 = js_context.JSEvaluateScript("new Widget1('foo', 456).sayHello();");
  JSValue test4 = js_context.JSEvaluateScript("new Widget2('bar', 234).sayHello();");
  
  XCTAssertEqual("Hello, foo. Your number is 456.", static_cast<std::string>(test3));
  XCTAssertEqual("Hello, bar. Your number is 234.", static_cast<std::string>(test4));
}

/*
 * Call function with callback on Widget
 */
- (void)testFunctionCallback1
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
    Widget.sayHelloWithCallback(function(name,number) { return 'Hello, '+name+' ['+number+']!'; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, world [42]!", static_cast<std::string>(result));
}

/*
 * Call function with callback using "this"
 */
- (void)testFunctionCallback2
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
  Widget.sayHelloWithCallback(function(name,number) { return 'Hello, '+this.name+' ['+this.number+']!'; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, world [42]!", static_cast<std::string>(result));
}

/*
 * Call function with callback using "this" and check if string property works
 */
- (void)testFunctionCallback3
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
  Widget.sayHelloWithCallback(function(name,number) { return this.name==name; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using "this" and check if number property works
 */
- (void)testFunctionCallback4
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
  Widget.sayHelloWithCallback(function(name,number) { return this.number==number; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using nested function
 */
- (void)testFunctionCallback5
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
  Widget.sayHelloWithCallback(
    function(name,number) {
     return (function(obj) {
        return obj.name==name;
      }
     )(this);
    });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using "this" and check if object property works
 */
- (void)testFunctionCallback6
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  const std::string script = R"JS(
  Widget.sayHelloWithCallback(function(name,number) {
    this.value.test = name;
    return this.value;
  });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsObject());
  JSObject resultObj = static_cast<JSObject>(result);
  XCTAssertTrue(resultObj.HasProperty("test"));
  XCTAssertTrue(resultObj.GetProperty("test").IsString());
  XCTAssertEqual("world", static_cast<std::string>(resultObj.GetProperty("test")));
}

/*
 * Call function with callback for new Widget
 */
- (void)testFunctionCallback1ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");
  
  const std::string script = R"JS(
    widget.sayHelloWithCallback(function(name,number) { return 'Hello, '+name+' ['+number+']!'; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, bar [456]!", static_cast<std::string>(result));
}

/*
 * Call function with callback using "this" for new Widget
 */
- (void)testFunctionCallback2ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");
  
  const std::string script = R"JS(
  widget.sayHelloWithCallback(function(name,number) { return 'Hello, '+this.name+' ['+this.number+']!'; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, bar [456]!", static_cast<std::string>(result));
}

/*
 * Call function with callback using "this" and check if string property works
 */
- (void)testFunctionCallback3ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");

  const std::string script = R"JS(
  widget.sayHelloWithCallback(function(name,number) { return this.name==name; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using "this" and check if number property works
 */
- (void)testFunctionCallback4ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");

  const std::string script = R"JS(
  widget.sayHelloWithCallback(function(name,number) { return this.number==number; });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using nested function
 */
- (void)testFunctionCallback5ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");

  const std::string script = R"JS(
  widget.sayHelloWithCallback(
    function(name,number) {
     return (function(obj) {
        return obj.name==name;
      }
     )(this);
    });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

/*
 * Call function with callback using "this" and check if object property works
 */
- (void)testFunctionCallback6ForNewWidget
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  js_context.JSEvaluateScript("var widget = new Widget('bar',456);");
  
  const std::string script = R"JS(
  widget.sayHelloWithCallback(function(name,number) {
    this.value.test = name;
    return this.value;
  });
  )JS";
  
  JSValue result = js_context.JSEvaluateScript(script);
  XCTAssertTrue(result.IsObject());
  JSObject resultObj = static_cast<JSObject>(result);
  XCTAssertTrue(resultObj.HasProperty("test"));
  XCTAssertTrue(resultObj.GetProperty("test").IsString());
  XCTAssertEqual("bar", static_cast<std::string>(resultObj.GetProperty("test")));
}

- (void)testJSExportFindObjectFromPrivateData
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();

  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));

  auto widget_ptr = widget.GetPrivate<Widget>();
  XCTAssertNotEqual(nullptr, widget_ptr);

  XCTAssertFalse(global_object.HasProperty("jsobject"));
  auto jsobject = JSObject::FindJSObjectFromPrivateData(js_context, widget_ptr.get());
  XCTAssertFalse(jsobject.IsError());
  global_object.SetProperty("jsobject", jsobject);
  XCTAssertTrue(global_object.HasProperty("jsobject"));

  auto result = js_context.JSEvaluateScript("Widget === jsobject;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}

- (void)testJSExportFindObjectFromPrivateDataForCallAsConstructor
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();

  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));

  // widget = new Widget('foo', 123);
  XCTAssertFalse(global_object.HasProperty("widget"));
  const std::vector<JSValue> args = {js_context.CreateString("foo"), js_context.CreateNumber(123)};
  JSObject js_widget = widget.CallAsConstructor(args);
  global_object.SetProperty("widget", js_widget);
  XCTAssertTrue(global_object.HasProperty("widget"));

  auto widget_ptr = js_widget.GetPrivate<Widget>();
  XCTAssertNotEqual(nullptr, widget_ptr);

  XCTAssertFalse(global_object.HasProperty("jsobject"));
  auto jsobject = JSObject::FindJSObjectFromPrivateData(js_context, widget_ptr.get());
  XCTAssertFalse(jsobject.IsError());
  global_object.SetProperty("jsobject", jsobject);
  XCTAssertTrue(global_object.HasProperty("jsobject"));

  auto result = js_context.JSEvaluateScript("widget === jsobject;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));

  result = js_context.JSEvaluateScript("Widget === jsobject;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertFalse(static_cast<bool>(result));
}

- (void)testJSExportFindObjectFromPrivateDataGetObject
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  auto result = js_context.JSEvaluateScript("Widget.helloCallback(function(name){return 'Hello, '+name;});");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, Callback", static_cast<std::string>(result));
  
  result = js_context.JSEvaluateScript("Widget.helloCallback(function(name){return this === Widget; });");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));

}

- (void)testJSExportPostConstruct
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  auto result = js_context.JSEvaluateScript("Widget.test_postInitialize_called;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
  
  result = js_context.JSEvaluateScript("Widget.test_postCallAsConstructor_called;");
  XCTAssertTrue(result.IsUndefined());
}

- (void)testJSExportPostConstructForNewObject
{
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));
  
  // widget = new Widget('foo', 123);
  XCTAssertFalse(global_object.HasProperty("widget"));
  const std::vector<JSValue> args = {js_context.CreateString("foo"), js_context.CreateNumber(123)};
  JSObject js_widget = widget.CallAsConstructor(args);
  global_object.SetProperty("widget", js_widget);
  XCTAssertTrue(global_object.HasProperty("widget"));
  
  auto result = js_context.JSEvaluateScript("widget.test_postInitialize_called;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
  
  result = js_context.JSEvaluateScript("widget.test_postCallAsConstructor_called;");
  XCTAssertTrue(result.IsBoolean());
  XCTAssertTrue(static_cast<bool>(result));
}
@end
