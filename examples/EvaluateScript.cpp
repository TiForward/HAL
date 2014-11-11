/**
 * JavaScriptCoreCPP
 * Author: Matthew D. Langston
 *
 * Copyright (c) 2014 by Appcelerator, Inc. All Rights Reserved.
 * Licensed under the terms of the Apache Public License.
 * Please see the LICENSE included with this distribution for details.
 */

#include "JavaScriptCoreCPP/JavaScriptCoreCPP.hpp"
#include <iostream>

int main () {
	using namespace JavaScriptCoreCPP;
	JSContextGroup js_context_group;
	JSContext js_context = js_context_group.CreateContext();
	auto js_result       = js_context.JSEvaluateScript("21 / 7");
	std::cout << static_cast<int32_t>(js_result) << std::endl;
}

