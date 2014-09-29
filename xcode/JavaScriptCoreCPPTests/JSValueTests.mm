//
//  JSValueTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 9/19/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JavaScriptCoreCPP/JSValue.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdexcept>

@interface JSValueTests : XCTestCase
@end

namespace UnitTestConstants {
  static const double pi { 3.141592653589793 };
}

template <typename C>
void printClockData() {
  using namespace std;
    
  cout << "- precision: ";
  // The type of time unit.
  using P = typename C::period;
    
  // If time unit is less or equal to one millisecond.
  if (ratio_less<P, milli>::value) {
    // Convert to and print as milliseconds.
    using TT = typename ratio_multiply<P, kilo>::type;
    cout << fixed << double(TT::num) / TT::den << " milliseconds" << endl;
  } else {
    // print as seconds
    cout << fixed << double(P::num) / P::den << " seconds" << endl;
  }
    
  cout << "- is_steady: " << boolalpha << C::is_steady << endl;
}

inline
std::string asString(const std::chrono::system_clock::time_point& tp) {
  // Convert to system time.
  std::time_t t = std::chrono::system_clock::to_time_t(tp);
    
  // Convert to calendar time.
  //std::string ts = std::ctime(&t);
  std::string ts = std::asctime(std::localtime(&t));
  //std::string ts = std::asctime(std::gmtime(&t));
    
  // Strip trailing newline.
  ts.resize(ts.size() - 1);
    
  return ts;
}

inline
std::chrono::system_clock::time_point
makeTimePoint(int year, int mon, int day, int hour, int min, int sec = 0) {
  struct std::tm tm;
  tm.tm_sec   = sec;         // second of minute (0..59 and 60 for leap seconds)
  tm.tm_min   = min;         // minute of hour (0..59)
  tm.tm_hour  = hour;        // hour of day (0.23)
  tm.tm_mday  = day;         // day of month (0..31)
  tm.tm_mon   = mon - 1;     // month of year (0..11)
  tm.tm_year  = year - 1900; // year since 1900
  tm.tm_isdst = -1;          // determine whether daylight savings time.

  std::time_t time_t = std::mktime(&tm);
  if (time_t == -1) {
    throw std::invalid_argument("not a valid system time");
  }
    
  return std::chrono::system_clock::from_time_t(time_t);
}

@implementation JSValueTests {
  JSContext_ptr_t context_ptr;
}

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each test method in the class.
  context_ptr = JSContext::create();
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each test method in the class.
  [super tearDown];
  context_ptr = nullptr;

  std::cout << "MDL: JSSValue::ctorCounter = " << JSValue::ctorCounter() << std::endl;
  std::cout << "MDL: JSSValue::dtorCounter = " << JSValue::dtorCounter() << std::endl;
}

- (void)testDate {
  std::cout << "system_clock: " << std::endl;
  printClockData<std::chrono::system_clock>();
    
  std::cout << "\nhigh_resolution_clock: " << std::endl;
  printClockData<std::chrono::high_resolution_clock>();
    
  std::cout << "\nsteady_clock: " << std::endl;
  printClockData<std::chrono::steady_clock>();
    
  // Print the epoch of this system clock:
  std::chrono::system_clock::time_point tp;
  std::cout << "epoch: " << asString(tp) << std::endl;
    
  tp = std::chrono::system_clock::now();
  std::cout << "now:   " << asString(tp) << std::endl;

  tp = std::chrono::system_clock::time_point::min();
  //std::cout << "min:   " << asString(tp) << std::endl;

  tp = std::chrono::system_clock::time_point::max();
  //std::cout << "max:   " << asString(tp) << std::endl;
    
  auto tp1 = makeTimePoint(2010, 01, 01, 00, 00);
  std::cout << "tp1: " << asString(tp1) << std::endl;

  auto tp2 = makeTimePoint(2011, 05, 23, 13, 44);
  std::cout << "tp2: " << asString(tp2) << std::endl;
}

