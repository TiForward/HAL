/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"
#include <unordered_set>
#include <limits>
#import <XCTest/XCTest.h>

using namespace JavaScriptCoreCPP;

namespace UnitTestConstants {
  static const double pi { 3.141592653589793 };
}

@interface JSObjectTests2 : XCTestCase
@end

@implementation JSObjectTests2 {
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
  std::unordered_set<JSPropertyAttribute> attributes;
  XCTAssertEqual(0, attributes.count(JSPropertyAttribute::None));
  XCTAssertEqual(0, attributes.count(JSPropertyAttribute::ReadOnly));
  XCTAssertEqual(0, attributes.count(JSPropertyAttribute::DontEnum));
  XCTAssertEqual(0, attributes.count(JSPropertyAttribute::DontDelete));
  
  const auto insert_result = attributes.insert(JSPropertyAttribute::DontDelete);
  XCTAssertTrue(insert_result.second);
  XCTAssertEqual(1, attributes.count(JSPropertyAttribute::DontDelete));
  XCTAssertEqual(*insert_result.first, JSPropertyAttribute::DontDelete);

  XCTAssertEqual(1, attributes.size());
}

- (void)testJSObject {
  JSContext js_context = js_context_group.CreateContext();
  JSObject js_object = js_context.CreateObject();
  JSValue prototype = js_object.GetPrototype();
  XCTAssertFalse(prototype.IsUndefined());
  XCTAssertFalse(prototype.IsNull());
  XCTAssertFalse(prototype.IsBoolean());
  XCTAssertFalse(prototype.IsNumber());
  XCTAssertFalse(prototype.IsString());
  XCTAssertTrue(prototype.IsObject());

  XCTAssertFalse(js_object.HasProperty("foo"));
  XCTAssertTrue(js_object.GetProperty("foo").IsUndefined());
  
  js_object.SetProperty("foo", js_context.CreateNumber(42));
  XCTAssertTrue(js_object.HasProperty("foo"));
  JSNumber foo = js_object.GetProperty("foo");
  XCTAssertEqual(42, static_cast<int32_t>(42));
  XCTAssertTrue(js_object.DeleteProperty("foo"));

  js_object.SetProperty("bar", js_context.CreateBoolean(true), {JSPropertyAttribute::DontDelete});
  XCTAssertTrue(js_object.HasProperty("bar"));
  JSBoolean bar = js_object.GetProperty("bar");
  XCTAssertTrue(bar);
  XCTAssertFalse(js_object.DeleteProperty("bar"));
  XCTAssertTrue(js_object.HasProperty("bar"));
  bar = js_object.GetProperty("bar");
  XCTAssertTrue(bar);

  XCTAssertTrue(js_object.GetProperty(42).IsUndefined());
  js_object.SetProperty(42, js_context.CreateNumber(UnitTestConstants::pi));
  JSNumber pi = js_object.GetProperty(42);
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(pi), std::numeric_limits<double>::epsilon());

  XCTAssertFalse(js_object.IsFunction());
  
  try {
    js_object();
    XCTFail("js_object was called as a function but did not throw a std::runtime_error exception");
  } catch (const std::runtime_error& exception) {
    XCTAssert(YES, @"Caught expected std::runtime_error exception.");
  } catch (...) {
    XCTFail("Caught unexpected unknown exception, but we expected a std::runtime_error exception.");
  }

  XCTAssertFalse(js_object.IsConstructor());
  
  try {
    js_object.CallAsConstructor();
    XCTFail("js_object was called as a constructor but did not throw a std::runtime_error exception");
  } catch (const std::runtime_error& exception) {
    XCTAssert(YES, @"Caught expected std::runtime_error exception.");
  } catch (...) {
    XCTFail("Caught unexpected unknown exception, but we expected a std::runtime_error exception.");
  }
}

- (void)testJSArray {
  JSContext js_context = js_context_group.CreateContext();
  JSArray js_array = js_context.CreateArray();
}

- (void)testJSDate {
  JSContext js_context = js_context_group.CreateContext();
  JSDate js_date = js_context.CreateDate();
}

- (void)testJSError {
  JSContext js_context = js_context_group.CreateContext();
  JSError js_error = js_context.CreateError();
}

- (void)testJSRegExp {
  JSContext js_context = js_context_group.CreateContext();
  JSRegExp js_regexp = js_context.CreateRegExp();
}

- (void)testJSFunction {
  JSContext js_context = js_context_group.CreateContext();
  JSFunction js_function = js_context.CreateFunction("return 'Hello, ' + name;", {"name"});
  XCTAssertTrue(js_function.IsFunction());
  XCTAssertTrue(js_function.IsObject());
  //std::clog << "MDL: js_function(\"world\") = " << js_function("world") << std::endl;
  XCTAssertEqual("Hello, world", static_cast<std::string>(js_function("world")));
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
