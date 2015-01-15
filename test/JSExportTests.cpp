/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/HAL.hpp"
#include "Widget.hpp"
#include <functional>

#include "gtest/gtest.h"

#define XCTAssertEqual    ASSERT_EQ
#define XCTAssertNotEqual ASSERT_NE
#define XCTAssertTrue     ASSERT_TRUE
#define XCTAssertFalse    ASSERT_FALSE

using namespace HAL;

class JSExportTests : public testing::Test {
 protected:
  virtual void SetUp() {
  }
  
  virtual void TearDown() {
  }
  
  JSContextGroup js_context_group;
};

TEST_F(JSExportTests, JSPropertyAttribute) {
  std::unordered_set<JSClassAttribute> attributes;
  XCTAssertEqual(0, attributes.count(JSClassAttribute::None));
  XCTAssertEqual(0, attributes.count(JSClassAttribute::NoAutomaticPrototype));
  
  const auto insert_result = attributes.insert(JSClassAttribute::NoAutomaticPrototype);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, attributes.count(JSClassAttribute::NoAutomaticPrototype));
  XCTAssertEqual(*insert_result.first, JSClassAttribute::NoAutomaticPrototype);
  
  XCTAssertEqual(1, attributes.size());
}

TEST_F(JSExportTests, ValuePropertyCallback) {
  detail::JSExportNamedValuePropertyCallback<Widget>   name_callback("name"  , std::mem_fn(&Widget::js_get_name)  , std::mem_fn(&Widget::js_set_name)  , {JSPropertyAttribute::DontDelete});
  detail::JSExportNamedValuePropertyCallback<Widget> number_callback("number", std::mem_fn(&Widget::js_get_number), std::mem_fn(&Widget::js_set_number), {JSPropertyAttribute::DontDelete});
  detail::JSExportNamedValuePropertyCallback<Widget>     pi_callback("pi"    , std::mem_fn(&Widget::js_get_pi)    , nullptr                            , {JSPropertyAttribute::DontDelete});
  
  XCTAssertEqual(1, name_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(0, name_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));
  
  XCTAssertEqual(1, number_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(0, number_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));
  
  XCTAssertEqual(1, pi_callback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(1, pi_callback.get_attributes().count(JSPropertyAttribute::ReadOnly));
}

TEST_F(JSExportTests, JSExportClassDefinitionBuilder) {
  detail::JSExportClassDefinitionBuilder<Widget> builder("Widget");
  builder
      .AddValueProperty("name", std::mem_fn(&Widget::js_get_name), std::mem_fn(&Widget::js_set_name))
      .AddValueProperty("number", std::mem_fn(&Widget::js_get_number), std::mem_fn(&Widget::js_set_number))
      .AddValueProperty("pi", std::mem_fn(&Widget::js_get_pi))
      .AddFunctionProperty("sayhello", std::mem_fn(&Widget::js_sayHello))
  // .Function(&Widget::CallAsFunction)
  // .ConvertType(&Widget::ConvertToType);
  ;
  
  auto native_class = builder.build();
}

TEST_F(JSExportTests, JSExport) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject<Widget>();
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

  result = js_context.JSEvaluateScript("Widget.sayHello();");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, world. Your number is 42.", static_cast<std::string>(result));
  
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
  
  result = js_context.JSEvaluateScript("Widget.number = 3*7;");
  result = js_context.JSEvaluateScript("Widget.number;");
  XCTAssertTrue(result.IsNumber());
  XCTAssertEqual(21, static_cast<std::uint32_t>(result));
  
  result = js_context.JSEvaluateScript("Widget.sayHello();");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, foo. Your number is 21.", static_cast<std::string>(result));

  // Test getting access to the underlying C++ object that implements the
  // JavaScript object.
  auto widget_ptr = widget.GetPrivate<Widget>();
  XCTAssertNotEqual(nullptr, widget_ptr.get());
  
  widget_ptr -> set_name("bar");
  result = js_context.JSEvaluateScript("Widget.name;");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("bar", static_cast<std::string>(result));
  
  widget_ptr -> set_number(4 * 8);
  result = js_context.JSEvaluateScript("Widget.number;");
  XCTAssertTrue(result.IsNumber());
  XCTAssertEqual(32, static_cast<std::uint32_t>(result));
  
  result = js_context.JSEvaluateScript("Widget.sayHello();");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, bar. Your number is 32.", static_cast<std::string>(result));
  
  result = js_context.JSEvaluateScript("var widget = new Widget('baz', 999); widget.sayHello();");
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, baz. Your number is 999.", static_cast<std::string>(result));

  // FIXME
  auto string_ptr = widget.GetPrivate<std::string>();
  //XCTAssertEqual(nullptr, string_ptr);
}

/*
 * Call Widget.sayHello() through operator()
 */
TEST_F(JSExportTests, CallAsFunction) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
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
TEST_F(JSExportTests, New) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
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

