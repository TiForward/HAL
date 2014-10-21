//
//  JSValueTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/18/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#include "JavaScriptCoreCPP/RAII/JSUndefined.hpp"
#include "JavaScriptCoreCPP/RAII/JSNull.hpp"
#include "JavaScriptCoreCPP/RAII/JSBoolean.hpp"
#include "JavaScriptCoreCPP/RAII/JSNumber.hpp"
#import <XCTest/XCTest.h>

using namespace JavaScriptCoreCPP::RAII;

namespace UnitTestConstants {
  static const double pi { 3.141592653589793 };
}

@interface JSValueTests2 : XCTestCase
@end

@implementation JSValueTests2 {
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

//- (void)testJSValue {
//  JSValue js_value(js_context);
//  XCTAssertTrue(js_value.IsUndefined());
//}

- (void)testJSUndefined {
  JSUndefined js_undefined(js_context);
  XCTAssertEqual("undefined", static_cast<std::string>(js_undefined));
  XCTAssertTrue(js_undefined.IsUndefined());
  XCTAssertFalse(js_undefined.IsNull());
  XCTAssertFalse(js_undefined.IsBoolean());
  XCTAssertFalse(js_undefined.IsNumber());
  XCTAssertFalse(js_undefined.IsString());
  XCTAssertFalse(js_undefined.IsObject());
}

- (void)testJSNull {
  JSNull js_null(js_context);
  XCTAssertEqual("null", static_cast<std::string>(js_null));
  XCTAssertFalse(js_null.IsUndefined());
  XCTAssertTrue(js_null.IsNull());
  XCTAssertFalse(js_null.IsBoolean());
  XCTAssertFalse(js_null.IsNumber());
  XCTAssertFalse(js_null.IsString());
  XCTAssertFalse(js_null.IsObject());
}

- (void)testJSBoolean {
  JSBoolean js_false(false, js_context);
  XCTAssertFalse(js_false);
  
  XCTAssertEqual("false", static_cast<std::string>(js_false));
  XCTAssertFalse(js_false.IsUndefined());
  XCTAssertFalse(js_false.IsNull());
  XCTAssertTrue(js_false.IsBoolean());
  XCTAssertFalse(js_false.IsNumber());
  XCTAssertFalse(js_false.IsString());
  XCTAssertFalse(js_false.IsObject());
  
  JSBoolean js_true(true, js_context);
  XCTAssertTrue(js_true);
  
  XCTAssertEqual("true", static_cast<std::string>(js_true));
  XCTAssertFalse(js_false.IsUndefined());
  XCTAssertFalse(js_false.IsNull());
  XCTAssertTrue(js_false.IsBoolean());
  XCTAssertFalse(js_false.IsNumber());
  XCTAssertFalse(js_false.IsString());
  XCTAssertFalse(js_false.IsObject());
  
  JSBoolean js_boolean(true, js_context);
  XCTAssertTrue(js_boolean);
  js_boolean = false;
  XCTAssertFalse(js_boolean);
}

- (void)testJSNumber {
  JSNumber js_double(UnitTestConstants::pi, js_context);
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(js_double), std::numeric_limits<double>::epsilon());
  
  XCTAssertEqual("3.141592653589793", static_cast<std::string>(js_double));
  XCTAssertFalse(js_double.IsUndefined());
  XCTAssertFalse(js_double.IsNull());
  XCTAssertFalse(js_double.IsBoolean());
  XCTAssertTrue(js_double.IsNumber());
  XCTAssertFalse(js_double.IsString());
  XCTAssertFalse(js_double.IsObject());
  
  const int32_t int32_value = 42;
  JSNumber js_int32(int32_value, js_context);
  XCTAssertEqual(int32_value, static_cast<int32_t>(js_int32));
  
  XCTAssertEqual("42", static_cast<std::string>(js_int32));
  XCTAssertFalse(js_int32.IsUndefined());
  XCTAssertFalse(js_int32.IsNull());
  XCTAssertFalse(js_int32.IsBoolean());
  XCTAssertTrue(js_int32.IsNumber());
  XCTAssertFalse(js_int32.IsString());
  XCTAssertFalse(js_int32.IsObject());

  const uint32_t uint32_value = 42;
  JSNumber js_uint32(uint32_value, js_context);
  XCTAssertEqual(uint32_value, static_cast<uint32_t>(js_uint32));
  
  XCTAssertEqual("42", static_cast<std::string>(js_uint32));
  XCTAssertFalse(js_uint32.IsUndefined());
  XCTAssertFalse(js_uint32.IsNull());
  XCTAssertFalse(js_uint32.IsBoolean());
  XCTAssertTrue(js_uint32.IsNumber());
  XCTAssertFalse(js_uint32.IsString());
  XCTAssertFalse(js_uint32.IsObject());

  //  auto piValue_ptr  = context_ptr . evaluateScript("Math.PI");
  //  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*piValue_ptr), std::numeric_limits<double>::epsilon());

  //  auto result_ptr  = context_ptr . evaluateScript("21 / 7");
  //  XCTAssertEqual(3, static_cast<int32_t>(*result_ptr));

  //  auto result_ptr  = context_ptr . evaluateScript("21 / 7");
  //  XCTAssertEqual(3, static_cast<uint32_t>(*result_ptr));
}

