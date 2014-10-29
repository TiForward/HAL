//
//  JSClassDefinitionBuilderTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/22/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#include "NativeObject.hpp"
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

- (void)testObjectCallbacks {
  InitializeCallback<NativeObject>        InitializeCallback        = &NativeObject::Initialize;
  FinalizeCallback<NativeObject>          FinalizeCallback          = &NativeObject::Finalize;
  CallAsConstructorCallback<NativeObject> CallAsConstructorCallback = &NativeObject::Constructor;
  CallAsFunctionCallback<NativeObject>    Helloallback              = &NativeObject::Hello;
  CallAsFunctionCallback<NativeObject>    GoodbyeCallback           = &NativeObject::Goodbye;
}

- (void)testPropertyCallbacks {
  using NativeObjectPropertyCallback     = JSNativeObjectFunctionPropertyCallback<NativeObject>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectFunctionPropertyCallbackHash<NativeObject>;
  using NativeObjectFunctionCallbackSet  = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  NativeObjectFunctionCallbackSet function_callbacks;

  JSNativeObjectFunctionPropertyCallback<NativeObject>   HelloCallback("hello"  , &NativeObject::Hello  , {JSPropertyAttribute::DontDelete});
  JSNativeObjectFunctionPropertyCallback<NativeObject> GoodbyeCallback("goodbye", &NativeObject::Goodbye, {JSPropertyAttribute::DontDelete});
  
  XCTAssertEqual(0, function_callbacks.count(HelloCallback));
  XCTAssertEqual(0, function_callbacks.count(GoodbyeCallback));
  
  const auto insert_result = function_callbacks.insert(HelloCallback);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, function_callbacks.count(HelloCallback));
  XCTAssertEqual(*insert_result.first, HelloCallback);
  
  XCTAssertEqual(1, function_callbacks.size());
}

- (void)testValuePropertyCallback {
  using NativeObjectPropertyCallback     = JSNativeObjectValuePropertyCallback<NativeObject>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectValuePropertyCallbackHash<NativeObject>;
  using NativeObjectValueCallbackSet     = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  NativeObjectValueCallbackSet value_property_callbacks;

  JSNativeObjectValuePropertyCallback<NativeObject>   NamePropertyCallback("name"  , &NativeObject::GetName  , &NativeObject::SetName  , {JSPropertyAttribute::DontDelete});
  JSNativeObjectValuePropertyCallback<NativeObject> NumberPropertyCallback("number", &NativeObject::GetNumber, &NativeObject::SetNumber, {JSPropertyAttribute::DontDelete});
  JSNativeObjectValuePropertyCallback<NativeObject>     PiPropertyCallback("pi"    , &NativeObject::GetPi    , nullptr                 , {JSPropertyAttribute::DontDelete});
  
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

- (void)testGetFunctionName {
  JSContext js_context = js_context_group.CreateContext();

  auto foo = js_context.CreateFunction("foo", {}, "return arguments.callee.toString().match(/function ([^\(]+)/)[1];");
  XCTAssertTrue(foo.IsFunction());
  XCTAssertTrue(foo.IsObject());
  std::clog << "MDL: foo() = " << foo() << std::endl;
  XCTAssertEqual("foo", static_cast<std::string>(foo()));
  XCTAssertEqual("foo", static_cast<std::string>(foo(js_context.CreateNumber(42))));

  auto add = js_context.CreateFunction("add", {"a", "b"}, "return a + b;");
  std::clog << "MDL: add = "
            << add({js_context.CreateNumber(2), js_context.CreateNumber(2)})
            << std::endl;
}

- (void)testJSNativeClassBuilder {
  JSNativeClassBuilder<NativeObject> builder("MyClass");
  builder
      .initialize_callback(&NativeObject::Initialize)
      .finalize_callback(&NativeObject::Finalize)
      // .call_as_constructor_callback(&NativeObject::Constructor)
      // .has_instance_callback(&NativeObject::HasInstance)
      .AddValuePropertyCallback("name", &NativeObject::GetName, &NativeObject::SetName)
      .AddValuePropertyCallback("number", &NativeObject::GetNumber, &NativeObject::SetNumber)
      .AddValuePropertyCallback("pi", &NativeObject::GetPi)
      .AddFunctionPropertyCallback("hello", &NativeObject::Hello)
      .AddFunctionPropertyCallback("goodbye", &NativeObject::Goodbye)
      // .get_property_callback(&NativeObject::GetProperty)
      // .set_property_callback(&NativeObject::SetProperty)
      // .delete_property_callback(&NativeObject::DeleteProperty)
      // .get_property_names_callback(&NativeObject::GetPropertyNames)
      // .has_property_callback(&NativeObject::HasProperty)
      // .call_as_function_callback(&NativeObject::CallAsFunction)
      // .convert_to_type_callback(&NativeObject::ConvertToType);
      ;

  auto native_class = builder.build();
  JSContext js_context = js_context_group.CreateContext(native_class);

  //std::vector<JSStaticValue> js_static_values;
  //XCTAssertTrue(js_static_values.empty());
}

- (void)testJSNativeObject {
  JSContext js_context = js_context_group.CreateContext();
  NativeObject native_object(js_context);
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
