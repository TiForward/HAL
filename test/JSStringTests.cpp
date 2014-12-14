/**
 * HAL
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "HAL/HAL.hpp"

#include <string>
#include <iostream>

#include "gtest/gtest.h"

#define XCTAssertEqual    ASSERT_EQ
#define XCTAssertNotEqual ASSERT_NE
#define XCTAssertTrue     ASSERT_TRUE
#define XCTAssertFalse    ASSERT_FALSE

using namespace HAL;

JSString makeJSString() {
  JSString js_string;
  return js_string;
}

TEST(JSStringTests, BasicObjectCapabilities) {
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

TEST(JSStringTests, EmptyString) {
  JSString string;
  XCTAssertEqual(0, string.length());
  
  std::string stdString = string;
  XCTAssertEqual(0, stdString.length());
  XCTAssertTrue(stdString.empty());
}

TEST(JSStringTests, Equal) {
  JSString string1 { "hello, world" };
  JSString string2 = string1;
  XCTAssertEqual(string1, string2);
  
  JSString string3 { "hello" };
  XCTAssertNotEqual(string1, string3);
}

TEST(JSStringTests, StdString) {
  JSString string1 { "hello, JSString" };
  XCTAssertEqual("hello, JSString", static_cast<std::string>(string1));
  
  // No implicit conversions.
  //XCTAssertEqual(std::string("hello, JSString"), string1);
  
  std::string string2 { "hello, std::string" };
  XCTAssertEqual("hello, std::string", static_cast<std::string>(JSString(string2)));

  // No implicit conversions.
  //XCTAssertEqual(std::string("hello, std::string"), JSString(string2));
}
