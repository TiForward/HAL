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
  CallAsFunctionCallback<NativeObject>         CallAsFunctionCallback         = &NativeObject::CallAsFunction;
  CallAsConstructorCallback<NativeObject>      CallAsConstructorCallback      = &NativeObject::CallAsConstructor;
  HasInstanceCallback<NativeObject>            HasInstanceCallback            = &NativeObject::HasInstance;
  ConvertToTypeCallback<NativeObject>          ConvertToTypeCallback          = &NativeObject::ConvertToType;
}

- (void)testJSNativeObjectFunctionPropertyCallback {
  JSNativeObjectFunctionPropertyCallback<NativeObject> FunctionPropertyCallback("Foo", &NativeObject::CallAsFunction);
//  JSContext js_context = js_context_group.CreateContext();
//  NativeObject native_object(js_context);
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
