//
//  JSLoggerTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 11/1/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP.hpp"
#import <XCTest/XCTest.h>

@interface JSLoggerTests : XCTestCase
@end

@implementation JSLoggerTests

- (void)setUp {
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testJSLogger {
  JAVASCRIPTCORECPP_LOG_DEBUG("Hello, world");
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
