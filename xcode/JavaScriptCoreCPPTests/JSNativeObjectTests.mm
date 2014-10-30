/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/RAII.hpp"
#include "Widget.hpp"
#import <XCTest/XCTest.h>

using namespace JavaScriptCoreCPP::RAII;

@interface JSClassDefinitionBuilderTests : XCTestCase
@end

@implementation JSClassDefinitionBuilderTests {
  JSContextGroup js_context_group;
}

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testJSPropertyAttribute {
  std::unordered_set<JSNativeClassAttribute> attributes;
  XCTAssertEqual(0, attributes.count(JSNativeClassAttribute::None));
  XCTAssertEqual(0, attributes.count(JSNativeClassAttribute::NoAutomaticPrototype));
  
  const auto insert_result = attributes.insert(JSNativeClassAttribute::NoAutomaticPrototype);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, attributes.count(JSNativeClassAttribute::NoAutomaticPrototype));
  XCTAssertEqual(*insert_result.first, JSNativeClassAttribute::NoAutomaticPrototype);
  
  XCTAssertEqual(1, attributes.size());
}

- (void)testValuePropertyCallback {
  using NativeObjectPropertyCallback     = JSNativeObjectValuePropertyCallback<Widget>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectValuePropertyCallbackHash<Widget>;
  using NativeObjectValueCallbackSet     = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  NativeObjectValueCallbackSet value_property_callbacks;

  JSNativeObjectValuePropertyCallback<Widget>   NamePropertyCallback("name"  , &Widget::GetName  , &Widget::SetName  , {JSPropertyAttribute::DontDelete});
  JSNativeObjectValuePropertyCallback<Widget> NumberPropertyCallback("number", &Widget::GetNumber, &Widget::SetNumber, {JSPropertyAttribute::DontDelete});
  JSNativeObjectValuePropertyCallback<Widget>     PiPropertyCallback("pi"    , &Widget::GetPi    , nullptr                 , {JSPropertyAttribute::DontDelete});
  
  XCTAssertEqual(1, NamePropertyCallback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(0, NamePropertyCallback.get_attributes().count(JSPropertyAttribute::ReadOnly));

  XCTAssertEqual(1, NumberPropertyCallback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(0, NumberPropertyCallback.get_attributes().count(JSPropertyAttribute::ReadOnly));

  XCTAssertEqual(1, PiPropertyCallback.get_attributes().count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(1, PiPropertyCallback.get_attributes().count(JSPropertyAttribute::ReadOnly));

  XCTAssertEqual(0, value_property_callbacks.count(NamePropertyCallback));
  XCTAssertEqual(0, value_property_callbacks.count(NumberPropertyCallback));
  XCTAssertEqual(0, value_property_callbacks.count(PiPropertyCallback));
  
  const auto insert_name_result = value_property_callbacks.insert(NamePropertyCallback);
  XCTAssertTrue(insert_name_result.second);
  XCTAssertEqual(1, value_property_callbacks.count(NamePropertyCallback));
  XCTAssertEqual(*insert_name_result.first, NamePropertyCallback);
  XCTAssertEqual(1, value_property_callbacks.size());

  const auto insert_pi_result = value_property_callbacks.insert(PiPropertyCallback);
  XCTAssertTrue(insert_pi_result.second);
  XCTAssertEqual(1, value_property_callbacks.count(PiPropertyCallback));
  XCTAssertEqual(*insert_pi_result.first, PiPropertyCallback);
  XCTAssertEqual(2, value_property_callbacks.size());
}

- (void)testFunctionPropertyCallbacks {
  using NativeObjectPropertyCallback     = JSNativeObjectFunctionPropertyCallback<Widget>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectFunctionPropertyCallbackHash<Widget>;
  using NativeObjectFunctionCallbackSet  = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  JSNativeObjectFunctionPropertyCallback<Widget> HelloCallback("hello", &Widget::SayHello, {JSPropertyAttribute::DontDelete});
  
  NativeObjectFunctionCallbackSet function_callbacks;
  XCTAssertEqual(0, function_callbacks.count(HelloCallback));
  
  const auto insert_result = function_callbacks.insert(HelloCallback);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, function_callbacks.count(HelloCallback));
  XCTAssertEqual(*insert_result.first, HelloCallback);
  
  XCTAssertEqual(1, function_callbacks.size());
}

- (void)testJSNativeClassBuilder {
  JSNativeClassBuilder<Widget> builder("Widget");
  builder
      // .Initialize(&Widget::Initialize)
      // .Finalize(&Widget::Finalize)
      .Constructor(&Widget::Constructor)
      // .HasInstance(&Widget::HasInstance)
      .AddValueProperty("name", &Widget::GetName, &Widget::SetName)
      .AddValueProperty("number", &Widget::GetNumber, &Widget::SetNumber)
      .AddValueProperty("pi", &Widget::GetPi)
      .AddFunctionProperty("hello", &Widget::SayHello)
      // .GetProperty(&Widget::GetProperty)
      // .SetProperty(&Widget::SetProperty)
      // .DeleteProperty(&Widget::DeleteProperty)
      // .GetPropertyNames(&Widget::GetPropertyNames)
      // .HasProperty(&Widget::HasProperty)
      // .Function(&Widget::CallAsFunction)
      // .ConvertType(&Widget::ConvertToType);
      ;
  
  auto native_class = builder.build();
}

- (void)testJSNativeObject {
  JSContext js_context = js_context_group.CreateContext();
  auto global_object = js_context.get_global_object();

  auto widget = js_context.CreateObject<Widget>();
  global_object.SetProperty("Widget", widget);
      
  
  JSString script =
      "Widget.sayHello();"
      // "var widget = new Widget();"
      // "widget.hello();"
      ;
  js_context.JSEvaluateScript(script);
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testPerformanceExample {
  // This is an example of a performance test case.
  [self measureBlock:^{
    // Put the code you want to measure the time of here.
  }];
}
#endif

@end
