//
//  HashUtilitiesTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/20/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#import "HashUtilities.hpp"
#import <XCTest/XCTest.h>

@interface HashUtilitiesTests : XCTestCase

@end

@implementation HashUtilitiesTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testExample {
    // This is an example of a functional test case.
    XCTAssert(YES, @"Pass");
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
