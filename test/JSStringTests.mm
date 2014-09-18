//
//  JSStringTests.m
//  TiValue
//
//  Created by Matt Langston on 9/10/14.
//  Copyright (c) 2014 Pedro Enrique. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <XCTest/XCTest.h>
#include "JSString.h"
#include <iostream>

@interface JSStringTests : XCTestCase
@end

@implementation JSStringTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];

    std::cout << "MDL: JSString::ctorCounter = " << JSString::ctorCounter() << std::endl;
    std::cout << "MDL: JSString::dtorCounter = " << JSString::dtorCounter() << std::endl;
}

- (void)testEqual {
    JSString string1 { "hello, world" };
    JSString string2 = string1;
    XCTAssertEqual(string1, string2);
    
    JSString string3 { "hello" };
    XCTAssertNotEqual(string1, string3);

    std::clog << "MDL: " << string1 << std::endl;
}

- (void)testPerformanceExample {
    // This is an example of a performance test case.
    [self measureBlock:^{
        // Put the code you want to measure the time of here.
    }];
}

@end