/*
 * Call sayHello() through operator() over CallAsConstructor-ed object
 */
TEST_F(JSExportTests, CallAsConstructor) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget);
  XCTAssertTrue(global_object.HasProperty("Widget"));

  const std::vector<JSValue> args = {js_context.CreateString("foo"), js_context.CreateNumber(123)};
  JSObject js_widget = widget.CallAsConstructor(args);
  auto widget_ptr = js_widget.GetPrivate<Widget>();
  XCTAssertTrue(widget_ptr.get());
  XCTAssertTrue(js_widget.HasProperty("sayHello"));
  JSValue widget_sayHello_property = js_widget.GetProperty("sayHello");
  XCTAssertTrue(widget_sayHello_property.IsObject());
  JSObject widget_sayHello = static_cast<JSObject>(widget_sayHello_property);
  XCTAssertTrue(widget_sayHello.IsFunction());
  JSValue result = widget_sayHello(js_widget);
  XCTAssertTrue(result.IsString());
  XCTAssertEqual("Hello, foo. Your number is 123.", static_cast<std::string>(result));
}

TEST_F(JSExportTests, JSExportFinalize) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
  {
    XCTAssertFalse(global_object.HasProperty("Widget"));
    JSObject Widget_ = js_context.CreateObject(JSExport<Widget>::Class());
    global_object.SetProperty("Widget", Widget_);
    XCTAssertTrue(global_object.HasProperty("Widget"));
    
    {
      const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
      JSObject widget = Widget_.CallAsConstructor(args);
      global_object.SetProperty("widget", widget);
    }
  }
  
  global_object.SetProperty("Widget", js_context.CreateNull());
  global_object.SetProperty("widget", js_context.CreateNull());
  
  js_context.GarbageCollect();
}

TEST_F(JSExportTests, JSExportFinalize2) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();
  
  for (int i = 0; i < 10000; i++) {
    {
      XCTAssertFalse(global_object.HasProperty("Widget"));
      JSObject Widget_ = js_context.CreateObject(JSExport<Widget>::Class());
      global_object.SetProperty("Widget", Widget_);
      XCTAssertTrue(global_object.HasProperty("Widget"));
    
      {
        const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
        JSObject widget = Widget_.CallAsConstructor(args);
        global_object.SetProperty("widget", widget);
      }
    }
  
    global_object.DeleteProperty("Widget");
    global_object.DeleteProperty("widget");
    
    XCTAssertFalse(global_object.HasProperty("Widget"));
    XCTAssertFalse(global_object.HasProperty("widget"));
  
    js_context.GarbageCollect();
  }
}

TEST_F(JSExportTests, EvaluateNewWidgetProperty) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();

  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
  XCTAssertTrue(global_object.HasProperty("Widget"));

  auto test = widget.GetProperty("name");

  const std::vector<JSValue> args = {js_context.CreateString("newbar"), js_context.CreateNumber(123)};
  JSObject js_widget = widget.CallAsConstructor(args);
  auto widget_ptr = js_widget.GetPrivate<Widget>();
  XCTAssertTrue(widget_ptr.get());
  XCTAssertTrue(js_widget.HasProperty("name"));
  JSValue widget_sayHello_property = js_widget.GetProperty("name");
  XCTAssertTrue(widget_sayHello_property.IsString());
  XCTAssertEqual("newbar", static_cast<std::string>(widget_sayHello_property));
}

TEST_F(JSExportTests, EvaluateNewWidgetPropertyByEval) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();

  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
  XCTAssertTrue(global_object.HasProperty("Widget"));

  auto test = widget.GetProperty("name");

  auto result = js_context.JSEvaluateScript("var widget = new Widget(); widget.name = 'newbar'; widget");
  XCTAssertTrue(result.IsObject());
  auto new_widget = static_cast<JSObject>(result);

  auto name = new_widget.GetProperty("name");
  XCTAssertTrue(name.IsString());
  XCTAssertEqual("newbar", static_cast<std::string>(name));
}

TEST_F(JSExportTests, EvaluateNewWidgetPropertyByEval2) {
  JSContext js_context = js_context_group.CreateContext();
  JSObject global_object   = js_context.get_global_object();

  XCTAssertFalse(global_object.HasProperty("Widget"));
  JSObject widget = js_context.CreateObject(JSExport<Widget>::Class());
  global_object.SetProperty("Widget", widget, {JSPropertyAttribute::ReadOnly, JSPropertyAttribute::DontDelete});
  XCTAssertTrue(global_object.HasProperty("Widget"));

  auto test = js_context.JSEvaluateScript("Widget.name;");

  auto result = js_context.JSEvaluateScript("var widget = new Widget(); widget.name = 'newbar'; widget");
  XCTAssertTrue(result.IsObject());
  auto new_widget = static_cast<JSObject>(result);

  auto name = new_widget.GetProperty("name");
  XCTAssertTrue(name.IsString());
  XCTAssertEqual("newbar", static_cast<std::string>(name));
}