- (void)testUndefined {
  auto undefinedValue_ptr = JSValue::valueWithUndefinedInContext(context_ptr);
  XCTAssertEqual("undefined", static_cast<std::string>(*undefinedValue_ptr));
  XCTAssertTrue(undefinedValue_ptr->isUndefined());
  XCTAssertFalse(undefinedValue_ptr->isNull());
  XCTAssertFalse(undefinedValue_ptr->isBoolean());
  XCTAssertFalse(undefinedValue_ptr->isNumber());
  XCTAssertFalse(undefinedValue_ptr->isString());
  XCTAssertFalse(undefinedValue_ptr->isObject());
}

- (void)testBool {
  auto falseValue_ptr = JSValue::valueWithBoolInContext(false, context_ptr);
  XCTAssertFalse(static_cast<bool>(*falseValue_ptr));
  XCTAssertEqual("false", static_cast<std::string>(*falseValue_ptr));

  XCTAssertFalse(falseValue_ptr->isUndefined());
  XCTAssertFalse(falseValue_ptr->isNull());
  XCTAssertTrue(falseValue_ptr->isBoolean());
  XCTAssertFalse(falseValue_ptr->isNumber());
  XCTAssertFalse(falseValue_ptr->isString());
  XCTAssertFalse(falseValue_ptr->isObject());
    
  auto trueValue_ptr = JSValue::valueWithBoolInContext(true, context_ptr);
  XCTAssertTrue(static_cast<bool>(*trueValue_ptr));
  XCTAssertEqual("true", static_cast<std::string>(*trueValue_ptr));

  XCTAssertFalse(trueValue_ptr->isUndefined());
  XCTAssertFalse(trueValue_ptr->isNull());
  XCTAssertTrue(trueValue_ptr->isBoolean());
  XCTAssertFalse(trueValue_ptr->isNumber());
  XCTAssertFalse(trueValue_ptr->isString());
  XCTAssertFalse(trueValue_ptr->isObject());
}

- (void)testDouble {
  auto doubleValue_ptr = JSValue::valueWithDoubleInContext(UnitTestConstants::pi, context_ptr);
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*doubleValue_ptr), std::numeric_limits<double>::epsilon());
  XCTAssertEqual("3.141592653589793", static_cast<std::string>(*doubleValue_ptr));
  
  XCTAssertFalse(doubleValue_ptr->isUndefined());
  XCTAssertFalse(doubleValue_ptr->isNull());
  XCTAssertFalse(doubleValue_ptr->isBoolean());
  XCTAssertTrue(doubleValue_ptr->isNumber());
  XCTAssertFalse(doubleValue_ptr->isString());
  XCTAssertFalse(doubleValue_ptr->isObject());

  auto piValue_ptr  = context_ptr -> evaluateScript("Math.PI");
  XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*piValue_ptr), std::numeric_limits<double>::epsilon());
}

- (void)testInt32 {
  auto intValue_ptr = JSValue::valueWithInt32InContext(42, context_ptr);
  XCTAssertEqual(42, static_cast<int32_t>(*intValue_ptr));
  XCTAssertEqual("42", static_cast<std::string>(*intValue_ptr));

  XCTAssertFalse(intValue_ptr->isUndefined());
  XCTAssertFalse(intValue_ptr->isNull());
  XCTAssertFalse(intValue_ptr->isBoolean());
  XCTAssertTrue(intValue_ptr->isNumber());
  XCTAssertFalse(intValue_ptr->isString());
  XCTAssertFalse(intValue_ptr->isObject());

  auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
  XCTAssertEqual(3, static_cast<int32_t>(*result_ptr));
  
}

- (void)testUInt32 {
  auto uintValue_ptr = JSValue::valueWithUInt32InContext(42, context_ptr);
  XCTAssertEqual(42, static_cast<uint32_t>(*uintValue_ptr));
  XCTAssertEqual("42", static_cast<std::string>(*uintValue_ptr));
  
  XCTAssertFalse(uintValue_ptr->isUndefined());
  XCTAssertFalse(uintValue_ptr->isNull());
  XCTAssertFalse(uintValue_ptr->isBoolean());
  XCTAssertTrue(uintValue_ptr->isNumber());
  XCTAssertFalse(uintValue_ptr->isString());
  XCTAssertFalse(uintValue_ptr->isObject());

  auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
  XCTAssertEqual(3, static_cast<uint32_t>(*result_ptr));
  
}