- (void)testToJSONString {
  JSUndefined js_undefined(js_context);
  JSString js_undefined_json = js_undefined.ToJSONString();
  //std::clog << "MDL: js_undefined_json = " << js_undefined_json << std::endl;
  XCTAssertEqual(0, js_undefined_json.length());
  
  JSNull js_null(js_context);
  JSString js_null_json = js_null.ToJSONString();
  std::clog << "MDL: js_null_json = " << js_null_json << std::endl;
  XCTAssertEqual("null", js_null_json);
  
  JSBoolean js_false(false, js_context);
  JSString js_false_sjon = js_false.ToJSONString();
  //std::clog << "MDL: js_false_sjon = " << js_false_sjon << std::endl;
  XCTAssertEqual("false", js_false_sjon);

  JSBoolean js_true(true, js_context);
  JSString js_true_sjon = js_true.ToJSONString();
  //std::clog << "MDL: js_true_sjon = " << js_true_sjon << std::endl;
  XCTAssertEqual("true", js_true_sjon);
  
  JSNumber js_double(UnitTestConstants::pi, js_context);
  JSString js_double_sjon = js_double.ToJSONString();
  //std::clog << "MDL: js_double_sjon = " << js_double_sjon << std::endl;
  XCTAssertEqual("3.141592653589793", js_double_sjon);

  JSNumber js_int32(int32_t(42), js_context);
  JSString js_int32_sjon = js_int32.ToJSONString();
  //std::clog << "MDL: js_int32_sjon = " << js_int32_sjon << std::endl;
  XCTAssertEqual("42", js_int32_sjon);

  JSNumber js_uint32(uint32_t(42), js_context);
  JSString js_uint32_sjon = js_uint32.ToJSONString();
  //std::clog << "MDL: js_uint32_sjon = " << js_uint32_sjon << std::endl;
  XCTAssertEqual("42", js_uint32_sjon);
}

- (void)testString {
  JSValue js_value("hello, world", js_context);
  XCTAssertEqual("hello, world", static_cast<std::string>(js_value));
  
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertFalse(js_value.IsNumber());
  XCTAssertTrue(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  JSString js_string = js_value;
  XCTAssertEqual("hello, world", js_string);
  
//  auto result_ptr  = context_ptr . evaluateScript("\"hello, JavaScript\"");
//  XCTAssertEqual("hello, JavaScript", static_cast<std::string>(*result_ptr));
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testJSContextCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSValue?
    JSUndefined js_value(js_context);
  }];
}
#endif

@end
