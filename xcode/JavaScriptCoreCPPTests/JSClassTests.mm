//
//  JSClassTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 9/22/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JavaScriptCoreCPP/JSClass.h"

@interface JSClassTests : XCTestCase
@end

@implementation JSClassTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
    
    std::cout << "MDL: JSClass::ctorCounter = " << JSClass::ctorCounter() << std::endl;
    std::cout << "MDL: JSClass::dtorCounter = " << JSClass::dtorCounter() << std::endl;
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