- (void)testString {
  auto stringValue_ptr = JSValue::valueWithStringInContext("hello, world", context_ptr);
  XCTAssertEqual("hello, world", static_cast<std::string>(*stringValue_ptr));
    
  XCTAssertFalse(stringValue_ptr->isUndefined());
  XCTAssertFalse(stringValue_ptr->isNull());
  XCTAssertFalse(stringValue_ptr->isBoolean());
  XCTAssertFalse(stringValue_ptr->isNumber());
  XCTAssertTrue(stringValue_ptr->isString());
  XCTAssertFalse(stringValue_ptr->isObject());

  auto result_ptr  = context_ptr -> evaluateScript("\"hello, JavaScript\"");
  XCTAssertEqual("hello, JavaScript", static_cast<std::string>(*result_ptr));
  
}

- (void)testObject {
  auto objectValue_ptr = JSValue::valueWithNewObjectInContext(context_ptr);
  XCTAssertFalse(objectValue_ptr->isUndefined());
  XCTAssertFalse(objectValue_ptr->isNull());
  XCTAssertFalse(objectValue_ptr->isBoolean());
  XCTAssertFalse(objectValue_ptr->isNumber());
  XCTAssertFalse(objectValue_ptr->isString());
  XCTAssertTrue(objectValue_ptr->isObject());

  auto result_ptr  = context_ptr -> evaluateScript("new Object()");
  XCTAssertTrue(result_ptr -> isObject());

  result_ptr  = context_ptr -> evaluateScript("Object()");
  XCTAssertTrue(result_ptr -> isObject());
  
  // It is surprising to me that an object literal, "{}", is not an object.
  result_ptr  = context_ptr -> evaluateScript("{}");
  XCTAssertTrue(result_ptr -> isUndefined());

  // But this is an object.
  result_ptr  = context_ptr -> evaluateScript("var a = {}; a");
  XCTAssertTrue(result_ptr -> isObject());
  
  // This is nor a primitive string.
  result_ptr  = context_ptr -> evaluateScript("new String()");
  XCTAssertTrue(result_ptr -> isObject());
  XCTAssertFalse(result_ptr -> isString());

  // Yet this is a primitive string (i.e. without new).
  result_ptr  = context_ptr -> evaluateScript("String()");
  XCTAssertFalse(result_ptr -> isObject());
  XCTAssertTrue(result_ptr -> isString());

  result_ptr  = context_ptr -> evaluateScript("new Date()");
  XCTAssertTrue(result_ptr -> isObject());

  result_ptr  = context_ptr -> evaluateScript("new Array()");
  XCTAssertTrue(result_ptr -> isObject());

  // An array literal is an Object, as expected. Why isn't an object literal,
  // "{}", an Object?
  result_ptr  = context_ptr -> evaluateScript("[]");
  XCTAssertTrue(result_ptr -> isObject());

  result_ptr  = context_ptr -> evaluateScript("[1, 3, 5, 7]");
  XCTAssertTrue(result_ptr -> isObject());
}

- (void)testValueForProperty {
  auto result_ptr  = context_ptr -> evaluateScript("[1, 3, 5, 7]");
  XCTAssertTrue(result_ptr -> isObject());
  XCTAssertTrue(result_ptr -> hasProperty("length"));
  JSValue_ptr_t length_ptr = result_ptr -> valueForProperty("length");
  XCTAssertEqual(4, static_cast<int32_t>(*length_ptr));

  result_ptr  = context_ptr -> evaluateScript("\"hello, JavaScript\"");
  XCTAssertTrue(result_ptr -> isString());
  XCTAssertTrue(result_ptr -> hasProperty("length"));
  length_ptr = result_ptr -> valueForProperty("length");
  XCTAssertEqual(17, static_cast<int32_t>(*length_ptr));
  
  // Trying to reference an undefined property returns undefined.
  XCTAssertFalse(result_ptr -> hasProperty("foo"));
  auto foo_ptr = result_ptr -> valueForProperty("foo");
  XCTAssertTrue(foo_ptr->isUndefined());
}

