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

@interface JSValueTests : XCTestCase
@end

namespace UnitTestConstants {
static const double pi { 3.141592653589793 };
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

    std::cout << "MDL: JSSValue::ctorCounter = " << JSValue::ctorCounter() << std::endl;
    std::cout << "MDL: JSSValue::dtorCounter = " << JSValue::dtorCounter() << std::endl;
}

- (void)testUndefined {
    auto undefinedValue = JSValue(context_ptr);
    XCTAssertTrue(undefinedValue.isUndefined());
    XCTAssertFalse(undefinedValue.isNull());
    XCTAssertFalse(undefinedValue.isBoolean());
    XCTAssertFalse(undefinedValue.isNumber());
    XCTAssertFalse(undefinedValue.isString());
}

- (void)testBool {
    auto falseValue = JSValue(false, context_ptr);
    XCTAssertFalse(bool(falseValue));

    XCTAssertFalse(falseValue.isUndefined());
    XCTAssertFalse(falseValue.isNull());
    XCTAssertTrue(falseValue.isBoolean());
    XCTAssertFalse(falseValue.isNumber());
    XCTAssertFalse(falseValue.isString());
    
    auto trueValue = JSValue(true, context_ptr);
    XCTAssertTrue(bool(trueValue));

    XCTAssertFalse(trueValue.isUndefined());
    XCTAssertFalse(trueValue.isNull());
    XCTAssertTrue(trueValue.isBoolean());
    XCTAssertFalse(trueValue.isNumber());
    XCTAssertFalse(trueValue.isString());
}

- (void)testInt32 {
    auto intValue = JSValue(42, context_ptr);
    XCTAssertEqual(42, int32_t(intValue));

    auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
    XCTAssertEqual(3, int32_t(*result_ptr));

    XCTAssertFalse(intValue.isUndefined());
    XCTAssertFalse(intValue.isNull());
    XCTAssertFalse(intValue.isBoolean());
    XCTAssertTrue(intValue.isNumber());
    XCTAssertFalse(intValue.isString());
}

- (void)testUInt32 {
    auto intValue = JSValue(42, context_ptr);
    XCTAssertEqual(42, uint32_t(intValue));
    
    auto result_ptr  = context_ptr -> evaluateScript("21 / 7");
    XCTAssertEqual(3, uint32_t(*result_ptr));
    
    XCTAssertFalse(intValue.isUndefined());
    XCTAssertFalse(intValue.isNull());
    XCTAssertFalse(intValue.isBoolean());
    XCTAssertTrue(intValue.isNumber());
    XCTAssertFalse(intValue.isString());
}

- (void)testDouble {
    auto doubleValue = JSValue(UnitTestConstants::pi, context_ptr);
    XCTAssertEqualWithAccuracy(UnitTestConstants::pi, double(doubleValue), std::numeric_limits<double>::epsilon());
    
    auto piValue_ptr  = context_ptr -> evaluateScript("Math.PI");
    XCTAssertEqualWithAccuracy(UnitTestConstants::pi, double(*piValue_ptr), std::numeric_limits<double>::epsilon());

    XCTAssertFalse(doubleValue.isUndefined());
    XCTAssertFalse(doubleValue.isNull());
    XCTAssertFalse(doubleValue.isBoolean());
    XCTAssertTrue(doubleValue.isNumber());
    XCTAssertFalse(doubleValue.isString());
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
