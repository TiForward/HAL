//
//  JSContextGroupTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/18/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#import <XCTest/XCTest.h>

#ifdef JAVASCRIPTCORECPP_RAII_ENABLE_PERFORMANCE_COUNTER
static JavaScriptCoreCPP::detail::JSPerformanceCounterPrinter js_performancecounter_printer;
#endif

using namespace JavaScriptCoreCPP::RAII;

JSContextGroup makeJSContextGroup() {
  JSContextGroup js_context_group;
  return js_context_group;
}

@interface JSContextGroupTests : XCTestCase
@end

@implementation JSContextGroupTests

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each test method in the class.
  [super tearDown];
}

- (void)testJSContextGroup {
  JSContextGroup js_context_group_1;
  JSContextGroup js_context_group_2;
  XCTAssertNotEqual(js_context_group_1, js_context_group_2);

  // Test copy constructor.
  JSContextGroup js_context_group_3(js_context_group_1);
  XCTAssertEqual(js_context_group_1, js_context_group_3);

  // Test copy assignment.
  JSContextGroup js_context_group_4 = js_context_group_1;
  XCTAssertEqual(js_context_group_1, js_context_group_4);

  // Test move constructor.
  JSContextGroup js_context_group_5(makeJSContextGroup());
  
  // Test unified assignment operator
  JSContextGroup js_context_group_6 = js_context_group_1;
  XCTAssertEqual(js_context_group_1, js_context_group_6);
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testJSContextGroupCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSContextGroup?
    JSContextGroup js_context_group;
  }];
}
#endif

@end