- (void)testCustomProperty {
  auto object_ptr  = context_ptr -> evaluateScript("new Object()");
  XCTAssertTrue(object_ptr -> isObject());

  XCTAssertFalse(object_ptr -> hasProperty("foo"));
  object_ptr -> setValueForProperty(JSValue::valueWithInt32InContext(42, context_ptr), "foo");
  XCTAssertTrue(object_ptr -> hasProperty("foo"));
  auto value_ptr = object_ptr -> valueForProperty("foo");
  XCTAssertEqual(42, static_cast<int32_t>(*value_ptr));

  // Should be able to delete a property we created.
  XCTAssertTrue(object_ptr -> deleteProperty("foo"));

  // It is legal to delete a non-existent property.
  XCTAssertTrue(object_ptr -> deleteProperty("bar"));
  
  const std::string quoteString = "Life can only be understood backwards; but it must be lived forwards.";

  // You can't set a custom propery on a bool.
  value_ptr = JSValue::valueWithBoolInContext(true, context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));
  
  // You can't set a custom propery on a double.
  value_ptr = JSValue::valueWithDoubleInContext(3.14, context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));

  // You can't set a custom propery on an int32_t.
  value_ptr = JSValue::valueWithInt32InContext(42, context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));

  // You can't set a custom propery on an uint32_t.
  value_ptr = JSValue::valueWithUInt32InContext(42, context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));

  // You can't set a custom propery on a string.
  value_ptr = JSValue::valueWithStringInContext("hello, world", context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));

  // You can set a custom propery on a string.
  value_ptr = JSValue::valueWithNewObjectInContext(context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertTrue(value_ptr -> hasProperty("quote"));
  XCTAssertEqual(quoteString, static_cast<std::string>(*(value_ptr -> valueForProperty("quote"))));

  // You can set a custom propery on an array.
  value_ptr = JSValue::valueWithNewArrayInContext(context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertTrue(value_ptr -> hasProperty("quote"));
  XCTAssertEqual(quoteString, static_cast<std::string>(*(value_ptr -> valueForProperty("quote"))));

  // You can set a custom propery on a RegExp.
  value_ptr = JSValue::valueWithNewRegularExpressionFromPatternAndFlagsInContext("^Live can only .+$", "", context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertTrue(value_ptr -> hasProperty("quote"));
  XCTAssertEqual(quoteString, static_cast<std::string>(*(value_ptr -> valueForProperty("quote"))));

  // You can set a custom propery on a Error.
  value_ptr = JSValue::valueWithNewErrorFromMessageInContext("Error Message Goes Here", context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertTrue(value_ptr -> hasProperty("quote"));
  XCTAssertEqual(quoteString, static_cast<std::string>(*(value_ptr -> valueForProperty("quote"))));

  // You can't set a custom propery on a Null.
  value_ptr = JSValue::valueWithNullInContext(context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));

  // You can't set a custom propery on a Undefined.
  value_ptr = JSValue::valueWithUndefinedInContext(context_ptr);
  value_ptr -> setValueForProperty(JSValue::valueWithStringInContext(quoteString, context_ptr), "quote");
  XCTAssertFalse(value_ptr -> hasProperty("quote"));
}

- (void)testExamle {
  std::cout << "MDL: sizeof(size_t)" << sizeof(size_t) << std::endl;
  std::cout << "MDL: sizeof(unsigned)" << sizeof(unsigned) << std::endl;
  std::cout << "MDL: sizeof(NSUInteger)" << sizeof(NSUInteger) << std::endl;
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
