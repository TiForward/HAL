//
//  JSContextTests.m
//  TiValue
//
//  Created by Matt Langston on 9/10/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JSContext.h"
#include "JSValue.h"

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

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
