//
//  JSValueTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/18/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/JSValue.hpp"
#import <XCTest/XCTest.h>

using namespace JavaScriptCoreCPP;

@interface JSValueTests2 : XCTestCase
@end

@implementation JSValueTests2 {
  JSContext js_context;
}

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each test method in the class.
  [super tearDown];
}

- (void)testJSValue {
  JSValue js_value(js_context);
  XCTAssertTrue(js_value.isUndefined());
}

- (void)testJSContextCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSValue?
    JSValue js_value(js_context);
  }];
}

@end
