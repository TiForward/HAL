/**
 * HAL
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/HAL.hpp"
#import <XCTest/XCTest.h>

using namespace HAL;

JSString makeJSString() {
  JSString js_string;
  return js_string;
}

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
}

- (void)testJSString {
  JSString js_string_1;
  JSString js_string_2;
  XCTAssertEqual(js_string_1, js_string_2);
  
  // Test copy constructor.
  JSString js_string_3(js_string_1);
  XCTAssertEqual(js_string_1, js_string_3);
  
  // Test copy assignment.
  JSString js_string_4 = js_string_1;
  XCTAssertEqual(js_string_1, js_string_4);
  
  // Test move constructor.
  JSString js_string_5(makeJSString());
  
  // Test unified assignment operator
  JSString js_string_6 = js_string_1;
  XCTAssertEqual(js_string_1, js_string_6);
}

- (void)testEmptyString {
  JSString string;
  XCTAssertEqual(0, string.length());
  
  std::string stdString = string;
  XCTAssertEqual(0, stdString.length());
  XCTAssertTrue(stdString.empty());
}

- (void)testEqual {
  JSString string1 { "hello, world" };
  JSString string2 = string1;
  XCTAssertEqual(string1, string2);
  
  JSString string3 { "hello" };
  XCTAssertNotEqual(string1, string3);
}

- (void)testStdString {
  JSString string1 { "hello, JSString" };
  XCTAssertEqual("hello, JSString", static_cast<std::string>(string1));
  
  // No implicit conversions.
  //XCTAssertEqual(std::string("hello, JSString"), string1);
  
  std::string string2 { "hello, std::string" };
  XCTAssertEqual("hello, std::string", static_cast<std::string>(JSString(string2)));

  // No implicit conversions.
  //XCTAssertEqual(std::string("hello, std::string"), JSString(string2));
}

// As of 2014.09.20 Travis CI only supports Xcode 5.1 which lacks support for
// measureBlock.
#ifndef TRAVIS
- (void)testJSStringCreationPerformance {
  [self measureBlock:^{
    // How long does it take to create a JSString?
    JSString string;
  }];
}
#endif

@end
