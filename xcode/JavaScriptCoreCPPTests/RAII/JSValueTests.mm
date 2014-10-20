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

using namespace JavaScriptCoreCPP;

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
  JSValue js_value = JSUndefined(js_context);
  XCTAssertEqual("undefined", static_cast<std::string>(js_value));
  XCTAssertTrue(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertFalse(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  JSUndefined js_undefined(js_context);
  XCTAssertEqual(js_undefined, js_value);
}

- (void)testJSNull {
  JSValue js_value = JSNull(js_context);
  XCTAssertEqual("null", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertTrue(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertFalse(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  JSNull js_null(js_context);
  XCTAssertEqual(js_null, js_value);
}

- (void)testJSBoolean {
  JSBoolean js_false(false, js_context);
  XCTAssertFalse(js_false);
  
  JSValue js_value = js_false;
  XCTAssertEqual("false", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertTrue(js_value.IsBoolean());
  XCTAssertFalse(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  JSBoolean js_true(true, js_context);
  XCTAssertTrue(js_true);
  
  js_value = js_true;
  XCTAssertEqual("true", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertTrue(js_value.IsBoolean());
  XCTAssertFalse(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  JSBoolean js_boolean(true, js_context);
  XCTAssertTrue(js_boolean);
  js_boolean = false;
  XCTAssertFalse(js_boolean);
}

- (void)testJSNumber {
  JSNumber js_double(UnitTestConstants::pi, js_context);
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(js_double), std::numeric_limits<double>::epsilon());
  
  JSValue js_value = js_double;
  XCTAssertEqual("3.141592653589793", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertTrue(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());
  
  const int32_t int32_value = 42;
  JSNumber js_int32(int32_value, js_context);
  XCTAssertEqual(int32_value, static_cast<int32_t>(js_int32));
  
  js_value = js_int32;
  XCTAssertEqual("42", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertTrue(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());

  const uint32_t uint32_value = 42;
  JSNumber js_uint32(uint32_value, js_context);
  XCTAssertEqual(uint32_value, static_cast<uint32_t>(js_uint32));
  
  js_value = js_uint32;
  XCTAssertEqual("42", static_cast<std::string>(js_value));
  XCTAssertFalse(js_value.IsUndefined());
  XCTAssertFalse(js_value.IsNull());
  XCTAssertFalse(js_value.IsBoolean());
  XCTAssertTrue(js_value.IsNumber());
  XCTAssertFalse(js_value.IsString());
  XCTAssertFalse(js_value.IsObject());

  //  auto piValue_ptr  = context_ptr . evaluateScript("Math.PI");
  //  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*piValue_ptr), std::numeric_limits<double>::epsilon());

  //  auto result_ptr  = context_ptr . evaluateScript("21 / 7");
  //  XCTAssertEqual(3, static_cast<int32_t>(*result_ptr));

  //  auto result_ptr  = context_ptr . evaluateScript("21 / 7");
  //  XCTAssertEqual(3, static_cast<uint32_t>(*result_ptr));
}

- (void)testJSValueToJSONString {
  JSUndefined js_undefined(js_context);
  JSString js_undefined_json = JSValue::ToJSONString(js_undefined);
  XCTAssertEqual(0, js_undefined_json.length());
  //std::clog << "MDL: js_undefined_json = " << js_undefined_json << std::endl;
  
  JSNull js_null(js_context);
  JSString js_null_json = JSValue::ToJSONString(js_null);
  std::clog << "MDL: js_null_json = " << js_null_json << std::endl;
  XCTAssertEqual("null", js_null_json);
  
  JSBoolean js_false(false, js_context);
  JSString js_false_sjon = JSValue::ToJSONString(js_false);
  //std::clog << "MDL: js_false_sjon = " << js_false_sjon << std::endl;
  XCTAssertEqual("false", js_false_sjon);

  JSBoolean js_true(true, js_context);
  JSString js_true_sjon = JSValue::ToJSONString(js_true);
  //std::clog << "MDL: js_true_sjon = " << js_true_sjon << std::endl;
  XCTAssertEqual("true", js_true_sjon);
  
  JSNumber js_double(UnitTestConstants::pi, js_context);
  JSString js_double_sjon = JSValue::ToJSONString(js_double);
  //std::clog << "MDL: js_double_sjon = " << js_double_sjon << std::endl;
  XCTAssertEqual("3.141592653589793", js_double_sjon);

  JSNumber js_int32(int32_t(42), js_context);
  JSString js_int32_sjon = JSValue::ToJSONString(js_int32);
  //std::clog << "MDL: js_int32_sjon = " << js_int32_sjon << std::endl;
  XCTAssertEqual("42", js_int32_sjon);

  JSNumber js_uint32(uint32_t(42), js_context);
  JSString js_uint32_sjon = JSValue::ToJSONString(js_uint32);
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
  
//  auto result_ptr  = context_ptr . evaluateScript("\"hello, JavaScript\"");
//  XCTAssertEqual("hello, JavaScript", static_cast<std::string>(*result_ptr));
}

- (void)testJSContextCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSValue?
    JSUndefined js_value(js_context);
  }];
}

@end
