//
//  JSContextTests.m
//  TiValue
//
//  Created by Matt Langston on 9/10/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JavaScriptCoreCPP/JSContext.h"
#include "JavaScriptCoreCPP/JSValue.h"

@interface JSContextTests : XCTestCase

@end

@implementation JSContextTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    
    std::cout << "MDL: JSContext::ctorCounter = " << JSContext::ctorCounter() << std::endl;
    std::cout << "MDL: JSContext::dtorCounter = " << JSContext::dtorCounter() << std::endl;

    std::cout << "MDL: JSValue::ctorCounter = " << JSValue::ctorCounter() << std::endl;
    std::cout << "MDL: JSValue::dtorCounter = " << JSValue::dtorCounter() << std::endl;

    std::cout << "MDL: JSString::ctorCounter = " << JSString::ctorCounter() << std::endl;
    std::cout << "MDL: JSString::dtorCounter = " << JSString::dtorCounter() << std::endl;
}

- (void)testException{
    auto context_ptr = JSContext::create();
    auto exception_ptr = context_ptr -> exception();
    XCTAssertFalse(static_cast<bool>(exception_ptr));
    
    context_ptr -> setExceptionHandler([](const JSContext_ptr_t& context_ptr, const JSValue_ptr_t& exception_ptr) {
        
        context_ptr->setException(exception_ptr);});
}

- (void)testGlobalObject{
    auto context_ptr = JSContext::create();
    auto globalObject = context_ptr -> globalObject();

    XCTAssertFalse(globalObject->isUndefined());
    XCTAssertFalse(globalObject->isNull());
    XCTAssertFalse(globalObject->isBoolean());
    XCTAssertFalse(globalObject->isNumber());
    XCTAssertFalse(globalObject->isString());
    XCTAssertTrue(globalObject->isObject());
}

- (void)testEvaluateScript {
    auto context_ptr = JSContext::create();
    auto result_ptr  = context_ptr -> evaluateScript("2 + 2");
    std::clog << "MDL(JSContext): result = " << *result_ptr << std::endl;

    //XCTAssertEqual(JSString("\"hello world\"")          , *context_ptr->evaluateScript("JSON.stringify(testMethod('hello world'));"));
//    XCTAssertEqual("[\"hello\",\"world\"]"    , context_ptr->evaluateScript("JSON.stringify(testMethod('hello','world'));"));
//    XCTAssertEqual("[\"hello world\"]"        , context_ptr->evaluateScript("JSON.stringify(testMethod(['hello world']));"));
//    XCTAssertEqual("[[\"hello\"],[\"world\"]]", context_ptr->evaluateScript("JSON.stringify(testMethod(['hello'], ['world']));"));
//    XCTAssertEqual("[1,2,3,4,5,6]"            , context_ptr->evaluateScript("JSON.stringify(testMethod(1,2,3,4,5,6));"));
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
