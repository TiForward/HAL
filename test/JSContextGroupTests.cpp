/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"

#include "gtest/gtest.h"

#define XCTAssertEqual    ASSERT_EQ
#define XCTAssertNotEqual ASSERT_NE

using namespace JavaScriptCoreCPP;

JSContextGroup makeJSContextGroup() {
	JSContextGroup js_context_group;
	return js_context_group;
}

TEST(JSContextGroupTests, BasicObjectCapabilities) {
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
  
  // Test assignment operator
  JSContextGroup js_context_group_6 = js_context_group_1;
  XCTAssertEqual(js_context_group_1, js_context_group_6);
}
