//
//  JSObjectTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 9/22/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JavaScriptCoreCPP/JSObject.h"

@interface JSObjectTests : XCTestCase
@end

@implementation JSObjectTests {
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
    
    std::cout << "MDL: JSObject::ctorCounter = " << JSObject::ctorCounter() << std::endl;
    std::cout << "MDL: JSObject::dtorCounter = " << JSObject::dtorCounter() << std::endl;
}

- (void)testExample {
    // This is an example of a functional test case.
    XCTAssert(YES, @"Pass");
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
