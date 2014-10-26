//
//  JSClassDefinitionBuilderTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/22/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#include "NativeObject.hpp"
//#include "JavaScriptCoreCPP/RAII/JSNativeObject.hpp"
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
  std::unordered_set<JSNativeObjectAttribute> attributes;
  XCTAssertEqual(0, attributes.count(JSNativeObjectAttribute::None));
  XCTAssertEqual(0, attributes.count(JSNativeObjectAttribute::NoAutomaticPrototype));
  
  const auto insert_result = attributes.insert(JSNativeObjectAttribute::NoAutomaticPrototype);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, attributes.count(JSNativeObjectAttribute::NoAutomaticPrototype));
  XCTAssertEqual(*insert_result.first, JSNativeObjectAttribute::NoAutomaticPrototype);
  
  XCTAssertEqual(1, attributes.size());
}

- (void)testInitializeCallback {
  InitializeCallback<NativeObject>             InitializeCallback             = &NativeObject::Initialize;
  FinalizeCallback<NativeObject>               FinalizeCallback               = &NativeObject::Finalize;
  HasPropertyCallback<NativeObject>            HasPropertyCallback            = &NativeObject::HasProperty;
  GetPropertyCallback<NativeObject>            GetPropertyCallback            = &NativeObject::GetProperty;
  SetPropertyCallback<NativeObject>            SetPropertyCallback            = &NativeObject::SetProperty;
  DeletePropertyCallback<NativeObject>         DeletePropertyCallback         = &NativeObject::DeleteProperty;
  GetPropertyNamesCallback<NativeObject>       GetPropertyNamesCallback       = &NativeObject::GetPropertyNames;
  CallAsFunctionCallback<NativeObject>         FooCallback                    = &NativeObject::FooFunction;
  CallAsFunctionCallback<NativeObject>         BarCallback                    = &NativeObject::BarFunction;
  CallAsConstructorCallback<NativeObject>      CallAsConstructorCallback      = &NativeObject::Constructor;
  HasInstanceCallback<NativeObject>            HasInstanceCallback            = &NativeObject::HasInstance;
  ConvertToTypeCallback<NativeObject>          ConvertToTypeCallback          = &NativeObject::ConvertToType;
}

- (void)testJSNativeObjectFunctionPropertyCallback {
  using NativeObjectPropertyCallback     = JSNativeObjectFunctionPropertyCallback<NativeObject>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectFunctionPropertyCallbackHash<NativeObject>;
  using NativeObjectFunctionCallbackSet  = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  NativeObjectFunctionCallbackSet function_callbacks;

  JSNativeObjectFunctionPropertyCallback<NativeObject> FooCallback("Foo", &NativeObject::FooFunction);
  JSNativeObjectFunctionPropertyCallback<NativeObject> BarCallback("Bar", &NativeObject::BarFunction);
  
  XCTAssertEqual(0, function_callbacks.count(FooCallback));
  XCTAssertEqual(0, function_callbacks.count(BarCallback));
  
  const auto insert_result = function_callbacks.insert(FooCallback);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, function_callbacks.count(FooCallback));
  XCTAssertEqual(*insert_result.first, FooCallback);
  
  XCTAssertEqual(1, function_callbacks.size());
}

- (void)testJSNativeObjectValuePropertyCallback {
  using NativeObjectPropertyCallback     = JSNativeObjectValuePropertyCallback<NativeObject>;
  using NativeObjectPropertyCallbackHash = JSNativeObjectValuePropertyCallbackHash<NativeObject>;
  using NativeObjectValueCallbackSet     = std::unordered_set<NativeObjectPropertyCallback, NativeObjectPropertyCallbackHash>;
  
  NativeObjectValueCallbackSet value_property_callbacks;

  JSNativeObjectValuePropertyCallback<NativeObject> NamePropertyCallback("name", &NativeObject::GetName, &NativeObject::SetName);
  JSNativeObjectValuePropertyCallback<NativeObject> NumberPropertyCallback("number", &NativeObject::GetNumber, &NativeObject::SetNumber);
  JSNativeObjectValuePropertyCallback<NativeObject> PiPropertyCallback("pi", &NativeObject::GetPi, nullptr);
  
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
