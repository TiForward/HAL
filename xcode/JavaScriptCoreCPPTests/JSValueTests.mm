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
    auto undefinedValue = JSValue::valueWithUndefinedInContext(context_ptr);
    XCTAssertTrue(undefinedValue->isUndefined());
    XCTAssertFalse(undefinedValue->isNull());
    XCTAssertFalse(undefinedValue->isBoolean());
    XCTAssertFalse(undefinedValue->isNumber());
    XCTAssertFalse(undefinedValue->isString());
    XCTAssertFalse(undefinedValue->isObject());
}

- (void)testBool {
    auto falseValue = JSValue::valueWithBoolInContext(false, context_ptr);
    XCTAssertFalse(static_cast<bool>(*falseValue));

    XCTAssertFalse(falseValue->isUndefined());
    XCTAssertFalse(falseValue->isNull());
    XCTAssertTrue(falseValue->isBoolean());
    XCTAssertFalse(falseValue->isNumber());
    XCTAssertFalse(falseValue->isString());
    XCTAssertFalse(falseValue->isObject());
    
    auto trueValue = JSValue::valueWithBoolInContext(true, context_ptr);
    XCTAssertTrue(static_cast<bool>(*trueValue));

    XCTAssertFalse(trueValue->isUndefined());
    XCTAssertFalse(trueValue->isNull());
    XCTAssertTrue(trueValue->isBoolean());
    XCTAssertFalse(trueValue->isNumber());
    XCTAssertFalse(trueValue->isString());
    XCTAssertFalse(trueValue->isObject());
}

- (void)testDouble {
    auto doubleValue = JSValue::valueWithDoubleInContext(UnitTestConstants::pi, context_ptr);
    XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*doubleValue), std::numeric_limits<double>::epsilon());
    
    auto piValue_ptr  = context_ptr -> evaluateScript("Math.PI");
    XCTAssertEqualWithAccuracy(UnitTestConstants::pi, static_cast<double>(*piValue_ptr), std::numeric_limits<double>::epsilon());
    
    XCTAssertFalse(doubleValue->isUndefined());
    XCTAssertFalse(doubleValue->isNull());
    XCTAssertFalse(doubleValue->isBoolean());
    XCTAssertTrue(doubleValue->isNumber());
    XCTAssertFalse(doubleValue->isString());
    XCTAssertFalse(doubleValue->isObject());
}

- (void)testInt32 {
    auto intValue = JSValue::valueWithInt32InContext(42, context_ptr);
    XCTAssertEqual(42, static_cast<int32_t>(*intValue));

    auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
    XCTAssertEqual(3, static_cast<int32_t>(*result_ptr));

    XCTAssertFalse(intValue->isUndefined());
    XCTAssertFalse(intValue->isNull());
    XCTAssertFalse(intValue->isBoolean());
    XCTAssertTrue(intValue->isNumber());
    XCTAssertFalse(intValue->isString());
    XCTAssertFalse(intValue->isObject());
}

- (void)testUInt32 {
    auto uintValue = JSValue::valueWithUInt32InContext(42, context_ptr);
    XCTAssertEqual(42, static_cast<uint32_t>(*uintValue));
    
    auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
    XCTAssertEqual(3, static_cast<uint32_t>(*result_ptr));
    
    XCTAssertFalse(uintValue->isUndefined());
    XCTAssertFalse(uintValue->isNull());
    XCTAssertFalse(uintValue->isBoolean());
    XCTAssertTrue(uintValue->isNumber());
    XCTAssertFalse(uintValue->isString());
    XCTAssertFalse(uintValue->isObject());
}

- (void)testString {
    auto stringValue = JSValue::valueWithStringInContext("hello, world", context_ptr);
    std::cout << "MDL: " << stringValue << std:: endl;
//    XCTAssertEqual("hello, world", static_cast<std::string>(*stringValue));
    
//    auto result_ptr  = context_ptr -> evaluateScript("\"hello, JavaScript\"");
//    XCTAssertEqual("hello, JavaScript", static_cast<std::string>(*result_ptr));
    
    XCTAssertFalse(stringValue->isUndefined());
    XCTAssertFalse(stringValue->isNull());
    XCTAssertFalse(stringValue->isBoolean());
    XCTAssertFalse(stringValue->isNumber());
    XCTAssertTrue(stringValue->isString());
    XCTAssertFalse(stringValue->isObject());
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
