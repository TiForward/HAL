//
//  JSContextTests.m
//  JavaScriptCoreCPP
//
//  Created by Matt Langston on 10/18/14.
//  Copyright (c) 2014 Appcelerator. All rights reserved.
//

#include "JavaScriptCoreCPP/RAII/RAII.hpp"
#import <XCTest/XCTest.h>

using namespace JavaScriptCoreCPP::RAII;

JSContext makeJSContext() {
  JSContext js_context;
  return js_context;
}

JSContext makeJSContext(const JSContextGroup& js_context_group) {
  JSContext js_context(js_context_group);
  return js_context;
}

@interface JSContextTests2 : XCTestCase
@end

@implementation JSContextTests2

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each test method in the class.
  [super tearDown];
}

- (void)testJSContext {
  JSContext js_context_1;
  JSContext js_context_2;
  XCTAssertNotEqual(js_context_1, js_context_2);
  
  // Test copy constructor.
  JSContext js_context_3(js_context_1);
  XCTAssertEqual(js_context_1, js_context_3);
  
  // Test copy assignment.
  JSContext js_context_4 = js_context_1;
  XCTAssertEqual(js_context_1, js_context_4);
  
  // Test move constructor.
  JSContext js_context_5(makeJSContext());
  
  // Test unified assignment operator
  JSContext js_context_6 = js_context_1;
  XCTAssertEqual(js_context_1, js_context_6);
  
  // Test creating JSContexts in different groups.
  JSContextGroup js_context_group;
  JSContext js_context_7(js_context_group);
  JSContext js_context_8(js_context_group);
  XCTAssertNotEqual(js_context_7, js_context_8);
  XCTAssertEqual(js_context_7.get_context_group(), js_context_8.get_context_group());
  
  // Test copy constructor.
  JSContext js_context_9(js_context_7);
  XCTAssertEqual(js_context_7, js_context_9);
  
  // Test copy assignment.
  JSContext js_context_10 = js_context_7;
  XCTAssertEqual(js_context_7, js_context_10);
  
  // Test move constructor.
  JSContext js_context_11(makeJSContext(js_context_group));
  
  // Test unified assignment operator
  JSContext js_context_12 = js_context_7;
  XCTAssertEqual(js_context_7, js_context_12);
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testJSContextCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSContext?
    JSContext js_context;
  }];
}
#endif

@end
