//
//  JSObjectTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/19/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSObject.hpp"
#include "JavaScriptCoreCPP/RAII/JSArray.hpp"
#include "JavaScriptCoreCPP/RAII/JSNumber.hpp"
#import <XCTest/XCTest.h>

namespace UnitTestConstants {
  static const double pi { 3.141592653589793 };
}

using namespace JavaScriptCoreCPP;
@interface JSObjectTests2 : XCTestCase
@end

@implementation JSObjectTests2 {
  JSContext js_context;
}

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testJSObject {
  JSObject js_object(js_context);
  JSValue prototype = js_object.GetPrototype();
  XCTAssertFalse(prototype.IsUndefined());
  XCTAssertFalse(prototype.IsNull());
  XCTAssertFalse(prototype.IsBoolean());
  XCTAssertFalse(prototype.IsNumber());
  XCTAssertFalse(prototype.IsString());
  XCTAssertTrue(prototype.IsObject());

  XCTAssertFalse(js_object.HasProperty("foo"));
  XCTAssertTrue(js_object.GetProperty("foo").IsUndefined());
  
  js_object.SetProperty("foo", JSNumber(42, js_context));
  XCTAssertTrue(js_object.HasProperty("foo"));
  JSNumber foo = js_object.GetProperty("foo");
  XCTAssertEqual(42, foo);
  XCTAssertTrue(js_object.DeleteProperty("foo"));

  js_object.SetProperty("bar", JSNumber(42, js_context), {JSPropertyAttribute::DontDelete});
  XCTAssertTrue(js_object.HasProperty("bar"));
  JSNumber bar = js_object.GetProperty("bar");
  XCTAssertEqual(42, bar);
  XCTAssertFalse(js_object.DeleteProperty("bar"));
  XCTAssertTrue(js_object.HasProperty("bar"));
  bar = js_object.GetProperty("bar");
  XCTAssertEqual(42, bar);

  XCTAssertTrue(js_object.GetPropertyAtIndex(42).IsUndefined());
  js_object.SetPropertyAtIndex(42, JSNumber(UnitTestConstants::pi, js_context));
  JSNumber pi = js_object.GetPropertyAtIndex(42);
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, pi, std::numeric_limits<double>::epsilon());

  XCTAssertFalse(js_object.IsFunction());
  
  try {
    js_object.CallAsFunction({});
    XCTFail("js_object was called as a function but did not throw a std::runtime_error exception");
  } catch (const std::runtime_error& exception) {
    XCTAssert(YES, @"Caught expected std::runtime_error exception.");
  } catch (...) {
    XCTFail("Caught unexpected unknown exception, but we expected a std::runtime_error exception.");
  }

  XCTAssertFalse(js_object.IsConstructor());
  
  try {
    js_object.CallAsConstructor({});
    XCTFail("js_object was called as a constructor but did not throw a std::runtime_error exception");
  } catch (const std::runtime_error& exception) {
    XCTAssert(YES, @"Caught expected std::runtime_error exception.");
  } catch (...) {
    XCTFail("Caught unexpected unknown exception, but we expected a std::runtime_error exception.");
  }
  
  const auto property_names = js_object.GetPropertyNames();
  XCTAssertEqual(2, property_names.size());
  for (const auto& property_name : property_names) {
    std::clog << "MDL: property_name = " << property_name << std::endl;
  }
}

- (void)testJSArray {
  JSArray js_array(js_context);
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
